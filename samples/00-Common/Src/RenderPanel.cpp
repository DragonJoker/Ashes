#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <ashespp/Buffer/StagingBuffer.hpp>
#include <ashespp/Buffer/UniformBuffer.hpp>
#include <ashespp/Buffer/VertexBuffer.hpp>
#include <ashespp/Core/Surface.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Core/Instance.hpp>
#include <ashespp/Core/SwapChain.hpp>
#include <ashespp/Descriptor/DescriptorSet.hpp>
#include <ashespp/Descriptor/DescriptorSetLayout.hpp>
#include <ashespp/Descriptor/DescriptorSetPool.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/RenderPass/FrameBuffer.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>
#include <ashespp/Sync/Queue.hpp>

#include <util/GlslToSpv.hpp>
#include <util/Transform.hpp>

#include "FileUtils.hpp"

#include <algorithm>
#include <chrono>
#include <numeric>

#ifdef min
#	undef min
#	undef max
#	undef abs
#endif

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

	void RenderPanel::initialise( utils::Instance const & instance )
	{
		wxSize size = GetClientSize();
		try
		{
			auto surface = doCreateSurface( instance );
			std::cout << "Surface created." << std::endl;
			doCreateDevice( instance, *surface );
			std::cout << "Logical device created." << std::endl;
			doCreateSwapChain( std::move( surface ) );
			std::cout << "Swap chain created." << std::endl;
			m_stagingBuffer = std::make_unique< ashes::StagingBuffer >( m_device->getDevice()
				, 0u
				, 1024u * 64u );
			doInitialise( *m_device
				, { uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ) } );
			m_gui = std::make_unique< Gui >( *m_device
				, *m_graphicsQueue
				, *m_commandPool
				, VkExtent2D{ uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ) } );
			m_gui->updateView( m_renderTarget->getColourView() );
			m_sampler = m_device->getDevice().createSampler( VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
				, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
				, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
				, VK_FILTER_LINEAR
				, VK_FILTER_LINEAR );
			doCreatePipelineLayout();
			std::cout << "Main pipeline layout created." << std::endl;
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
		static utils::Clock::time_point save = utils::Clock::now();
		auto duration = std::chrono::duration_cast< std::chrono::microseconds >( utils::Clock::now() - save );
		doUpdate();
		m_renderTarget->update( duration );
		doUpdateGui( duration );
		save = utils::Clock::now();
	}

	void RenderPanel::draw()
	{
		if ( m_ready )
		{
			m_renderTarget->draw( *m_graphicsQueue, m_frameTime );
			++m_frameCount;
			m_framesTimes[m_frameIndex] = m_frameTime;
			m_frameIndex = ++m_frameIndex % FrameSamplesCount;

			m_gui->submit( *m_graphicsQueue );

			auto resources = m_swapChain->getResources();

			if ( !resources )
			{
				throw std::runtime_error{ "Couldn't acquire next frame from swap chain." };
			}

			m_graphicsQueue->submit( *m_commandBuffers[resources->getImageIndex()]
				, resources->getImageAvailableSemaphore()
				, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, resources->getRenderingFinishedSemaphore()
				, &resources->getFence() );
			m_swapChain->present( *resources, *m_presentQueue );
		}
	}

	void RenderPanel::doCleanup()
	{
		m_ready = false;

		if ( m_device )
		{
			m_device->getDevice().waitIdle();

			m_gui.reset();

			m_renderTarget.reset();
			m_commandBuffers.clear();
			m_frameBuffers.clear();

			m_descriptorSet.reset();
			m_descriptorPool.reset();
			m_descriptorLayout.reset();
			m_pipeline.reset();
			m_pipelineLayout.reset();

			m_vertexBuffer.reset();
			m_renderPass.reset();
			m_sampler.reset();
			m_stagingBuffer.reset();

			m_swapChain.reset();
			m_commandPool.reset();
			m_presentQueue.reset();
			m_graphicsQueue.reset();
			m_device.reset();
		}
	}

	ashes::SurfacePtr RenderPanel::doCreateSurface( utils::Instance const & instance )
	{
		auto handle = common::makeWindowHandle( *this );
		auto & gpu = instance.getPhysicalDevice( 0u );
		return instance.getInstance().createSurface( gpu
			, std::move( handle ) );
	}

	void RenderPanel::doCreateDevice( utils::Instance const & instance
		, ashes::Surface const & surface )
	{
		m_device = std::make_unique< utils::Device >( instance.getInstance()
			, surface );
		m_graphicsQueue = m_device->getDevice().getQueue( m_device->getGraphicsQueueFamily(), 0u );
		m_presentQueue = m_device->getDevice().getQueue( m_device->getPresentQueueFamily(), 0u );
		m_commandPool = m_device->getDevice().createCommandPool( m_device->getGraphicsQueueFamily()
			, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT );
	}

	void RenderPanel::doCreateSwapChain( ashes::SurfacePtr surface )
	{
		wxSize size{ GetClientSize() };
		m_swapChain = std::make_unique< utils::SwapChain >( m_device->getDevice()
			, *m_commandPool
			, std::move( surface )
			, VkExtent2D{ uint32_t( size.x ), uint32_t( size.y ) } );
		m_clearColour = { 1.0f, 0.8f, 0.4f, 0.0f };
		m_swapChainReset = m_swapChain->onReset.connect( [this]()
		{
			m_renderTarget->resize( m_swapChain->getDimensions() );
			m_gui->updateView( m_renderTarget->getColourView() );
			doCreateDescriptorSet();
			doPrepareFrames();
		} );
	}

	void RenderPanel::doCreatePipelineLayout()
	{
		ashes::VkDescriptorSetLayoutBindingArray bindings
		{
			{ 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			{ 1u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
		};
		m_descriptorLayout = m_device->getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_pipelineLayout = m_device->getDevice().createPipelineLayout( *m_descriptorLayout );
	}

	void RenderPanel::doCreateDescriptorSet()
	{
		m_descriptorSet.reset();
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
		ashes::VkAttachmentDescriptionArray attaches
		{
			{
				0u,
				m_swapChain->getFormat(),
				VK_SAMPLE_COUNT_1_BIT,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_STORE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			}
		};
		ashes::SubpassDescriptionArray subpasses;
		subpasses.emplace_back( ashes::SubpassDescription
			{
				0u,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				{},
				{ { 0u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } },
				{},
				ashes::nullopt,
				{},
			} );
		ashes::VkSubpassDependencyArray dependencies
		{
			{
				VK_SUBPASS_EXTERNAL,
				0u,
				VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
				VK_DEPENDENCY_BY_REGION_BIT,
			},
			{
				0u,
				VK_SUBPASS_EXTERNAL,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_DEPENDENCY_BY_REGION_BIT,
			}
		};
		ashes::RenderPassCreateInfo createInfo
		{
			0u,
			std::move( attaches ),
			std::move( subpasses ),
			std::move( dependencies ),
		};
		m_renderPass = m_device->getDevice().createRenderPass( std::move( createInfo ) );
	}

	void RenderPanel::doCreateVertexBuffer()
	{
		m_vertexBuffer = utils::makeVertexBuffer< TexturedVertexData >( *m_device
			, uint32_t( m_vertexData.size() )
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_stagingBuffer->uploadVertexData( *m_graphicsQueue
			, *m_commandPool
			, m_vertexData
			, *m_vertexBuffer );
	}

	void RenderPanel::doCreatePipeline()
	{
		wxSize size{ GetClientSize() };
		std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / "Sample-00-Common" / "Shaders";

		if ( !wxFileExists( shadersFolder / "main.vert" )
			|| !wxFileExists( shadersFolder / "main.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}
		
		ashes::PipelineVertexInputStateCreateInfo vertexLayout
		{
			0u,
			{
				{ 0u, sizeof( TexturedVertexData ), VK_VERTEX_INPUT_RATE_VERTEX },
			},
			{
				{ 0u, 0u, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof( TexturedVertexData, position ) },
				{ 1u, 0u, VK_FORMAT_R32G32_SFLOAT, offsetof( TexturedVertexData, uv ) },
			},
		};
		
		ashes::PipelineShaderStageCreateInfoArray shaderStages;
		shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
			{
				0u,
				VK_SHADER_STAGE_VERTEX_BIT,
				m_device->getDevice().createShaderModule( dumpShaderFile( *m_device
					, VK_SHADER_STAGE_VERTEX_BIT
					, shadersFolder / "main.vert" ) ),
				"main",
				ashes::nullopt,
			} );
		shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
			{
				0u,
				VK_SHADER_STAGE_FRAGMENT_BIT,
				m_device->getDevice().createShaderModule( dumpShaderFile( *m_device
					, VK_SHADER_STAGE_FRAGMENT_BIT
					, shadersFolder / "main.frag" ) ),
				"main",
				ashes::nullopt,
			} );

		std::vector< VkDynamicState > dynamicStateEnables
		{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		m_pipeline = m_device->getDevice().createPipeline( ashes::GraphicsPipelineCreateInfo
			{
				0u,
				std::move( shaderStages ),
				std::move( vertexLayout ),
				ashes::PipelineInputAssemblyStateCreateInfo{ 0u, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP },
				ashes::nullopt,
				ashes::PipelineViewportStateCreateInfo{},
				ashes::PipelineRasterizationStateCreateInfo{ 0u, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE },
				ashes::PipelineMultisampleStateCreateInfo{},
				ashes::nullopt,
				ashes::PipelineColorBlendStateCreateInfo{},
				ashes::PipelineDynamicStateCreateInfo{ 0u, { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR } },
				*m_pipelineLayout,
				*m_renderPass,
			} );
	}

	void RenderPanel::doPrepareFrames()
	{
		m_frameBuffers = m_swapChain->createFrameBuffers( *m_renderPass );
		m_commandBuffers = m_swapChain->createCommandBuffers();
		static VkClearValue const clearValue{ { 1.0, 0.0, 0.0, 1.0 } };

		for ( size_t i = 0u; i < m_frameBuffers.size(); ++i )
		{
			auto & frameBuffer = *m_frameBuffers[i];
			auto & commandBuffer = *m_commandBuffers[i];

			wxSize size{ GetClientSize() };

			commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT );
			auto dimensions = m_swapChain->getDimensions();
			commandBuffer.beginRenderPass( *m_renderPass
				, frameBuffer
				, { clearValue }
				, VK_SUBPASS_CONTENTS_INLINE );
			commandBuffer.bindPipeline( *m_pipeline );
			commandBuffer.setViewport( { 0.0f, 0.0f, float( dimensions.width ), float( dimensions.height ), 0.0f, 1.0f } );
			commandBuffer.setScissor( { { 0, 0 }, { dimensions.width, dimensions.height } } );
			commandBuffer.bindVertexBuffer( 0u, m_vertexBuffer->getBuffer(), 0u );
			commandBuffer.bindDescriptorSet( *m_descriptorSet
				, *m_pipelineLayout );
			commandBuffer.draw( 4u );
			commandBuffer.endRenderPass();
			commandBuffer.end();
		}
	}

	void RenderPanel::doUpdateGui( std::chrono::microseconds const & duration )
	{
		auto size = GetClientSize();
		ImGuiIO & io = ImGui::GetIO();

		io.DisplaySize = ImVec2( float( size.GetWidth() ), float( size.GetHeight() ) );
		io.DeltaTime = std::chrono::duration_cast< std::chrono::milliseconds >( duration ).count() / 1000.0f;

		io.MousePos = ImVec2( m_mouse.position.x, m_mouse.position.y );
		io.MouseDown[0] = m_mouse.left;
		io.MouseDown[1] = m_mouse.right;

		ImGui::NewFrame();

		ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 10 );
		ImGui::SetNextWindowPos( ImVec2( 10, 10 ) );
		ImGui::SetNextWindowSize( ImVec2( 0, 0 ), ImGuiCond_FirstUseEver );
		ImGui::Begin( "Ashes Sample", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );
		ImGui::TextUnformatted( getName( m_appDesc, m_device->getDevice().getInstance().getName() ).c_str() );
		ImGui::TextUnformatted( m_device->getDevice().getProperties().deviceName );

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

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0.0f, 5.0f * UIOverlay->scale ) );
#endif

		ImGui::PushItemWidth( 110.0f );
		doUpdateOverlays( *m_gui );
		ImGui::PopItemWidth();

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
		ImGui::PopStyleVar();
#endif

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::Render();

		m_gui->update();

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
		m_mouse.left = false;
#endif
	}

	void RenderPanel::onSize( wxSizeEvent & event )
	{
		m_ready = false;
		m_device->getDevice().waitIdle();
		m_swapChain->reset( { uint32_t( event.GetSize().GetWidth() ), uint32_t( event.GetSize().GetHeight() ) } );
		m_ready = true;
		event.Skip();
	}

	void RenderPanel::onMouseLDown( wxMouseEvent & event )
	{
		m_mouse.left = true;
		m_mouse.position.x = event.GetPosition().x;
		m_mouse.position.y = event.GetPosition().y;
	}

	void RenderPanel::onMouseLUp( wxMouseEvent & event )
	{
		m_mouse.left = false;
		m_mouse.position.x = event.GetPosition().x;
		m_mouse.position.y = event.GetPosition().y;
	}

	void RenderPanel::onMouseLDClick( wxMouseEvent & event )
	{
		m_mouse.left = true;
		m_mouse.position.x = event.GetPosition().x;
		m_mouse.position.y = event.GetPosition().y;
	}

	void RenderPanel::onMouseRDown( wxMouseEvent & event )
	{
		m_mouse.right = true;
		m_mouse.position.x = event.GetPosition().x;
		m_mouse.position.y = event.GetPosition().y;
	}

	void RenderPanel::onMouseRUp( wxMouseEvent & event )
	{
		m_mouse.right = false;
		m_mouse.position.x = event.GetPosition().x;
		m_mouse.position.y = event.GetPosition().y;
	}

	void RenderPanel::onMouseRDClick( wxMouseEvent & event )
	{
		m_mouse.right = true;
		m_mouse.position.x = event.GetPosition().x;
		m_mouse.position.y = event.GetPosition().y;
	}

	void RenderPanel::onMouseMove( wxMouseEvent & event )
	{
		m_mouse.position.x = event.GetPosition().x;
		m_mouse.position.y = event.GetPosition().y;
	}
}
