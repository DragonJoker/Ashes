#include "GeometryPass.hpp"

#include "RenderTarget.hpp"

#include <ashespp/Buffer/Buffer.hpp>
#include <ashespp/Buffer/StagingBuffer.hpp>
#include <ashespp/Buffer/VertexBuffer.hpp>
#include <ashespp/Command/CommandBuffer.hpp>
#include <ashespp/Command/CommandPool.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Descriptor/DescriptorSet.hpp>
#include <ashespp/Descriptor/DescriptorSetLayout.hpp>
#include <ashespp/Descriptor/DescriptorSetPool.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/Pipeline/GraphicsPipeline.hpp>
#include <ashespp/Pipeline/PipelineLayout.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>

#include <util/GlslToSpv.hpp>

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
				result.emplace_back( texture.view.getFormat() );
			}

			return result;
		}

		ashes::ImageView doCloneView( ashes::ImageView const & view )
		{
			return view.image->createView( view.createInfo );
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
		, ashes::UniformBuffer const & sceneUbo
		, ashes::UniformBuffer const & objectUbo )
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
		bindings.push_back( { 1u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_VERTEX_BIT, nullptr } );
		bindings.push_back( { 2u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_VERTEX_BIT, nullptr } );
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
