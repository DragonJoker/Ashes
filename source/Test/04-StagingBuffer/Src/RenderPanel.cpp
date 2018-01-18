#include "RenderPanel.hpp"
#include "Application.hpp"

#include <Renderer/BackBuffer.hpp>
#include <Renderer/BufferMemoryBarrier.hpp>
#include <Renderer/CommandBuffer.hpp>
#include <Renderer/Connection.hpp>
#include <Renderer/Device.hpp>
#include <Renderer/GeometryBuffers.hpp>
#include <Renderer/MultisampleState.hpp>
#include <Renderer/Pipeline.hpp>
#include <Renderer/PipelineLayout.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/RenderingResources.hpp>
#include <Renderer/RenderPass.hpp>
#include <Renderer/RenderPassState.hpp>
#include <Renderer/RenderSubpass.hpp>
#include <Renderer/RenderSubpassState.hpp>
#include <Renderer/Scissor.hpp>
#include <Renderer/ShaderProgram.hpp>
#include <Renderer/SubpassContents.hpp>
#include <Renderer/SwapChain.hpp>
#include <Renderer/Texture.hpp>
#include <Renderer/VertexBuffer.hpp>
#include <Renderer/VertexLayout.hpp>
#include <Renderer/Viewport.hpp>

#include <FileUtils.hpp>

#include <fstream>
#include <cstring>

namespace vkapp
{
	namespace
	{
		enum class Ids
		{
			RenderTimer = 42
		}	Ids;

		static int const TimerTimeMs = 40;
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, renderer::Renderer const & renderer )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
		, m_timer{ new wxTimer{ this, int( Ids::RenderTimer ) } }
		, m_vertexData{
		{
			{
				{ -0.7f, -0.7f, 0.0f, 1.0f },
				{ 1.0f, 0.0f, 0.0f, 0.0f }
			},
			{
				{ -0.7f, 0.7f, 0.0f, 1.0f },
				{ 0.0f, 1.0f, 0.0f, 0.0f }
			},
			{
				{ 0.7f, -0.7f, 0.0f, 1.0f },
				{ 0.0f, 0.0f, 1.0f, 0.0f }
			},
			{
				{ 0.7f, 0.7f, 0.0f, 1.0f },
				{ 0.3f, 0.3f, 0.3f, 0.0f }
			}
		} }
	{
		try
		{
			doCreateDevice( renderer );
			std::cout << "Logical device created." << std::endl;
			doCreateSwapChain();
			std::cout << "Swapchain created." << std::endl;
			doCreateRenderPass();
			std::cout << "Render pass created." << std::endl;
			doCreateVertexBuffer();
			std::cout << "Vertex buffer created." << std::endl;
			doCreatePipeline();
			std::cout << "Pipeline created." << std::endl;
			doCreateStagingBuffer();
			std::cout << "Staging buffer created." << std::endl;
			doCopyVertexData();
			std::cout << "Vertex data copied." << std::endl;
			doPrepareFrames();
			std::cout << "Frames prepared." << std::endl;
		}
		catch ( std::exception & )
		{
			if ( m_device )
			{
				m_device->waitIdle();
			}

			m_device.reset();
			throw;
		}

		m_timer->Start( TimerTimeMs );

		Connect( int( Ids::RenderTimer ), wxEVT_TIMER, wxTimerEventHandler( RenderPanel::onTimer ), nullptr, this );
		Connect( wxID_ANY, wxEVT_SIZE, wxSizeEventHandler( RenderPanel::onSize ), nullptr, this );
	}

	RenderPanel::~RenderPanel()
	{
		delete m_timer;
		m_device->waitIdle();
		m_stagingBuffer.reset();
		m_pipeline.reset();
		m_vertexLayout.reset();
		m_program.reset();
		m_pipelineLayout.reset();
		m_geometryBuffers.reset();
		m_vertexBuffer.reset();
		m_commandBuffers.clear();
		m_frameBuffers.clear();
		m_renderPass.reset();
		m_swapChain.reset();
		m_device.reset();
	}

	void RenderPanel::doCreateDevice( renderer::Renderer const & renderer )
	{
		m_device = renderer.createDevice( common::makeConnection( this, renderer ) );
	}

	void RenderPanel::doCreateSwapChain()
	{
		wxSize size{ GetClientSize() };
		m_swapChain = m_device->createSwapChain( { size.x, size.y } );
		m_swapChain->setClearColour( renderer::RgbaColour{ 1.0f, 0.8f, 0.4f, 0.0f } );
		m_swapChainReset = m_swapChain->onReset.connect( [this]()
		{
			doResetSwapChain();
		} );
	}

	void RenderPanel::doCreateRenderPass()
	{
		std::vector< renderer::PixelFormat > formats{ { m_swapChain->getFormat() } };
		renderer::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( m_device->createRenderSubpass( formats
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
			, renderer::AccessFlag::eColourAttachmentWrite } ) );
		m_renderPass = m_device->createRenderPass( formats
			, std::move( subpasses )
			, renderer::RenderPassState{ renderer::PipelineStageFlag::eBottomOfPipe
			, renderer::AccessFlag::eMemoryRead
			, { renderer::ImageLayout::ePresentSrc } }
			, renderer::RenderPassState{ renderer::PipelineStageFlag::eBottomOfPipe
			, renderer::AccessFlag::eMemoryRead
			, { renderer::ImageLayout::ePresentSrc } } );
	}

	void RenderPanel::doCreateVertexBuffer()
	{
		m_vertexBuffer = renderer::makeVertexBuffer< VertexData >( *m_device
			, uint32_t( m_vertexData.size() )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_vertexLayout = m_device->createVertexLayout( 0u, sizeof( VertexData ) );
		m_vertexLayout->createAttribute( 0u
			, renderer::AttributeFormat::eVec4f
			, offsetof( VertexData, position ) );
		m_vertexLayout->createAttribute( 1u
			, renderer::AttributeFormat::eVec4f
			, offsetof( VertexData, colour ) );
	}

	void RenderPanel::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique< renderer::StagingBuffer >( *m_device
			, 0u
			, 4000u );
		renderer::ByteArray array{ reinterpret_cast< uint8_t * >( m_vertexData.data() )
			, reinterpret_cast< uint8_t * >( m_vertexData.data() ) + sizeof( m_vertexData ) };
		auto buffer = m_stagingBuffer->getBuffer().lock( 0
			, uint32_t( array.size() )
			, renderer::MemoryMapFlag::eWrite | renderer::MemoryMapFlag::eInvalidateBuffer );

		if ( !buffer )
		{
			throw std::runtime_error{ "Staging buffer storage memory mapping failed." };
		}

		std::memcpy( buffer, array.data(), array.size() );
		m_stagingBuffer->getBuffer().unlock( uint32_t( array.size() ), true );
	}

	void RenderPanel::doCopyVertexData()
	{
		auto & resources = m_swapChain->getDefaultResources();;
		auto & commandBuffer = resources.getCommandBuffer();

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			commandBuffer.copyBuffer( m_stagingBuffer->getBuffer()
				, m_vertexBuffer->getBuffer()
				, uint32_t( sizeof( m_vertexData ) ) );
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTransfer
				, renderer::PipelineStageFlag::eVertexInput
				, m_vertexBuffer->getBuffer().makeVertexShaderInputResource() );

			if ( !commandBuffer.end() )
			{
				throw std::runtime_error{ "Vertex data copy failed" };
			}

			auto res = m_device->getGraphicsQueue().submit( commandBuffer
				, nullptr );

			if ( !res )
			{
				throw std::runtime_error{ "Vertex data copy failed" };
			}

			m_device->waitIdle();
		}
	}

	void RenderPanel::doCreatePipeline()
	{
		m_pipelineLayout = m_device->createPipelineLayout();
		wxSize size{ GetClientSize() };
		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( wxGetApp().getRendererName() != wxT( "gl" ) )
		{
			if ( !wxFileExists( shadersFolder / "vert.spv" )
				|| !wxFileExists( shadersFolder / "frag.spv" ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			m_program = m_device->createShaderProgram();
			m_program->createModule( common::dumpBinaryFile( shadersFolder / "vert.spv" )
				, renderer::ShaderStageFlag::eVertex );
			m_program->createModule( common::dumpBinaryFile( shadersFolder / "frag.spv" )
				, renderer::ShaderStageFlag::eFragment );
			m_program->link();
		}
		else
		{
			if ( !wxFileExists( shadersFolder / "shader.vert" )
				|| !wxFileExists( shadersFolder / "shader.frag" ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			m_program = m_device->createShaderProgram();
			m_program->createModule( common::dumpTextFile( shadersFolder / "shader.vert" )
				, renderer::ShaderStageFlag::eVertex );
			m_program->createModule( common::dumpTextFile( shadersFolder / "shader.frag" )
				, renderer::ShaderStageFlag::eFragment );
			m_program->link();
		}

		m_pipeline = m_device->createPipeline( *m_pipelineLayout
			, *m_program
			, { *m_vertexLayout }
			, *m_renderPass
			, renderer::PrimitiveTopology::eTriangleStrip );
		m_pipeline->multisampleState( renderer::MultisampleState{} );
		m_pipeline->finish();
		m_geometryBuffers = m_device->createGeometryBuffers( { *m_vertexBuffer }
			, { 0ull }
		, { *m_vertexLayout } );
	}

	bool RenderPanel::doPrepareFrames()
	{
		bool result{ true };
		m_commandBuffers = m_swapChain->createCommandBuffers();
		m_frameBuffers = m_swapChain->createFrameBuffers( *m_renderPass );
		wxSize size{ GetClientSize() };

		for ( size_t i = 0u; i < m_commandBuffers.size() && result; ++i )
		{
			auto & frameBuffer = *m_frameBuffers[i];
			auto & commandBuffer = *m_commandBuffers[i];

			if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
			{
				commandBuffer.beginRenderPass( *m_renderPass
					, frameBuffer
					, { m_swapChain->getClearColour() }
					, renderer::SubpassContents::eInline );
				commandBuffer.bindPipeline( *m_pipeline );
				commandBuffer.setViewport( renderer::Viewport{ uint32_t( size.x ), uint32_t( size.y ), 0, 0 } );
				commandBuffer.setScissor( renderer::Scissor{ 0, 0, uint32_t( size.x ), uint32_t( size.y ) } );
				commandBuffer.bindGeometryBuffers( *m_geometryBuffers );
				commandBuffer.draw( 4u, 1u, 0u, 0u );
				commandBuffer.endRenderPass();

				result = commandBuffer.end();

				if ( !result )
				{
					std::cerr << "Command buffers recording failed" << std::endl;
				}
			}
		}

		return result;
	}

	void RenderPanel::doDraw()
	{
		auto resources = m_swapChain->getResources();

		if ( resources )
		{
			auto before = std::chrono::high_resolution_clock::now();
			auto & queue = m_device->getGraphicsQueue();
			auto res = queue.submit( *m_commandBuffers[resources->getBackBuffer()]
				, resources->getImageAvailableSemaphore()
				, renderer::PipelineStageFlag::eColourAttachmentOutput
				, resources->getRenderingFinishedSemaphore()
				, &resources->getFence() );
			m_swapChain->present( *resources );

			auto after = std::chrono::high_resolution_clock::now();
			wxGetApp().updateFps( std::chrono::duration_cast< std::chrono::microseconds >( after - before ) );
		}
		else
		{
			m_timer->Stop();
		}
	}

	void RenderPanel::doResetSwapChain()
	{
		doPrepareFrames();
	}

	void RenderPanel::onTimer( wxTimerEvent & event )
	{
		if ( event.GetId() == int( Ids::RenderTimer ) )
		{
			doDraw();
		}
	}

	void RenderPanel::onSize( wxSizeEvent & event )
	{
		m_timer->Stop();
		m_device->waitIdle();
		wxSize size{ GetClientSize() };
		m_swapChain->reset( { size.GetWidth(), size.GetHeight() } );
		m_timer->Start( TimerTimeMs );
		event.Skip();
	}
}
