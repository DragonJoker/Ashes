#include "GeometryPass.hpp"

#include "RenderTarget.hpp"

#include <AshesPP/Buffer/Buffer.hpp>
#include <AshesPP/Buffer/StagingBuffer.hpp>
#include <AshesPP/Buffer/VertexBuffer.hpp>
#include <AshesPP/Command/CommandBuffer.hpp>
#include <AshesPP/Command/CommandPool.hpp>
#include <AshesPP/Core/Device.hpp>
#include <AshesPP/Descriptor/DescriptorSet.hpp>
#include <AshesPP/Descriptor/DescriptorSetLayout.hpp>
#include <AshesPP/Descriptor/DescriptorSetPool.hpp>
#include <AshesPP/Image/Image.hpp>
#include <AshesPP/Image/ImageView.hpp>
#include <AshesPP/Pipeline/GraphicsPipeline.hpp>
#include <AshesPP/Pipeline/PipelineLayout.hpp>
#include <AshesPP/RenderPass/RenderPass.hpp>

#include <Utils/GlslToSpv.hpp>

#include <FileUtils.hpp>

#include <algorithm>

namespace vkapp
{
	namespace
	{
		std::vector< VkFormat > doGetFormats( GeometryPassResult const & gbuffer
			, VkFormat depthFormat )
		{
			std::vector< VkFormat > result
			{
				depthFormat,
			};

			for ( auto & texture : gbuffer )
			{
				result.push_back( texture.view->getFormat() );
			}

			return result;
		}

		ashes::ImageView doCloneView( ashes::ImageView const & view )
		{
			return view.image->createView(
				{
					view->viewType,
					view->format,
					view->components,
					view->subresourceRange,
				} );
		}

		ashes::ImageViewArray doGetViews( GeometryPassResult const & gbuffer
			, ashes::ImageView depthview )
		{
			ashes::ImageViewArray result;
			result.emplace_back( depthview );

			for ( auto & texture : gbuffer )
			{
				result.emplace_back( texture.view );
			}

			return result;
		}
	}

	GeometryPass::GeometryPass( utils::Device const & device
		, ashes::CommandPool const & commandPool
		, ashes::Queue const & transferQueue
		, std::string const & fragmentShaderFile
		, GeometryPassResult const & gbuffer
		, VkFormat depthFormat
		, ashes::UniformBuffer< common::SceneData > const & sceneUbo
		, ashes::UniformBuffer< common::ObjectData > const & objectUbo )
		: common::NodesRenderer{ device
			, commandPool
			, transferQueue
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

	void GeometryPass::doFillObjectDescriptorLayoutBindings( ashes::VkDescriptorSetLayoutBindingArray & bindings )
	{
		bindings.emplace_back( 1u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT );
		bindings.emplace_back( 2u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT );
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
