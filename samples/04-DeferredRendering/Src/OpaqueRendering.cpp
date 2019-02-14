#include "OpaqueRendering.hpp"

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
		ashes::TextureViewCRefArray doGetViews( GeometryPassResult const & gbuffer
			, ashes::TextureViewCRefArray const & views )
		{
			ashes::TextureViewCRefArray result
			{
				views[0].get()
			};

			for ( auto & texture : gbuffer )
			{
				result.emplace_back( *texture.view );
			}

			return result;
		}
	}

	OpaqueRendering::OpaqueRendering( std::unique_ptr< GeometryPass > && renderer
		, common::Scene const & scene
		, ashes::StagingBuffer & stagingBuffer
		, GeometryPassResult const & gbuffer
		, ashes::TextureViewCRefArray const & views
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
		, m_lightingPass{ m_renderer->getDevice()
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
		m_renderer->update( target );
		m_lightingPass.update( m_sceneUbo.getData( 0u )
			, m_stagingBuffer
			, { target.getDepthView(), target.getColourView() }
			, static_cast< RenderTarget const & >( target ).getGBuffer() );
	}

	void OpaqueRendering::draw( std::chrono::nanoseconds & gpu )const
	{
		m_renderer->draw( gpu );
		m_lightingPass.draw( gpu );
	}
}
