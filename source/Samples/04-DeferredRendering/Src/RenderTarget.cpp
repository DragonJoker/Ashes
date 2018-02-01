#include "RenderTarget.hpp"

#include <Buffer/StagingBuffer.hpp>
#include <Buffer/UniformBuffer.hpp>
#include <Command/Queue.hpp>
#include <Image/Texture.hpp>
#include <Image/TextureView.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderPassState.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <Utils/Transform.hpp>

#include <chrono>

namespace vkapp
{
	namespace
	{
		static renderer::PixelFormat const DepthFormat = renderer::PixelFormat::eD32F;
	}

	RenderTarget::RenderTarget( renderer::Device const & device
		, renderer::UniformBuffer< common::LightsData > const & lightsUbo
		, renderer::UIVec2 const & size
		, common::Object && object )
		: m_device{ device }
		, m_lightsUbo{ lightsUbo }
		, m_size{ size }
		, m_object{ std::move( object ) }
	{
		try
		{
			doCreateStagingBuffer();
			std::cout << "Staging buffer created." << std::endl;
			doCreateUniformBuffer();
			std::cout << "Uniform buffers created." << std::endl;
			doCreateRenderPass();
			std::cout << "Offscreen render pass created." << std::endl;
			doCreateFrameBuffer();
			std::cout << "Frame buffer created." << std::endl;
			doPrepareFrame();
			std::cout << "Frame prepared." << std::endl;
		}
		catch ( std::exception & )
		{
			doCleanup();
			throw;
		}

	}

	RenderTarget::~RenderTarget()
	{
		doCleanup();
	}

	void RenderTarget::resize( renderer::UIVec2 const & size )
	{
#if 0
		float halfWidth = static_cast< float >( size.x ) * 0.5f;
		float halfHeight = static_cast< float >( size.y ) * 0.5f;
		float wRatio = 1.0f;
		float hRatio = 1.0f;

		if ( halfHeight > halfWidth )
		{
			hRatio = halfHeight / halfWidth;
		}
		else
		{
			wRatio = halfWidth / halfHeight;
		}

		m_matrixUbo->getData( 0u ) = m_device->ortho( -2.0f * wRatio
			, 2.0f * wRatio
			, -2.0f * hRatio
			, 2.0f * hRatio
			, 0.0f
			, 10.0f );
#else
		auto width = float( size[0] );
		auto height = float( size[1] );
		m_matrixUbo->getData( 0u ) = m_device.perspective( utils::toRadians( 90.0_degrees )
			, width / height
			, 0.01f
			, 100.0f );
#endif
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_matrixUbo->getDatas()
			, *m_matrixUbo
			, renderer::PipelineStageFlag::eVertexShader );
		m_size = size;
	}

	void RenderTarget::update()
	{
		static renderer::Mat4 const originalTranslate = []()
		{
			renderer::Mat4 result;
			result = utils::translate( result, { 0, 0, -5 } );
			return result;
		}( );
		static renderer::Clock::time_point save = renderer::Clock::now();
		auto duration = std::chrono::duration_cast< std::chrono::microseconds >( renderer::Clock::now() - save );
		m_rotate = utils::rotate( m_rotate
			, float( utils::DegreeToRadian ) * ( duration.count() / 20000.0f )
			, { 0, 1, 0 } );
		m_objectUbo->getData( 0 ) = originalTranslate * m_rotate;
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_objectUbo->getDatas()
			, *m_objectUbo
			, renderer::PipelineStageFlag::eVertexShader );
		save = renderer::Clock::now();
	}

	void RenderTarget::draw()
	{
		auto & queue = m_device.getGraphicsQueue();
		auto res = queue.submit( *m_commandBuffer
			, nullptr );
	}

	void RenderTarget::doCleanup()
	{
		m_updateCommandBuffer.reset();

		m_stagingBuffer.reset();

		m_matrixUbo.reset();
		m_objectUbo.reset();

		m_frameBuffer.reset();
		m_renderPass.reset();
		m_depthView.reset();
		m_depth.reset();
		m_colourView.reset();
		m_colour.reset();
	}

	void RenderTarget::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique< renderer::StagingBuffer >( m_device
			, 0u
			, 65u * 1024u );
	}

	void RenderTarget::doCreateUniformBuffer()
	{
		m_matrixUbo = std::make_unique< renderer::UniformBuffer< renderer::Mat4 > >( m_device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_objectUbo = std::make_unique< renderer::UniformBuffer< renderer::Mat4 > >( m_device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
	}

	void RenderTarget::doCreateRenderPass()
	{
		std::vector< renderer::PixelFormat > formats{ { renderer::PixelFormat::eR8G8B8A8, DepthFormat } };
		renderer::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( m_device.createRenderSubpass( formats
			, { renderer::PipelineStageFlag::eColourAttachmentOutput, renderer::AccessFlag::eColourAttachmentWrite } ) );
		m_renderPass = m_device.createRenderPass( formats
			, subpasses
			, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite
				, { renderer::ImageLayout::eColourAttachmentOptimal, renderer::ImageLayout::eDepthStencilAttachmentOptimal } }
			, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite
				, { renderer::ImageLayout::eColourAttachmentOptimal, renderer::ImageLayout::eDepthStencilAttachmentOptimal } } );
	}

	void RenderTarget::doCreateFrameBuffer()
	{
		m_colour = m_device.createTexture();
		m_colour->setImage( renderer::PixelFormat::eR8G8B8A8
			, m_size
			, renderer::ImageUsageFlag::eColourAttachment | renderer::ImageUsageFlag::eSampled );
		m_colourView = m_colour->createView( m_colour->getType()
			, m_colour->getFormat()
			, 0u
			, 1u
			, 0u
			, 1u );

		m_depth = m_device.createTexture();
		m_depth->setImage( DepthFormat
			, m_size
			, renderer::ImageUsageFlag::eDepthStencilAttachment );
		m_depthView = m_depth->createView( m_depth->getType()
			, m_depth->getFormat()
			, 0u
			, 1u
			, 0u
			, 1u );
		renderer::TextureAttachmentPtrArray attaches;
		attaches.emplace_back( std::make_unique< renderer::TextureAttachment >( *m_colourView ) );
		attaches.emplace_back( std::make_unique< renderer::TextureAttachment >( *m_depthView ) );
		m_frameBuffer = m_renderPass->createFrameBuffer( m_size
			, std::move( attaches ) );
	}

	void RenderTarget::doPrepareFrame()
	{
		resize( m_size );
		m_commandBuffer = m_device.getGraphicsCommandPool().createCommandBuffer();
		auto & commandBuffer = *m_commandBuffer;
		auto & frameBuffer = *m_frameBuffer;

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
		{
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::PipelineStageFlag::eColourAttachmentOutput
				, m_colourView->makeColourAttachment() );
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTopOfPipe
				, renderer::PipelineStageFlag::eEarlyFragmentTests
				, m_depthView->makeDepthStencilAttachment() );
			commandBuffer.beginRenderPass( *m_renderPass
				, frameBuffer
				, { renderer::ClearValue{ { 1.0f, 0.8f, 0.4f, 0.0f } }
					, renderer::ClearValue{ renderer::DepthStencilClearValue{ 1.0f, 0u } } }
				, renderer::SubpassContents::eInline );

			// Opaque pass

			// Transparent pass

			commandBuffer.endRenderPass();
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::PipelineStageFlag::eBottomOfPipe
				, m_colourView->makeShaderInputResource() );
			auto res = commandBuffer.end();

			if ( !res )
			{
				std::stringstream stream;
				stream << "Command buffers recording failed.";
				throw std::runtime_error{ stream.str() };
			}
		}
	}
}
