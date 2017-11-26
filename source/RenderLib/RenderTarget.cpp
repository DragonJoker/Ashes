#include "RenderTarget.h"

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
	RenderTarget::RenderTarget( renderer::Device const & device
		, utils::IVec2 const & dimensions
		, utils::PixelFormat format )
		: m_size{ dimensions }
		, m_renderPass{ std::make_unique< renderer::RenderPass >( device
			, std::vector< utils::PixelFormat >{ utils::PixelFormat::eR8G8B8A8, utils::PixelFormat::eD16 }
			, renderer::RenderSubpassArray{ { device
				, std::vector< utils::PixelFormat >{ utils::PixelFormat::eR8G8B8A8, utils::PixelFormat::eD16 }
				, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput, renderer::AccessFlag::eColourAttachmentWrite } } }
				, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite
					, { renderer::ImageLayout::eColourAttachmentOptimal
						, renderer::ImageLayout::eDepthStencilAttachmentOptimal } }
				, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite
					, { renderer::ImageLayout::eColourAttachmentOptimal
						, renderer::ImageLayout::eDepthStencilAttachmentOptimal } } ) }
		, m_colour{ std::make_unique< renderer::Texture >( device ) }
		, m_depth{ std::make_unique< renderer::Texture >( device ) }
		, m_stagingBuffer{ std::make_shared< renderer::StagingBuffer >( device ) }
	{
		m_colour->setImage( format, dimensions, renderer::ImageUsageFlag::eColourAttachment );
		m_depth->setImage( utils::PixelFormat::eD16, dimensions, renderer::ImageUsageFlag::eDepthStencilAttachment );
		m_framebuffer = std::make_shared< renderer::FrameBuffer >( *m_renderPass
			, dimensions
			, renderer::TextureCRefArray{ *m_colour, *m_depth } );
	}

	RenderTarget::~RenderTarget()noexcept
	{
	}

	void RenderTarget::drawScene( renderer::CommandBuffer const & commandBuffer
		, Scene const & scene )const noexcept
	{
		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
			, renderer::PipelineStageFlag::eColourAttachmentOutput
			, m_colour->makeColourAttachment() );
		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
			, renderer::PipelineStageFlag::eColourAttachmentOutput
			, m_depth->makeColourAttachment() );

		scene.draw( *m_stagingBuffer
			, commandBuffer );

		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
			, renderer::PipelineStageFlag::eFragmentShader
			, m_colour->makeShaderInputResource() );
		commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
			, renderer::PipelineStageFlag::eFragmentShader
			, m_depth->makeDepthStencilReadOnly() );
	}
}
