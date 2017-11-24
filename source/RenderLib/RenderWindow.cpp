#include "RenderWindow.h"

#include "Scene.h"

#include <Renderer/DescriptorSetLayoutBinding.hpp>
#include <Renderer/FrameBuffer.hpp>
//#include <Renderer/Queue.hpp>
#include <Renderer/RenderBuffer.hpp>
#include <Renderer/RenderingResources.hpp>
//#include <Renderer/RenderPass.hpp>
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
			uint32_t index = 0u;
			std::vector< renderer::DescriptorSetLayoutBinding > bindings;

			bindings.emplace_back( index++
				, renderer::DescriptorType::eSampledImage
				, renderer::ShaderStageFlag::eFragment );

			return renderer::DescriptorSetLayout{ device, bindings };
		}
	}

	//*********************************************************************************************

	RenderWindow::RenderWindow( renderer::Device const & device
		, utils::IVec2 const & dimensions
		, render::FontLoader & loader
		, bool debug )
		: m_device{ device }
		, m_swapChain{ device.createSwapChain( dimensions ) }
		, m_stagingBuffer{ device }
		, m_descriptorLayout{ doCreateDescriptorLayout( device ) }
		, m_pipelineLayout{ device, &m_descriptorLayout }
		, m_target{ dimensions, utils::PixelFormat::eR8G8B8 }
		, m_scene{ device, dimensions }
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
		, m_descriptorPool{ m_descriptorLayout }
		, m_descriptor{ m_descriptorPool }
		, m_viewport{ dimensions }
		, m_overlayRenderer{ std::make_unique< OverlayRenderer >( device ) }
		, m_picking{ device, dimensions }
		, m_debug{ device, m_stagingBuffer, m_swapChain->getDefaultResources().getCommandBuffer(), debug, m_scene, loader }
	{
		//m_pipeline = std::make_shared< renderer::Pipeline >( device
		//	, m_pipelineLayout
		//	, *m_program
		//	, { m_posLayout, m_nmlLayout, m_texLayout }
		//	, vk::RenderPass{ resources.getDevice()
		//		, { VK_FORMAT_R8G8B8A8_UNORM }
		//		, {}
		//		, vk::RenderPassState{}
		//		, vk::RenderPassState{}
		//		, true
		//		, VK_SAMPLE_COUNT_32_BIT }
		//	, renderer::PrimitiveTopology::eTriangleFan );

		m_layout->createAttribute< utils::Vec2 >( 0u, 0u );
		m_layout->createAttribute< utils::Vec2 >( 1u, sizeof( utils::Vec2 ) );

		m_descriptor.createBinding( renderer::DescriptorSetLayoutBinding{ 0u
				, renderer::DescriptorType::eCombinedImageSampler
				, renderer::ShaderStageFlag::eFragment }
			, m_target.texture()
			, *m_sampler );
	}

	void RenderWindow::beginFrame()
	{
		m_debug.start();
		m_resources = m_swapChain->getResources();

		if ( m_resources && !m_vboInitialised )
		{
			m_stagingBuffer.copyVertexData( m_resources->getCommandBuffer()
				, doGetVtxData()
				, *m_vbo
				, renderer::PipelineStageFlag::eVertexInput );
			m_vboInitialised = true;
		}
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

	void RenderWindow::draw()const noexcept
	{
		if ( m_resources )
		{
#if !DEBUG_PICKING
			if ( m_pick )
			{
				m_picking.pick( *m_resources
					, m_pickPosition
					, m_scene.camera()
					, state().zoomBounds().percent( state().zoom() )
					, m_scene.renderObjects()
					, m_scene.renderBillboards() );
				m_pick = false;
			}

			m_target.drawScene( *m_resources, m_scene );
			doRenderTextureToScreen( *m_resources, m_target.texture() );
#else
			m_picking.pick( m_pickPosition
				, m_scene.camera()
				, state().zoomBounds().percent( state().zoom() )
				, m_scene.renderObjects()
				, m_scene.renderBillboards() );
			doRenderTextureToScreen( *m_resources, m_picking.texture() );
#endif

			m_overlayRenderer->beginRender( m_size );
			m_overlayRenderer->draw( m_stagingBuffer
				, m_resources->getCommandBuffer()
				, m_scene.overlays() );
			m_overlayRenderer->endRender();
		}
	}

	void RenderWindow::endFrame()
	{
		if ( m_resources )
		{
			m_debug.count( m_scene.billboards()
				, m_scene.billboardsBuffers() );
			m_debug.end();
			m_swapChain->present( *m_resources );
			m_resources = nullptr;
		}
	}

	void RenderWindow::resize( utils::IVec2 const & size )noexcept
	{
		m_size = size;
		m_viewport.resize( m_size );
		m_scene.resize( m_size );
	}

	void RenderWindow::doRenderTextureToScreen( renderer::RenderingResources const & resources
		, renderer::Texture const & texture )const noexcept
	{
		auto & commandBuffer = resources.getCommandBuffer();
		texture.bindAsShaderInput( commandBuffer
			, 0u );

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			commandBuffer.bindPipeline( *m_pipeline );
			commandBuffer.clear( texture, renderer::RgbaColour{ 0.0, 0.0, 0.0, 1.0 } );
			commandBuffer.bindVertexBuffer( *m_vbo, 0u );
			commandBuffer.bindDescriptorSet( m_descriptor, m_pipelineLayout );
			commandBuffer.draw( m_vbo->getCount()
				, 1u
				, 0u
				, 0u );

			//auto res = vk::checkError( m_resources.getDevice().getGraphicsQueue().submit( { commandBuffer }
			//	, {}
			//	, {}
			//	, {}
			//, nullptr ) );

			//if ( !res )
			//{
			//	throw std::runtime_error{ "Window quad rendering binding failed: " + vk::getLastError() };
			//}
		}
	}
}
