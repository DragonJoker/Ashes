#include "GeometryPass.hpp"

#include "RenderTarget.hpp"

#include <Ashes/Buffer/Buffer.hpp>
#include <Ashes/Buffer/StagingBuffer.hpp>
#include <Ashes/Buffer/VertexBuffer.hpp>
#include <Ashes/Command/CommandBuffer.hpp>
#include <Ashes/Command/CommandPool.hpp>
#include <Ashes/Core/Device.hpp>
#include <Ashes/Descriptor/DescriptorSet.hpp>
#include <Ashes/Descriptor/DescriptorSetLayout.hpp>
#include <Ashes/Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Ashes/Descriptor/DescriptorSetPool.hpp>
#include <Ashes/Image/Texture.hpp>
#include <Ashes/Image/TextureView.hpp>
#include <Ashes/Pipeline/DepthStencilState.hpp>
#include <Ashes/Pipeline/InputAssemblyState.hpp>
#include <Ashes/Pipeline/MultisampleState.hpp>
#include <Ashes/Pipeline/Pipeline.hpp>
#include <Ashes/Pipeline/PipelineLayout.hpp>
#include <Ashes/Pipeline/Scissor.hpp>
#include <Ashes/Pipeline/VertexLayout.hpp>
#include <Ashes/Pipeline/Viewport.hpp>
#include <Ashes/RenderPass/RenderPass.hpp>
#include <Ashes/RenderPass/RenderSubpass.hpp>
#include <Ashes/RenderPass/RenderSubpassState.hpp>
#include <Ashes/RenderPass/FrameBufferAttachment.hpp>
#include <Ashes/Sync/ImageMemoryBarrier.hpp>

#include <Utils/GlslToSpv.hpp>

#include <FileUtils.hpp>

#include <algorithm>

namespace vkapp
{
	namespace
	{
		std::vector< ashes::Format > doGetFormats( GeometryPassResult const & gbuffer
			, ashes::Format depthFormat )
		{
			std::vector< ashes::Format > result
			{
				depthFormat,
			};

			for ( auto & texture : gbuffer )
			{
				result.push_back( texture.view->getFormat() );
			}

			return result;
		}

		ashes::TextureViewCRefArray doGetViews( GeometryPassResult const & gbuffer
			, ashes::TextureView const & depthview )
		{
			ashes::TextureViewCRefArray result
			{
				depthview
			};

			for ( auto & texture : gbuffer )
			{
				result.emplace_back( *texture.view );
			}

			return result;
		}
	}

	GeometryPass::GeometryPass( ashes::Device const & device
		, std::string const & fragmentShaderFile
		, GeometryPassResult const & gbuffer
		, ashes::Format depthFormat
		, ashes::UniformBuffer< common::SceneData > const & sceneUbo
		, ashes::UniformBuffer< common::ObjectData > const & objectUbo )
		: common::NodesRenderer{ device
			, fragmentShaderFile
			, doGetFormats( gbuffer, depthFormat )
			, true
			, true }
		, m_sceneUbo{ sceneUbo }
		, m_objectUbo{ objectUbo }
	{
	}

	void GeometryPass::update( common::RenderTarget const & target )
	{
		doUpdate( doGetViews( static_cast< RenderTarget const & >( target ).getGBuffer()
			, target.getDepthView() ) );
	}

	void GeometryPass::doFillObjectDescriptorLayoutBindings( ashes::DescriptorSetLayoutBindingArray & bindings )
	{
		bindings.emplace_back( 1u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eVertex );
		bindings.emplace_back( 2u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eVertex );
	}

	void GeometryPass::doFillObjectDescriptorSet( ashes::DescriptorSetLayout & descriptorLayout
		, ashes::DescriptorSet & descriptorSet )
	{
		descriptorSet.createBinding( descriptorLayout.getBinding( 1u )
			, m_sceneUbo
			, 0u
			, 1u );
		descriptorSet.createBinding( descriptorLayout.getBinding( 2u )
			, m_objectUbo
			, 0u
			, 1u );
	}
}
