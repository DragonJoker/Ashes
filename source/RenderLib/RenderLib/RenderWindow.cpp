#include "RenderWindow.h"

#include "Scene.h"

#include <Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Core/Device.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <Sync/ImageMemoryBarrier.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Command/Queue.hpp>
#include <RenderPass/RenderBuffer.hpp>
#include <Core/RenderingResources.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderPassState.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <Image/Sampler.hpp>
#include <Image/Texture.hpp>
#include <Buffer/VertexBuffer.hpp>

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
					{ { -1, -1 }, { 0, 1 } },
					{ { 1, -1 }, { 1, 1 } },
					{ { 1, 1 }, { 1, 0 } },
					{ { -1, 1 }, { 0, 0 } },
				}
			};
			return result;
		}

		renderer::DescriptorSetLayoutPtr doCreateDescriptorLayout( renderer::Device const & device )
		{
			std::vector< renderer::DescriptorSetLayoutBinding > bindings;

			bindings.emplace_back( UberShader::TextureDiffuseBinding
				, renderer::DescriptorType::eCombinedImageSampler
				, renderer::ShaderStageFlag::eFragment );

			return device.createDescriptorSetLayout( std::move( bindings ) );
		}

		std::vector< renderer::PixelFormat > doGetPixelFormats()
		{
			return { renderer::PixelFormat::eB8G8R8A8 };
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

		renderer::RenderSubpassPtrArray doGetRenderSubpasses( renderer::Device const & device )
		{
			renderer::RenderSubpassPtrArray result;
			result.emplace_back( device.createRenderSubpass( doGetPixelFormats(), doGetSubpassState() ) );
			return result;
		}
	}

	//*********************************************************************************************

	RenderWindow::RenderWindow( renderer::Device const & device
		, renderer::IVec2 const & dimensions
		, render::FontLoader & loader
		, bool debug )
		: m_device{ device }
		, m_swapChain{ device.createSwapChain( dimensions ) }
		, m_renderPass{ device.createRenderPass( doGetPixelFormats()
			, doGetRenderSubpasses( device )
			, doGetColourPassState()
			, doGetColourPassState() ) }
		, m_frameBuffers{ m_swapChain->createFrameBuffers( *m_renderPass ) }
		, m_commandBuffers{ m_swapChain->createCommandBuffers() }
		, m_drawCommandPool{ device.createCommandPool( device.getGraphicsQueue().getFamilyIndex()
			, renderer::CommandPoolCreateFlag::eResetCommandBuffer ) }
		, m_drawCommandBuffer{ m_drawCommandPool->createCommandBuffer() }
		, m_drawSemaphore{ device.createSemaphore() }
		, m_stagingBuffer{ std::make_unique< renderer::StagingBuffer >( device
			, 0u
			, 10000000u ) }
		, m_descriptorLayout{ doCreateDescriptorLayout( device ) }
		, m_pipelineLayout{ device.createPipelineLayout( *m_descriptorLayout ) }
		, m_target{ std::make_unique< RenderTarget >( device, dimensions, renderer::PixelFormat::eR8G8B8A8 ) }
		, m_overlayRenderer{ device, m_target->getRenderPass(), *m_drawCommandPool, dimensions }
		, m_scene{ device, m_target->getRenderPass(), dimensions, m_overlayRenderer }
		, m_size{ dimensions }
		, m_sampler{ device.createSampler( renderer::WrapMode::eClampToEdge
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
			, uint32_t( doGetVtxData().size() )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_layout{ renderer::makeLayout< RenderWindow::Vertex >( device, 0u ) }
		, m_vao{ device.createGeometryBuffers( *m_vbo, 0ull, *m_layout ) }
		, m_descriptorPool{ m_descriptorLayout->createPool( 1u ) }
		, m_descriptor{ m_descriptorPool->createDescriptorSet() }
		, m_viewport{ dimensions }
		//, m_picking{ device, dimensions }
		, m_debug{ device, *m_stagingBuffer, m_swapChain->getDefaultResources().getCommandBuffer(), debug, m_scene, loader }
	{
		m_layout->createAttribute< renderer::Vec2 >( 0u
			, uint32_t( offsetof( RenderWindow::Vertex, position ) ) );
		m_layout->createAttribute< renderer::Vec2 >( 1u
			, uint32_t( offsetof( RenderWindow::Vertex, texture ) ) );

		m_descriptor->createBinding( m_descriptorPool->getLayout().getBinding( UberShader::TextureDiffuseBinding )
			, m_target->getTexture().getView()
			, *m_sampler );
		m_descriptor->update();
		uint32_t index = 0u;

		m_pipeline = device.createPipeline( *m_pipelineLayout
			, *m_program
			, { *m_layout }
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
		m_overlayRenderer.update();
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
			}
		, renderer::SubpassContents::eSecondaryCommandBuffers );
		m_target->render( m_scene
			, m_overlayRenderer );
		m_drawCommandBuffer->executeCommands( {
			m_scene.commandBuffer(),
			m_overlayRenderer.commandBuffer()
		} );
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

	void RenderWindow::resize( renderer::IVec2 const & size )noexcept
	{
		m_size = size;
		m_viewport.resize( m_size );
		m_scene.resize( m_size );
		m_overlayRenderer.resize( m_size );
	}

	void RenderWindow::doPrepareCommandBuffer( uint32_t index
		, renderer::Texture const & texture )const
	{
		auto & commandBuffer = *m_commandBuffers[index];
		auto & frameBuffer = *m_frameBuffers[index];

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
		{
			m_swapChain->preRenderCommands( index, commandBuffer );
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTopOfPipe
				, renderer::PipelineStageFlag::eFragmentShader
				, texture.makeShaderInputResource() );
			commandBuffer.beginRenderPass( *m_renderPass
				, frameBuffer
				, {
					renderer::ClearValue{ renderer::RgbaColour{ 1.0, 1.0, 0.0, 1.0 } }
				}
				, renderer::SubpassContents::eInline );
			commandBuffer.bindPipeline( *m_pipeline );
			commandBuffer.setViewport( m_viewport.viewport() );
			commandBuffer.setScissor( m_viewport.scissor() );
			commandBuffer.bindGeometryBuffers( *m_vao );
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
