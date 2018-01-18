#include "RenderPanel.hpp"
#include "Application.hpp"

#include <Renderer/Core/BackBuffer.hpp>
#include <Renderer/Command/CommandBuffer.hpp>
#include <Renderer/Core/Connection.hpp>
#include <Renderer/Core/Device.hpp>
#include <Renderer/Buffer/GeometryBuffers.hpp>
#include <Renderer/Pipeline/MultisampleState.hpp>
#include <Renderer/Pipeline/Pipeline.hpp>
#include <Renderer/Pipeline/PipelineLayout.hpp>
#include <Renderer/Core/Renderer.hpp>
#include <Renderer/Core/RenderingResources.hpp>
#include <Renderer/RenderPass/RenderPass.hpp>
#include <Renderer/RenderPass/RenderPassState.hpp>
#include <Renderer/RenderPass/RenderSubpass.hpp>
#include <Renderer/RenderPass/RenderSubpassState.hpp>
#include <Renderer/Pipeline/Scissor.hpp>
#include <Renderer/Shader/ShaderProgram.hpp>
#include <Renderer/Enum/SubpassContents.hpp>
#include <Renderer/Core/SwapChain.hpp>
#include <Renderer/Image/Texture.hpp>
#include <Renderer/Buffer/VertexBuffer.hpp>
#include <Renderer/Pipeline/VertexLayout.hpp>
#include <Renderer/Pipeline/Viewport.hpp>

#include <FileUtils.hpp>

#include <fstream>
#include <cstring>

namespace vkapp
{
	namespace
	{
		enum class Ids
		{
			This = 41,
			RenderTimer = 42
		};

		static int const TimerTimeMs = 40;
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, renderer::Renderer const & renderer )
		: wxPanel{ parent, int( Ids::This ), wxDefaultPosition, size }
		, m_timer{ new wxTimer{ this, int( Ids::RenderTimer ) } }
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
		m_geometryBuffers.reset();
		m_pipeline.reset();
		m_pipelineLayout.reset();
		m_program.reset();
		m_commandBuffers.clear();
		m_frameBuffers.clear();
		m_vertexBuffer.reset();
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
		std::array< VertexData, 4u > data
		{
			VertexData
			{
				{ -0.7f, -0.7f, 0.0f, 1.0f },
				{ 1.0f, 0.0f, 0.0f, 0.0f }
			},
			VertexData
			{
				{ -0.7f, 0.7f, 0.0f, 1.0f },
				{ 0.0f, 1.0f, 0.0f, 0.0f }
			},
			VertexData
			{
				{ 0.7f, -0.7f, 0.0f, 1.0f },
				{ 0.0f, 0.0f, 1.0f, 0.0f }
			},
			VertexData
			{
				{ 0.7f, 0.7f, 0.0f, 1.0f },
				{ 0.3f, 0.3f, 0.3f, 0.0f }
			}
		};
		m_vertexBuffer = renderer::makeVertexBuffer< VertexData >( *m_device
			, uint32_t( data.size() )
			, 0u
			, renderer::MemoryPropertyFlag::eHostVisible );
		m_vertexLayout = m_device->createVertexLayout( 0u, sizeof( VertexData ) );
		m_vertexLayout->createAttribute( 0u
			, renderer::AttributeFormat::eVec4f
			, offsetof( VertexData, position ) );
		m_vertexLayout->createAttribute( 1u
			, renderer::AttributeFormat::eVec4f
			, offsetof( VertexData, colour ) );
		VertexData * buffer = m_vertexBuffer->lock( 0u
			, uint32_t( data.size() )
			, renderer::MemoryMapFlag::eWrite | renderer::MemoryMapFlag::eInvalidateBuffer );

		if ( buffer )
		{
			std::copy( data.begin()
				, data.end()
				, buffer );
			m_vertexBuffer->unlock( uint32_t( data.size() ), true );
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
			std::cerr << "Can't render" << std::endl;
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
		wxSize size = GetClientSize();
		m_swapChain->reset( { size.x, size.y } );
		m_timer->Start( TimerTimeMs );
		event.Skip();
	}
}
