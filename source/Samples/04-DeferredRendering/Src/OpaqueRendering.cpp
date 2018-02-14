#include "OpaqueRendering.hpp"

#include "RenderTarget.hpp"

#include <Buffer/Buffer.hpp>
#include <Buffer/GeometryBuffers.hpp>
#include <Buffer/StagingBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <Command/CommandBuffer.hpp>
#include <Command/CommandPool.hpp>
#include <Core/Device.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/Texture.hpp>
#include <Image/TextureView.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderPassState.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <RenderPass/FrameBufferAttachment.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <FileUtils.hpp>

#include <algorithm>

namespace vkapp
{
	namespace
	{
		renderer::TextureViewCRefArray doGetViews( GeometryPassResult const & gbuffer
			, renderer::TextureViewCRefArray const & views )
		{
			renderer::TextureViewCRefArray result
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
		, renderer::StagingBuffer & stagingBuffer
		, GeometryPassResult const & gbuffer
		, renderer::TextureViewCRefArray const & views
		, common::TextureNodePtrArray const & textureNodes
		, renderer::UniformBuffer< common::SceneData > const & sceneUbo
		, renderer::UniformBuffer< common::LightsData > const & lightsUbo )
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

	bool OpaqueRendering::draw( std::chrono::nanoseconds & gpu )const
	{
		auto result = m_renderer->draw( gpu );
		result &= m_lightingPass.draw( gpu );
		return result;
	}
}
