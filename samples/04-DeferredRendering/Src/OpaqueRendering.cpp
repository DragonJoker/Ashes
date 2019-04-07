#include "OpaqueRendering.hpp"

#include "RenderTarget.hpp"

#include <AshesPP/Buffer/Buffer.hpp>
#include <AshesPP/Buffer/StagingBuffer.hpp>
#include <AshesPP/Buffer/VertexBuffer.hpp>
#include <AshesPP/Command/CommandBuffer.hpp>
#include <AshesPP/Command/CommandPool.hpp>
#include <AshesPP/Core/Device.hpp>
#include <AshesPP/Descriptor/DescriptorSet.hpp>
#include <AshesPP/Descriptor/DescriptorSetLayout.hpp>
#include <AshesPP/Descriptor/DescriptorSetLayoutBinding.hpp>
#include <AshesPP/Descriptor/DescriptorSetPool.hpp>
#include <AshesPP/Image/Image.hpp>
#include <AshesPP/Image/ImageView.hpp>
#include <AshesPP/Pipeline/DepthStencilState.hpp>
#include <AshesPP/Pipeline/MultisampleState.hpp>
#include <AshesPP/Pipeline/GraphicsPipeline.hpp>
#include <AshesPP/Pipeline/PipelineLayout.hpp>
#include <AshesPP/Pipeline/Scissor.hpp>
#include <AshesPP/Pipeline/VertexLayout.hpp>
#include <AshesPP/Pipeline/Viewport.hpp>
#include <AshesPP/RenderPass/RenderPass.hpp>
#include <AshesPP/RenderPass/RenderSubpass.hpp>
#include <AshesPP/RenderPass/RenderSubpassState.hpp>
#include <AshesPP/RenderPass/FrameBufferAttachment.hpp>
#include <AshesPP/Sync/ImageMemoryBarrier.hpp>

#include <Utils/GlslToSpv.hpp>

#include <FileUtils.hpp>

#include <algorithm>

namespace vkapp
{
	namespace
	{
		ashes::ImageViewPtrArray doGetViews( GeometryPassResult const & gbuffer
			, ashes::ImageViewPtrArray views )
		{
			ashes::ImageViewPtrArray result
			{
				views[0]
			};

			for ( auto & texture : gbuffer )
			{
				result.emplace_back( texture.view );
			}

			return result;
		}
	}

	OpaqueRendering::OpaqueRendering( std::unique_ptr< GeometryPass > renderer
		, common::Scene const & scene
		, ashes::StagingBuffer & stagingBuffer
		, GeometryPassResult const & gbuffer
		, ashes::ImageViewPtrArray views
		, common::TextureNodePtrArray const & textureNodes
		, ashes::UniformBuffer< common::SceneData > const & sceneUbo
		, ashes::UniformBuffer< common::LightsData > const & lightsUbo )
		: common::OpaqueRendering{ std::move( renderer )
			, scene
			, stagingBuffer
			, doGetViews( gbuffer, views )
			, textureNodes }
		, m_sceneUbo{ sceneUbo }
		, m_lightsUbo{ lightsUbo }
		, m_stagingBuffer{ stagingBuffer }
		, m_lightingPass{ m_instance->getDevice()
			, getCommandPool()
			, getTransferQueue()
			, lightsUbo
			, stagingBuffer
			, views }
	{
		m_lightingPass.update( m_sceneUbo.getData( 0u )
			, m_stagingBuffer
			, views
			, gbuffer );
	}

	void OpaqueRendering::update( common::RenderTarget const & target )
	{
		m_instance->update( target );
		m_lightingPass.update( m_sceneUbo.getData( 0u )
			, m_stagingBuffer
			, { target.getDepthView(), target.getColourView() }
			, static_cast< RenderTarget const & >( target ).getGBuffer() );
	}

	void OpaqueRendering::draw( ashes::Queue const & queue
		, std::chrono::nanoseconds & gpu )const
	{
		m_instance->draw( queue, gpu );
		m_lightingPass.draw( queue, gpu );
	}
}
