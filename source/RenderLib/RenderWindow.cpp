#include "RenderWindow.h"

#include "Scene.h"

#include <Renderer/DescriptorSetLayoutBinding.hpp>
#include <Renderer/Device.hpp>
#include <Renderer/FrameBuffer.hpp>
#include <Renderer/MultisampleState.hpp>
#include <Renderer/Queue.hpp>
#include <Renderer/RenderBuffer.hpp>
#include <Renderer/RenderingResources.hpp>
#include <Renderer/RenderPass.hpp>
#include <Renderer/RenderPassState.hpp>
#include <Renderer/RenderSubpass.hpp>
#include <Renderer/RenderSubpassState.hpp>
#include <Renderer/Sampler.hpp>
#include <Renderer/Texture.hpp>
#include <Renderer/VertexBuffer.hpp>

namespace render
{
	//*********************************************************************************************

	namespace
	{
		std::vector< RenderWindow::Vertex > const & doGetVtxData()
		{
			static std::vector< RenderWindow::Vertex > result
			{
				{
					{ { -1, -1 }, { 1, 0 } },
					{ { 1, -1 }, { 0, 0 } },
					{ { 1, 1 }, { 0, 1 } },
					{ { -1, 1 }, { 1, 1 } },
				}
			};
			return result;
		}

		renderer::DescriptorSetLayout doCreateDescriptorLayout( renderer::Device const & device )
		{
			std::vector< renderer::DescriptorSetLayoutBinding > bindings;

			bindings.emplace_back( UberShader::TextureDiffuseBinding
				, renderer::DescriptorType::eCombinedImageSampler
				, renderer::ShaderStageFlag::eFragment );

			return renderer::DescriptorSetLayout{ device, bindings };
		}

		std::vector< utils::PixelFormat > doGetPixelFormats()
		{
			return { utils::PixelFormat::eB8G8R8A8 };
		}

		renderer::RenderSubpassState doGetSubpassState()
		{
			return { renderer::PipelineStageFlag::eColourAttachmentOutput, renderer::AccessFlag::eColourAttachmentWrite };
		}

		renderer::RenderPassState doGetColourPassState()
		{
			return { renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite
				, { renderer::ImageLayout::eColourAttachmentOptimal } };
		}
	}

	//*********************************************************************************************

	RenderWindow::RenderWindow( renderer::Device const & device
		, utils::IVec2 const & dimensions
		, render::FontLoader & loader
		, bool debug )
		: m_device{ device }
		, m_swapChain{ device.createSwapChain( dimensions ) }
		, m_renderPass{ std::make_unique< renderer::RenderPass >( device
			, doGetPixelFormats()
			, renderer::RenderSubpassArray{ { device, doGetPixelFormats(), doGetSubpassState() } }
			, doGetColourPassState()
			, doGetColourPassState() ) }
		, m_frameBuffers{ m_swapChain->createFrameBuffers( *m_renderPass ) }
		, m_commandBuffers{ m_swapChain->createCommandBuffers() }
		, m_drawCommandPool{ std::make_unique< renderer::CommandPool >( device
			, device.getGraphicsQueue().getFamilyIndex()
			, renderer::CommandPoolCreateFlag::eResetCommandBuffer ) }
		, m_drawCommandBuffer{ std::make_unique < renderer::CommandBuffer > ( device
			, *m_drawCommandPool ) }
		, m_drawSemaphore{ std::make_unique< renderer::Semaphore >( device ) }
		, m_stagingBuffer{ std::make_unique< renderer::StagingBuffer >( device ) }
		, m_descriptorLayout{ doCreateDescriptorLayout( device ) }
		, m_pipelineLayout{ std::make_unique< renderer::PipelineLayout >( device, &m_descriptorLayout ) }
		, m_target{ std::make_unique< RenderTarget >( device, dimensions, utils::PixelFormat::eR8G8B8A8 ) }
		, m_overlayRenderer{ std::make_unique< OverlayRenderer >( device, m_target->getRenderPass() ) }
		, m_scene{ device, m_target->getRenderPass(), dimensions, *m_overlayRenderer }
		, m_size{ dimensions }
		, m_sampler{ std::make_shared< renderer::Sampler >( device
			, renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::Filter::eLinear
			, renderer::Filter::eLinear ) }
		, m_program{ UberShader::createShaderProgram( device
			, RenderType::eScene
			, TextureFlag::eNone
			, OpacityType::eOpaque
			, ObjectType::eTexture ) }
		, m_vbo{ renderer::makeVertexBuffer< RenderWindow::Vertex >( device
			, 0u
			, uint32_t( doGetVtxData().size() )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_layout{ std::make_unique< renderer::VertexLayout >( 0u ) }
		, m_descriptorPool{ std::make_unique< renderer::DescriptorSetPool >( m_descriptorLayout, 1u ) }
		, m_descriptor{ std::make_unique< renderer::DescriptorSet >( *m_descriptorPool ) }
		, m_viewport{ dimensions }
		//, m_picking{ device, dimensions }
		, m_debug{ device, *m_stagingBuffer, m_swapChain->getDefaultResources().getCommandBuffer(), debug, m_scene, loader }
	{
		m_layout->createAttribute< utils::Vec2 >( 0u, 0u );
		m_layout->createAttribute< utils::Vec2 >( 1u, sizeof( utils::Vec2 ) );

		m_descriptor->createBinding( m_descriptorPool->getLayout().getBinding( UberShader::TextureDiffuseBinding )
			, m_target->getTexture()
			, *m_sampler );
		m_descriptor->update();
		uint32_t index = 0u;

		m_pipeline = std::make_shared< renderer::Pipeline >( device
			, *m_pipelineLayout
			, *m_program
			, renderer::VertexLayoutCRefArray{ *m_layout }
			, *m_renderPass
			, renderer::PrimitiveTopology::eTriangleFan );
		m_pipeline->multisampleState( renderer::MultisampleState{} )
			.finish();

		for ( auto & commandBuffer : m_commandBuffers )
		{
			doPrepareCommandBuffer( index++
				, m_target->getTexture() );
		}
	}

	RenderWindow::~RenderWindow()
	{
		m_device.waitIdle();
	}

	void RenderWindow::beginFrame()
	{
		m_debug.start();

		if ( !m_vboInitialised )
		{
			m_stagingBuffer->copyVertexData( *m_drawCommandBuffer
				, doGetVtxData()
				, *m_vbo
				, renderer::PipelineStageFlag::eVertexInput );
			m_vboInitialised = true;
		}

		m_resources = m_swapChain->getResources();
	}

	void RenderWindow::update()
	{
		state().update();
		m_scene.camera().reorient( state().rotation() );
		m_scene.camera().fovY( state().zoom() );
		m_scene.update();
	}

	void RenderWindow::updateOverlays()
	{
		m_scene.updateOverlays();
	}

	void RenderWindow::updateUbos()
	{
		m_target->updateScene( *m_drawCommandBuffer
			, m_scene );
		m_target->updateOverlays( *m_drawCommandBuffer
			, *m_overlayRenderer );
	}

	void RenderWindow::draw()const
	{
#if !DEBUG_PICKING

		//if ( m_pick )
		//{
		//	m_picking.pick( m_device.getGraphicsQueue()
		//		, m_pickPosition
		//		, m_scene.camera()
		//		, state().zoomBounds().percent( state().zoom() )
		//		, m_scene.renderObjects()
		//		, m_scene.renderBillboards() );
		//	m_pick = false;
		//}

		m_drawCommandBuffer->begin();
		m_drawCommandBuffer->beginRenderPass( m_target->getRenderPass()
			, m_target->getFrameBuffer()
			, {
				renderer::ClearValue{ m_scene.backgroundColour() },
				renderer::ClearValue{ renderer::DepthStencilClearValue{ 1.0f, 0 } }
			} );
		m_target->render( *m_drawCommandBuffer, m_scene );

		m_overlayRenderer->beginRender( m_size );
		m_overlayRenderer->draw( *m_drawCommandBuffer
			, m_scene.overlays() );
		m_overlayRenderer->endRender();

		m_drawCommandBuffer->endRenderPass();
		m_drawCommandBuffer->end();

		m_device.getGraphicsQueue().submit( *m_drawCommandBuffer
			, m_resources->getImageAvailableSemaphore()
			, renderer::PipelineStageFlag::eColourAttachmentOutput
			, *m_drawSemaphore
			, nullptr );

#else

		m_picking.pick( m_device.getGraphicsQueue()
			, m_pickPosition
			, m_scene.camera()
			, state().zoomBounds().percent( state().zoom() )
			, m_scene.renderObjects()
			, m_scene.renderBillboards() );

#endif
	}

	void RenderWindow::endFrame()
	{
		if ( m_resources )
		{
			m_debug.count( m_scene.billboards()
				, m_scene.billboardsBuffers() );
			m_debug.end();

			m_device.getGraphicsQueue().submit( *m_commandBuffers[m_resources->getBackBuffer()]
				, *m_drawSemaphore
				, renderer::PipelineStageFlag::eColourAttachmentOutput
				, m_resources->getRenderingFinishedSemaphore()
				, &m_resources->getFence() );

			m_swapChain->present( *m_resources );
			m_device.getGraphicsQueue().waitIdle();
			m_resources = nullptr;
		}
	}

	void RenderWindow::resize( utils::IVec2 const & size )noexcept
	{
		m_size = size;
		m_viewport.resize( m_size );
		m_scene.resize( m_size );
	}

	void RenderWindow::doPrepareCommandBuffer( uint32_t index
		, renderer::Texture const & texture )const
	{
		auto & commandBuffer = *m_commandBuffers[index];
		auto & frameBuffer = *m_frameBuffers[index];

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
		{
			m_swapChain->preRenderCommands( index, commandBuffer );
			texture.bindAsShaderInput( commandBuffer
				, 0u );
			commandBuffer.beginRenderPass( *m_renderPass
				, frameBuffer
				, {
					renderer::ClearValue{ utils::RgbaColour{ 1.0, 1.0, 0.0, 1.0 } }
				} );
			commandBuffer.bindPipeline( *m_pipeline );
			commandBuffer.setViewport( m_viewport.viewport() );
			commandBuffer.setScissor( m_viewport.scissor() );
			commandBuffer.bindVertexBuffer( *m_vbo, 0u );
			commandBuffer.bindDescriptorSet( *m_descriptor, *m_pipelineLayout );
			commandBuffer.draw( m_vbo->getCount()
				, 1u
				, 0u
				, 0u );
			commandBuffer.endRenderPass();
			m_swapChain->postRenderCommands( index, commandBuffer );
			commandBuffer.end();
		}
	}
}
