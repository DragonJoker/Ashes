#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <Buffer/GeometryBuffers.hpp>
#include <Buffer/StagingBuffer.hpp>
#include <Buffer/UniformBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <Command/Queue.hpp>
#include <Core/BackBuffer.hpp>
#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Core/Renderer.hpp>
#include <Core/SwapChain.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/Texture.hpp>
#include <Image/TextureView.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderPassState.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <Utils/Transform.hpp>

#include "FileUtils.hpp"

#include <chrono>
#include <numeric>

namespace common
{
	namespace
	{
		wxString getName( wxString const & name
			, wxString const & rendererName )
		{
			auto result = name + wxT( " (" ) + rendererName + wxT( ")" );
#if !defined( NDEBUG )
			result += wxT( " - Debug" );
#endif
			return result;
		}
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, std::string const & appName
		, std::string const & appDesc )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
		, m_appName{ appName }
		, m_appDesc{ appDesc }
		, m_vertexData
		{
			{ { -1.0, -1.0, 0.0, 1.0 }, { 0.0, 0.0 } },
			{ { -1.0, +1.0, 0.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, -1.0, 0.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, +1.0, 0.0, 1.0 }, { 1.0, 1.0 } },
		}
	{
	}

	RenderPanel::~RenderPanel()
	{
		doCleanup();
	}
	
	void RenderPanel::initialise( renderer::Renderer const & renderer )
	{
		wxSize size = GetClientSize();
		try
		{
			doCreateDevice( renderer );
			std::cout << "Logical device created." << std::endl;
			doCreateSwapChain();
			std::cout << "Swap chain created." << std::endl;
			m_stagingBuffer = std::make_unique< renderer::StagingBuffer >( *m_device
				, 0u
				, 1024u * 64u );
			doInitialise( *m_device
				, { size.GetWidth(), size.GetHeight() } );
			m_gui = std::make_unique< Gui >( *m_device
				, renderer::UIVec2{ size.GetWidth(), size.GetHeight() } );
			m_gui->updateView( m_renderTarget->getColourView() );
			m_sampler = m_device->createSampler( renderer::WrapMode::eClampToEdge
				, renderer::WrapMode::eClampToEdge
				, renderer::WrapMode::eClampToEdge
				, renderer::Filter::eLinear
				, renderer::Filter::eLinear );
			doCreateDescriptorSet();
			std::cout << "Main descriptor set created." << std::endl;
			doCreateRenderPass();
			std::cout << "Main render pass created." << std::endl;
			doCreateVertexBuffer();
			std::cout << "Main vertex buffer created." << std::endl;
			doCreatePipeline();
			std::cout << "Main pipeline created." << std::endl;
			doPrepareFrames();
			std::cout << "Main frames prepared." << std::endl;
			m_ready = true;
		}
		catch ( std::exception & )
		{
			doCleanup();
			throw;
		}

		Connect( wxID_ANY
			, wxEVT_SIZE
			, wxSizeEventHandler( RenderPanel::onSize )
			, nullptr
			, this );
		Connect( GetId()
			, wxEVT_LEFT_DOWN
			, wxMouseEventHandler( RenderPanel::onMouseLDown )
			, nullptr
			, this );
		Connect( GetId()
			, wxEVT_LEFT_UP
			, wxMouseEventHandler( RenderPanel::onMouseLUp )
			, nullptr
			, this );
		Connect( GetId()
			, wxEVT_LEFT_DCLICK
			, wxMouseEventHandler( RenderPanel::onMouseLDClick )
			, nullptr
			, this );
		Connect( GetId()
			, wxEVT_RIGHT_DOWN
			, wxMouseEventHandler( RenderPanel::onMouseRDown )
			, nullptr
			, this );
		Connect( GetId()
			, wxEVT_RIGHT_UP
			, wxMouseEventHandler( RenderPanel::onMouseRUp )
			, nullptr
			, this );
		Connect( GetId()
			, wxEVT_RIGHT_DCLICK
			, wxMouseEventHandler( RenderPanel::onMouseRDClick )
			, nullptr
			, this );
		Connect( GetId()
			, wxEVT_MOTION
			, wxMouseEventHandler( RenderPanel::onMouseMove )
			, nullptr
			, this );
	}

	void RenderPanel::update()
	{
		static renderer::Clock::time_point save = renderer::Clock::now();
		auto duration = std::chrono::duration_cast< std::chrono::microseconds >( renderer::Clock::now() - save );
		doUpdate();
		m_renderTarget->update( duration );
		doUpdateGui( duration );
		save = renderer::Clock::now();
	}

	void RenderPanel::draw()
	{
		if ( m_ready )
		{
			auto result = m_renderTarget->draw( m_frameTime );
			++m_frameCount;
			m_framesTimes[m_frameIndex] = m_frameTime;
			m_frameIndex = ++m_frameIndex % FrameSamplesCount;

			if ( !result )
			{
				throw std::runtime_error{ "Couldn't render offscreen frame." };
			}

			m_gui->submit( m_device->getGraphicsQueue() );

			auto resources = m_swapChain->getResources();

			if ( !resources )
			{
				throw std::runtime_error{ "Couldn't acquire next frame from swap chain." };
			}

			result = m_device->getGraphicsQueue().submit( *m_commandBuffers[resources->getBackBuffer()]
				, resources->getImageAvailableSemaphore()
				, renderer::PipelineStageFlag::eColourAttachmentOutput
				, resources->getRenderingFinishedSemaphore()
				, &resources->getFence() );

			if ( !result )
			{
				throw std::runtime_error{ "Couldn't render main frame." };
			}

			m_swapChain->present( *resources );
		}
	}

	void RenderPanel::doCleanup()
	{
		m_ready = false;

		if ( m_device )
		{
			m_device->waitIdle();

			m_gui.reset();

			m_renderTarget.reset();
			m_commandBuffers.clear();
			m_frameBuffers.clear();

			m_descriptorSet.reset();
			m_descriptorPool.reset();
			m_descriptorLayout.reset();
			m_pipeline.reset();
			m_pipelineLayout.reset();

			m_program.reset();
			m_vertexBuffer.reset();
			m_geometryBuffers.reset();
			m_vertexLayout.reset();
			m_renderPass.reset();
			m_sampler.reset();
			m_stagingBuffer.reset();

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
		m_swapChain = m_device->createSwapChain( { size.x, size.y } );
		m_swapChain->setClearColour( { 1.0f, 0.8f, 0.4f, 0.0f } );
		m_swapChainReset = m_swapChain->onReset.connect( [this]()
		{
			m_renderTarget->resize( m_swapChain->getDimensions() );
			m_gui->updateView( m_renderTarget->getColourView() );
			doCreateDescriptorSet();
			doPrepareFrames();
		} );
	}
	
	void RenderPanel::doCreateDescriptorSet()
	{
		std::vector< renderer::DescriptorSetLayoutBinding > bindings
		{
			renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
			renderer::DescriptorSetLayoutBinding{ 1u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
		};
		m_descriptorLayout = m_device->createDescriptorSetLayout( std::move( bindings ) );
		m_descriptorPool = m_descriptorLayout->createPool( 1u );
		m_descriptorSet = m_descriptorPool->createDescriptorSet();
		m_descriptorSet->createBinding( m_descriptorLayout->getBinding( 0u )
			, m_renderTarget->getColourView()
			, *m_sampler );
		m_descriptorSet->createBinding( m_descriptorLayout->getBinding( 1u )
			, m_gui->getTargetView()
			, *m_sampler );
		m_descriptorSet->update();
	}

	void RenderPanel::doCreateRenderPass()
	{
		std::vector< renderer::PixelFormat > formats{ { m_swapChain->getFormat() } };
		renderer::RenderPassAttachmentArray attaches{ { m_swapChain->getFormat(), true } };
		renderer::ImageLayoutArray const initialLayouts
		{
			renderer::ImageLayout::eColourAttachmentOptimal,
		};
		renderer::ImageLayoutArray const finalLayouts
		{
			renderer::ImageLayout::eColourAttachmentOptimal,
		};
		renderer::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( m_device->createRenderSubpass( formats
			, { renderer::PipelineStageFlag::eColourAttachmentOutput, renderer::AccessFlag::eColourAttachmentWrite } ) );
		m_renderPass = m_device->createRenderPass( attaches
			, subpasses
			, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite
				, initialLayouts }
			, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite
				, finalLayouts } );
	}

	void RenderPanel::doCreateVertexBuffer()
	{
		m_vertexLayout = renderer::makeLayout< TexturedVertexData >( *m_device, 0 );
		m_vertexLayout->createAttribute< renderer::Vec4 >( 0u
			, uint32_t( offsetof( TexturedVertexData, position ) ) );
		m_vertexLayout->createAttribute< renderer::Vec2 >( 1u
			, uint32_t( offsetof( TexturedVertexData, uv ) ) );

		m_vertexBuffer = renderer::makeVertexBuffer< TexturedVertexData >( *m_device
			, uint32_t( m_vertexData.size() )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_stagingBuffer->uploadVertexData( m_swapChain->getDefaultResources().getCommandBuffer()
			, m_vertexData
			, *m_vertexBuffer
			, renderer::PipelineStageFlag::eVertexInput );

		m_geometryBuffers = m_device->createGeometryBuffers( *m_vertexBuffer
			, 0u
			, *m_vertexLayout );
	}

	void RenderPanel::doCreatePipeline()
	{
		wxSize size{ GetClientSize() };
		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / "Sample-00-Common" / "Shaders";
		m_program = m_device->createShaderProgram();

		if ( !wxFileExists( shadersFolder / "main.vert" )
			|| !wxFileExists( shadersFolder / "main.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		m_program->createModule( common::dumpTextFile( shadersFolder / "main.vert" )
			, renderer::ShaderStageFlag::eVertex );
		m_program->createModule( common::dumpTextFile( shadersFolder / "main.frag" )
			, renderer::ShaderStageFlag::eFragment );

		m_pipelineLayout = m_device->createPipelineLayout( *m_descriptorLayout );
		m_pipeline = m_pipelineLayout->createPipeline( *m_program
			, { *m_vertexLayout }
			, *m_renderPass
			, { renderer::PrimitiveTopology::eTriangleStrip } );
		m_pipeline->multisampleState( renderer::MultisampleState{} );
		m_pipeline->finish();
	}

	void RenderPanel::doPrepareFrames()
	{
		m_frameBuffers = m_swapChain->createFrameBuffers( *m_renderPass );
		m_commandBuffers = m_swapChain->createCommandBuffers();
		static renderer::ClearValue const clearValue{ { 1.0, 0.0, 0.0, 1.0 } };

		for ( size_t i = 0u; i < m_frameBuffers.size(); ++i )
		{
			auto & frameBuffer = *m_frameBuffers[i];
			auto & commandBuffer = *m_commandBuffers[i];

			wxSize size{ GetClientSize() };

			if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
			{
				auto dimensions = m_swapChain->getDimensions();
				m_swapChain->preRenderCommands( i, commandBuffer );
				commandBuffer.beginRenderPass( *m_renderPass
					, frameBuffer
					, { clearValue }
					, renderer::SubpassContents::eInline );
				commandBuffer.bindPipeline( *m_pipeline );
				commandBuffer.setViewport( { uint32_t( dimensions.x )
					, uint32_t( dimensions.y )
					, 0
					, 0 } );
				commandBuffer.setScissor( { 0
					, 0
					, uint32_t( dimensions.x )
					, uint32_t( dimensions.y ) } );
				commandBuffer.bindGeometryBuffers( *m_geometryBuffers );
				commandBuffer.bindDescriptorSet( *m_descriptorSet
					, *m_pipelineLayout );
				commandBuffer.draw( 4u );
				commandBuffer.endRenderPass();
				m_swapChain->postRenderCommands( i, commandBuffer );

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

	void RenderPanel::doUpdateGui( std::chrono::microseconds const & duration )
	{
		auto size = GetClientSize();
		ImGuiIO & io = ImGui::GetIO();

		io.DisplaySize = ImVec2( float( size.GetWidth() ), float( size.GetHeight() ) );
		io.DeltaTime = std::chrono::duration_cast< std::chrono::milliseconds >( duration ).count() / 1000.0f;

		io.MousePos = ImVec2( m_mouse.position[0], m_mouse.position[1] );
		io.MouseDown[0] = m_mouse.left;
		io.MouseDown[1] = m_mouse.right;

		ImGui::NewFrame();

		ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 10 );
		ImGui::SetNextWindowPos( ImVec2( 10, 10 ) );
		ImGui::SetNextWindowSize( ImVec2( 0, 0 ), ImGuiSetCond_FirstUseEver );
		ImGui::Begin( "RendererLib Sample", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );
		ImGui::TextUnformatted( getName( m_appDesc, m_device->getRenderer().getName() ).c_str() );
		//ImGui::TextUnformatted( m_device->getRendererName() );

		auto count = std::min( m_frameCount, m_framesTimes.size() );

		if ( count )
		{
			auto instmillis = m_frameTime.count() / 1000.0f;
			std::sort( m_framesTimes.begin()
				, m_framesTimes.begin() + count );
			auto minGpuTime = *m_framesTimes.begin();
			auto maxGpuTime = *( m_framesTimes.begin() + count - 1 );
			auto averageGpuTime = std::accumulate( m_framesTimes.begin()
				, m_framesTimes.begin() + count
				, std::chrono::microseconds{ 0 } ) / count;
			auto avrgmillis = averageGpuTime.count() / 1000.0f;
			ImGui::Text( "Instant: %.2f ms/frame (%5d fps)", instmillis, int( 1000.0f / instmillis ) );
			ImGui::Text( "Average: %.2f ms/frame (%5d fps)", avrgmillis, int( 1000.0f / avrgmillis ) );
			ImGui::Text( "Min: %.2f ms, Max %.2f ms", ( minGpuTime.count() / 1000.0f ), ( maxGpuTime.count() / 1000.0f ) );
		}

#if RENDERLIB_ANDROID
		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0.0f, 5.0f * UIOverlay->scale ) );
#endif

		ImGui::PushItemWidth( 110.0f );
		doUpdateOverlays( *m_gui );
		ImGui::PopItemWidth();

#if RENDERLIB_ANDROID
		ImGui::PopStyleVar();
#endif

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::Render();

		m_gui->update();

#if RENDERLIB_ANDROID
		m_mouse.left = false;
#endif
	}

	void RenderPanel::onSize( wxSizeEvent & event )
	{
		m_ready = false;
		m_device->waitIdle();
		m_swapChain->reset( { event.GetSize().GetWidth(), event.GetSize().GetHeight() } );
		m_ready = true;
		event.Skip();
	}

	void RenderPanel::onMouseLDown( wxMouseEvent & event )
	{
		m_mouse.left = true;
		m_mouse.position[0] = event.GetPosition().x;
		m_mouse.position[1] = event.GetPosition().y;
	}

	void RenderPanel::onMouseLUp( wxMouseEvent & event )
	{
		m_mouse.left = false;
		m_mouse.position[0] = event.GetPosition().x;
		m_mouse.position[1] = event.GetPosition().y;
	}

	void RenderPanel::onMouseLDClick( wxMouseEvent & event )
	{
		m_mouse.left = true;
		m_mouse.position[0] = event.GetPosition().x;
		m_mouse.position[1] = event.GetPosition().y;
	}

	void RenderPanel::onMouseRDown( wxMouseEvent & event )
	{
		m_mouse.right = true;
		m_mouse.position[0] = event.GetPosition().x;
		m_mouse.position[1] = event.GetPosition().y;
	}

	void RenderPanel::onMouseRUp( wxMouseEvent & event )
	{
		m_mouse.right = false;
		m_mouse.position[0] = event.GetPosition().x;
		m_mouse.position[1] = event.GetPosition().y;
	}

	void RenderPanel::onMouseRDClick( wxMouseEvent & event )
	{
		m_mouse.right = true;
		m_mouse.position[0] = event.GetPosition().x;
		m_mouse.position[1] = event.GetPosition().y;
	}

	void RenderPanel::onMouseMove( wxMouseEvent & event )
	{
		m_mouse.position[0] = event.GetPosition().x;
		m_mouse.position[1] = event.GetPosition().y;
	}
}
