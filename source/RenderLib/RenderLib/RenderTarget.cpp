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
		std::vector< renderer::PixelFormat > doGetPixelFormats()
		{
			return { renderer::PixelFormat::eR8G8B8A8, renderer::PixelFormat::eD24S8 };
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

		renderer::RenderSubpassPtrArray doGetRenderSubpasses( renderer::Device const & device )
		{
			renderer::RenderSubpassPtrArray result;
			result.emplace_back( device.createRenderSubpass( doGetPixelFormats(), doGetSubpassState() ) );
			return result;
		}
	}

	RenderTarget::RenderTarget( renderer::Device const & device
		, renderer::IVec2 const & dimensions
		, renderer::PixelFormat format )
		: m_size{ dimensions }
		, m_renderPass{ device.createRenderPass( doGetPixelFormats()
			, doGetRenderSubpasses( device)
			, doGetInitialState()
			, doGetFinalState() ) }
		, m_colour{ device.createTexture() }
		, m_depth{ device.createTexture() }
		, m_stagingBuffer{ std::make_unique< renderer::StagingBuffer >( device
			, 0u
			, 10000000u ) }
	{
		m_colour->setImage( format
			, dimensions
			, renderer::ImageUsageFlag::eColourAttachment | renderer::ImageUsageFlag::eSampled );
		m_depth->setImage( renderer::PixelFormat::eD24S8
			, dimensions
			, renderer::ImageUsageFlag::eDepthStencilAttachment );
		m_framebuffer = m_renderPass->createFrameBuffer( dimensions
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

	void RenderTarget::preRenderCommands( renderer::CommandBuffer const & commandBuffer )const
	{
		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
			, renderer::PipelineStageFlag::eColourAttachmentOutput
			, m_colour->makeColourAttachment() );
		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
			, renderer::PipelineStageFlag::eColourAttachmentOutput
			, m_depth->makeDepthStencilAttachment() );
	}

	void RenderTarget::render( Scene const & scene
		, OverlayRenderer const & overlayRenderer )const
	{
		scene.draw( *m_framebuffer );
		overlayRenderer.draw( *m_framebuffer
			, scene.overlays() );
	}

	void RenderTarget::render( renderer::CommandBuffer const & commandBuffer
		, Scene const & scene
		, OverlayRenderer const & overlayRenderer )const
	{
		scene.draw( commandBuffer );
		overlayRenderer.draw( commandBuffer
			, scene.overlays() );
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
