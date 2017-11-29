#include "RenderTarget.h"

#include "OverlayRenderer.h"
#include "Scene.h"

#include <Renderer/FrameBuffer.hpp>
#include <Renderer/ImageMemoryBarrier.hpp>
#include <Renderer/RenderBuffer.hpp>
#include <Renderer/RenderPass.hpp>
#include <Renderer/RenderPassState.hpp>
#include <Renderer/RenderSubpass.hpp>
#include <Renderer/RenderSubpassState.hpp>
#include <Renderer/StagingBuffer.hpp>
#include <Renderer/Texture.hpp>

namespace render
{
	namespace
	{
		std::vector< utils::PixelFormat > doGetPixelFormats()
		{
			return { utils::PixelFormat::eR8G8B8A8, utils::PixelFormat::eD24S8 };
		}

		renderer::RenderSubpassState doGetSubpassState()
		{
			return { renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite | renderer::AccessFlag::eColourAttachmentRead };
		}

		renderer::RenderPassState doGetInitialState()
		{
			return renderer::RenderPassState{ renderer::PipelineStageFlag::eBottomOfPipe
				, renderer::AccessFlag::eMemoryRead
				, { renderer::ImageLayout::eUndefined
					, renderer::ImageLayout::eUndefined } };
		}

		renderer::RenderPassState doGetFinalState()
		{
			return renderer::RenderPassState{ renderer::PipelineStageFlag::eBottomOfPipe
				, renderer::AccessFlag::eMemoryRead
				, { renderer::ImageLayout::eShaderReadOnlyOptimal
					, renderer::ImageLayout::eDepthStencilAttachmentOptimal } };
		}
	}

	RenderTarget::RenderTarget( renderer::Device const & device
		, utils::IVec2 const & dimensions
		, utils::PixelFormat format )
		: m_size{ dimensions }
		, m_renderPass{ std::make_unique< renderer::RenderPass >( device
			, doGetPixelFormats()
			, renderer::RenderSubpassArray{ { device, doGetPixelFormats(), doGetSubpassState() } }
			, doGetInitialState()
			, doGetFinalState() ) }
		, m_colour{ std::make_unique< renderer::Texture >( device ) }
		, m_depth{ std::make_unique< renderer::Texture >( device ) }
		, m_stagingBuffer{ std::make_shared< renderer::StagingBuffer >( device, 10000000 ) }
	{
		m_colour->setImage( format
			, dimensions
			, renderer::ImageUsageFlag::eColourAttachment | renderer::ImageUsageFlag::eSampled );
		m_depth->setImage( utils::PixelFormat::eD24S8
			, dimensions
			, renderer::ImageUsageFlag::eDepthStencilAttachment );
		m_framebuffer = std::make_shared< renderer::FrameBuffer >( *m_renderPass
			, dimensions
			, renderer::TextureCRefArray{ *m_colour, *m_depth } );
	}

	RenderTarget::~RenderTarget()noexcept
	{
	}

	void RenderTarget::updateScene( renderer::CommandBuffer const & commandBuffer
		, Scene & scene )const
	{
		scene.updateUbos( *m_stagingBuffer
			, commandBuffer );

	}

	void RenderTarget::updateOverlays( renderer::CommandBuffer const & commandBuffer
		, OverlayRenderer & renderer )const
	{
		renderer.updateBuffers( *m_stagingBuffer
			, commandBuffer );
	}

	void RenderTarget::preRenderCommands( renderer::CommandBuffer const & commandBuffer )const
	{
		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
			, renderer::PipelineStageFlag::eColourAttachmentOutput
			, m_colour->makeColourAttachment() );
		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
			, renderer::PipelineStageFlag::eColourAttachmentOutput
			, m_depth->makeDepthStencilAttachment() );
	}

	void RenderTarget::render( renderer::CommandBuffer const & commandBuffer
		, Scene const & scene )const
	{
		scene.draw( commandBuffer );
	}

	void RenderTarget::postRenderCommands( renderer::CommandBuffer const & commandBuffer )const
	{
		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
			, renderer::PipelineStageFlag::eFragmentShader
			, m_colour->makeShaderInputResource() );
		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
			, renderer::PipelineStageFlag::eFragmentShader
			, m_depth->makeDepthStencilReadOnly() );
	}
}
