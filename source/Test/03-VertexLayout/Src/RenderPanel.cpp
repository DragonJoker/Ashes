#include "RenderPanel.hpp"
#include "Application.hpp"

#include <Buffer/VertexBuffer.hpp>
#include <Command/CommandBuffer.hpp>
#include <Core/BackBuffer.hpp>
#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Core/Renderer.hpp>
#include <Core/RenderingResources.hpp>
#include <Core/SwapChain.hpp>
#include <Image/Texture.hpp>
#include <Miscellaneous/QueryPool.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/ShaderStageState.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <Shader/ShaderProgram.hpp>

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
			doCleanup();
			throw;
		}

		m_timer->Start( TimerTimeMs );

		Connect( int( Ids::RenderTimer ), wxEVT_TIMER, wxTimerEventHandler( RenderPanel::onTimer ), nullptr, this );
		Connect( wxID_ANY, wxEVT_SIZE, wxSizeEventHandler( RenderPanel::onSize ), nullptr, this );
	}

	RenderPanel::~RenderPanel()
	{
		doCleanup();
	}

	void RenderPanel::doCleanup()
	{
		delete m_timer;

		if ( m_device )
		{
			m_device->waitIdle();
			m_queryPool.reset();
			m_pipeline.reset();
			m_pipelineLayout.reset();
			m_commandBuffers.clear();
			m_frameBuffers.clear();
			m_vertexBuffer.reset();
			m_renderPass.reset();
			m_swapChain.reset();
			m_device->disable();
			m_device.reset();
		}
	}

	void RenderPanel::doCreateDevice( renderer::Renderer const & renderer )
	{
		m_device = renderer.createDevice( common::makeConnection( this, renderer ) );
		m_device->enable();
	}

	void RenderPanel::doCreateSwapChain()
	{
		wxSize size{ GetClientSize() };
		m_swapChain = m_device->createSwapChain( { uint32_t( size.x ), uint32_t( size.y ) } );
		m_swapChain->setClearColour( renderer::ClearColorValue{ 1.0f, 0.8f, 0.4f, 0.0f } );
		m_swapChainReset = m_swapChain->onReset.connect( [this]()
		{
			doResetSwapChain();
		} );
	}

	void RenderPanel::doCreateRenderPass()
	{
		renderer::AttachmentDescriptionArray attaches
		{
			{
				m_swapChain->getFormat(),
				renderer::SampleCountFlag::e1,
				renderer::AttachmentLoadOp::eClear,
				renderer::AttachmentStoreOp::eStore,
				renderer::AttachmentLoadOp::eDontCare,
				renderer::AttachmentStoreOp::eDontCare,
				renderer::ImageLayout::eUndefined,
				renderer::ImageLayout::ePresentSrc,
			}
		};
		renderer::AttachmentReferenceArray subAttaches
		{
			{ 0u, renderer::ImageLayout::eColourAttachmentOptimal }
		};
		renderer::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( std::make_unique< renderer::RenderSubpass >( renderer::PipelineBindPoint::eGraphics
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite }
			, subAttaches ) );
		m_renderPass = m_device->createRenderPass( attaches
			, std::move( subpasses )
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eBottomOfPipe
				, renderer::AccessFlag::eMemoryRead }
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eBottomOfPipe
				, renderer::AccessFlag::eMemoryRead } );
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
		m_vertexLayout = renderer::makeLayout< VertexData >( 0u );
		m_vertexLayout->createAttribute( 0u
			, renderer::Format::eR32G32B32A32_SFLOAT
			, offsetof( VertexData, position ) );
		m_vertexLayout->createAttribute( 1u
			, renderer::Format::eR32G32B32A32_SFLOAT
			, offsetof( VertexData, colour ) );

		if ( auto * buffer = m_vertexBuffer->lock( 0u
			, uint32_t( data.size() )
			, renderer::MemoryMapFlag::eWrite | renderer::MemoryMapFlag::eInvalidateBuffer ) )
		{
			std::copy( data.begin()
				, data.end()
				, buffer );
			m_vertexBuffer->flush( 0u, uint32_t( data.size() ) );
			m_vertexBuffer->unlock();
		}
	}

	void RenderPanel::doCreatePipeline()
	{
		m_pipelineLayout = m_device->createPipelineLayout();
		wxSize size{ GetClientSize() };
		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "shader.vert" )
			|| !wxFileExists( shadersFolder / "shader.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		std::vector< renderer::ShaderStageState > shaderStages;
		shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "shader.vert" ) );
		shaderStages[1].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "shader.frag" ) );

		m_pipeline = m_pipelineLayout->createPipeline( renderer::GraphicsPipelineCreateInfo
		{
			std::move( shaderStages ),
			*m_renderPass,
			renderer::VertexInputState::create( *m_vertexLayout ),
			renderer::InputAssemblyState{ renderer::PrimitiveTopology::eTriangleStrip },
			renderer::RasterisationState{},
			renderer::MultisampleState{},
			renderer::ColourBlendState::createDefault(),
			{ renderer::DynamicState::eViewport, renderer::DynamicState::eScissor }
		} );
	}

	bool RenderPanel::doPrepareFrames()
	{
		bool result{ true };
		m_queryPool = m_device->createQueryPool( renderer::QueryType::eTimestamp
			, 2u
			, 0u );
		m_commandBuffers = m_swapChain->createCommandBuffers();
		m_frameBuffers = m_swapChain->createFrameBuffers( *m_renderPass );
		wxSize size{ GetClientSize() };

		for ( size_t i = 0u; i < m_commandBuffers.size() && result; ++i )
		{
			auto & frameBuffer = *m_frameBuffers[i];
			auto & commandBuffer = *m_commandBuffers[i];

			commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse );
			commandBuffer.resetQueryPool( *m_queryPool
				, 0u
				, 2u );
			commandBuffer.beginRenderPass( *m_renderPass
				, frameBuffer
				, { m_swapChain->getClearColour() }
				, renderer::SubpassContents::eInline );
			commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eTopOfPipe
				, *m_queryPool
				, 0u );
			commandBuffer.bindPipeline( *m_pipeline );
			commandBuffer.setViewport( renderer::Viewport{ uint32_t( size.x ), uint32_t( size.y ), 0, 0 } );
			commandBuffer.setScissor( renderer::Scissor{ 0, 0, uint32_t( size.x ), uint32_t( size.y ) } );
			commandBuffer.bindVertexBuffer( 0u, m_vertexBuffer->getBuffer(), 0u );
			commandBuffer.draw( 4u );
			commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eBottomOfPipe
				, *m_queryPool
				, 1u );
			commandBuffer.endRenderPass();

			commandBuffer.end();
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
			queue.submit( *m_commandBuffers[resources->getBackBuffer()]
				, resources->getImageAvailableSemaphore()
				, renderer::PipelineStageFlag::eColourAttachmentOutput
				, resources->getRenderingFinishedSemaphore()
				, &resources->getFence() );
			m_swapChain->present( *resources );

			renderer::UInt32Array values{ 0u, 0u };
			m_queryPool->getResults( 0u
				, 2u
				, 0u
				, renderer::QueryResultFlag::eWait
				, values );
			// Elapsed time in nanoseconds
			auto elapsed = std::chrono::nanoseconds{ uint64_t( ( values[1] - values[0] ) / float( m_device->getTimestampPeriod() ) ) };
			auto after = std::chrono::high_resolution_clock::now();
			wxGetApp().updateFps( std::chrono::duration_cast< std::chrono::microseconds >( elapsed )
				, std::chrono::duration_cast< std::chrono::microseconds >( after - before ) );
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
		m_swapChain->reset( { uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ) } );
		m_timer->Start( TimerTimeMs );
		event.Skip();
	}
}
