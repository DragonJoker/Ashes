#include "RenderPanel.hpp"
#include "Application.hpp"
#include "Gui.hpp"

#include <ashespp/Buffer/VertexBuffer.hpp>
#include <ashespp/Command/CommandBuffer.hpp>
#include <ashespp/Core/Surface.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Core/Instance.hpp>
#include <ashespp/Core/SwapChain.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Miscellaneous/QueryPool.hpp>
#include <ashespp/Pipeline/GraphicsPipeline.hpp>
#include <ashespp/Pipeline/PipelineLayout.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>
#include <util/GlslToSpv.hpp>

#include <FileUtils.hpp>

#include <numeric>
#include <fstream>
#include <cstring>

namespace vkapp
{
	namespace
	{
		enum class Ids
		{
			RenderTimer = 42
		};

		int const TimerTimeMs = 40;
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, utils::Instance const & instance )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
		, m_timer{ this, int( Ids::RenderTimer ) }
	{
		try
		{
			auto surface = doCreateSurface( instance );
			std::cout << "Surface created." << std::endl;
			doCreateDevice( instance, *surface );
			std::cout << "Logical device created." << std::endl;
			doCreateSwapChain( std::move( surface ) );
			std::cout << "Swapchain created." << std::endl;
			doCreateStagingBuffer();
			std::cout << "Staging buffer created." << std::endl;
			doCreateDescriptorSet();
			std::cout << "Descriptor set created." << std::endl;
			doCreateTexture();
			std::cout << "Image created." << std::endl;
			doCreateRenderPass();
			std::cout << "Render pass created." << std::endl;
			doCreateVertexBuffer();
			std::cout << "Vertex buffer created." << std::endl;
			doCreatePipeline();
			std::cout << "Pipeline created." << std::endl;
			m_gui = std::make_unique< Gui >( *m_device
				, *m_graphicsQueue
				, *m_commandPool
				, VkExtent2D{ uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ) } );
			m_gui->initialise( *m_renderPass );
			doPrepareFrames();
			std::cout << "Frames prepared." << std::endl;
			doUpdateGui();
		}
		catch ( common::Exception & )
		{
			doCleanup();
			throw;
		}

		m_timer.Start( TimerTimeMs );

		Connect( int( Ids::RenderTimer )
			, wxEVT_TIMER
			, wxTimerEventHandler( RenderPanel::onTimer )
			, nullptr
			, this );
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
		Connect( GetId()
			, wxEVT_KEY_UP
			, wxKeyEventHandler( RenderPanel::onKeyUp )
			, nullptr
			, this );
	}

	RenderPanel::~RenderPanel()noexcept
	{
		doCleanup();
	}

	void RenderPanel::doCleanup()noexcept
	{
		if ( m_device )
		{
			m_device->getDevice().waitIdle();
			m_gui.reset();
			m_queryPool.reset();
			m_descriptorSets.clear();
			m_descriptorPool.reset();
			m_descriptorLayout.reset();
			m_sampler.reset();
			m_dstTextures.clear();
			m_srcTexture.reset();
			m_stagingBuffer.reset();
			m_pipeline.reset();
			m_pipelineLayout.reset();
			m_vertexBuffer.reset();
			m_commandBuffers.clear();
			m_frameBuffers.clear();
			m_renderPass.reset();
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
		auto const & gpu = instance.getPhysicalDevice( 0u );
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
		m_clearColour = VkClearColorValue{ 1.0f, 0.8f, 0.4f, 1.0f };
		m_swapChainReset = m_swapChain->onReset.connect( [this]()
		{
			doResetSwapChain();
		} );
	}

	void RenderPanel::doCreateTexture()
	{
		std::string assetsFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / "Assets";
		auto image = common::loadImage( assetsFolder / "02-left.png" );
		m_srcTexture = m_device->createImage( ashes::ImageCreateInfo{ 0u
				, VK_IMAGE_TYPE_2D
				, image.format
				, { image.size.width, image.size.height, 1u }
				, 1u
				, 1u
				, VK_SAMPLE_COUNT_1_BIT
				, VK_IMAGE_TILING_OPTIMAL
				, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT }
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			, "Src" + ashes::getName( image.format ) );
		m_srcView = m_srcTexture->createView( "Src"
			, VK_IMAGE_VIEW_TYPE_2D
			, image.format );
		m_sampler = m_device->getDevice().createSampler( VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_FILTER_LINEAR
			, VK_FILTER_LINEAR );

		// Upload image from RAM to VRAM
		m_stagingBuffer->uploadTextureData( *m_graphicsQueue
			, *m_commandPool
			, image.format
			, image.data
			, m_srcView
			, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL );

		for ( int32_t fmt = ashes::beginFmt() + 1u; fmt <= ashes::endFmt(); ++fmt )
		{
			auto props = m_device->getPhysicalDevice().getFormatProperties( VkFormat( fmt ) );

			if ( ashes::checkFlag( props.optimalTilingFeatures, VK_FORMAT_FEATURE_BLIT_DST_BIT ) )
			{
				VkImageFormatProperties imageProperties{};

				if ( VK_SUCCESS == m_device->getPhysicalDevice().getImageFormatProperties( VkFormat( fmt )
					, VK_IMAGE_TYPE_2D
					, VK_IMAGE_TILING_OPTIMAL
					, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
					, 0u
					, imageProperties ) )
				{
					m_choices.push_back( ashes::getName( VkFormat( fmt ) ) );
					m_choicesIndex.push_back( VkFormat( fmt ) );
				}
			}
		}

		// Prepare first texture.
		m_curIndex = 0u;
		doCreateTextureDst( m_curIndex );
	}

	void RenderPanel::doCreateTextureDst( size_t index )
	{
		auto format = m_choicesIndex[index];

		if ( auto it = m_dstTextures.find( format );
			it == m_dstTextures.end() )
		{
			auto size = m_srcTexture->getDimensions();
			m_dstTextures[format] = m_device->createImage( ashes::ImageCreateInfo{ 0u
				, VK_IMAGE_TYPE_2D
				, format
				, { size.width, size.height, 1u }
				, 1u
				, 1u
				, VK_SAMPLE_COUNT_1_BIT
				, VK_IMAGE_TILING_OPTIMAL
				, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT }
				, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
				, "Dst" + ashes::getName( format ) );
			m_dstViews[format] = m_dstTextures[format]->createView( "Dst"
				, VK_IMAGE_VIEW_TYPE_2D
				, format );

			auto commandBuffer = m_commandPool->createCommandBuffer( "BlitTex"
				, VK_COMMAND_BUFFER_LEVEL_PRIMARY );
			commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );

			// Copy parts of src image to dst image
			commandBuffer->memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, VK_PIPELINE_STAGE_TRANSFER_BIT
				, m_dstViews[format].makeTransferDestination( VK_IMAGE_LAYOUT_UNDEFINED ) );
			// First clear it
			commandBuffer->clear( m_dstViews[format], m_clearColour );

			// Then copy some bits of src image
			auto copySize = int32_t( size.width / 4u );
			VkImageBlit blit{ { VK_IMAGE_ASPECT_COLOR_BIT, 0u, 0u, 1u }
				, { { 0, 0, 0 }, { int32_t( size.width ), int32_t( size.height ), 1u } }
				, { VK_IMAGE_ASPECT_COLOR_BIT, 0u, 0u, 1u }
				, { { 0, int32_t( size.height - copySize ), 0 }, { copySize, int32_t( size.height ), 1u } } };
			ashes::VkImageBlitArray blits;

			for ( auto i = 0; i < 4; ++i )
			{
				blits.push_back( blit );
				blit.dstOffsets[0].x += copySize;
				blit.dstOffsets[1].x += copySize;
				blit.dstOffsets[0].y -= copySize;
				blit.dstOffsets[1].y -= copySize;
			}

			commandBuffer->blitImage( *m_srcTexture
				, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
				, *m_dstTextures[format]
				, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
				, blits
				, VK_FILTER_LINEAR );
			commandBuffer->memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
				, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
				, m_dstViews[format].makeShaderInputResource( VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL ) );
			commandBuffer->end();
			auto fence = m_device->getDevice().createFence();
			m_graphicsQueue->submit( *commandBuffer
				, fence.get() );
			fence->wait( ashes::MaxTimeout );

			m_descriptorSets[format] = m_descriptorPool->createDescriptorSet();
			m_descriptorSets[format]->createBinding( m_descriptorLayout->getBinding( 0u )
				, m_dstViews[format]
				, *m_sampler );
			m_descriptorSets[format]->update();
		}

		m_curIndex = index;
	}

	void RenderPanel::doCreateDescriptorSet()
	{
		ashes::VkDescriptorSetLayoutBindingArray bindings
		{
			{ 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }
		};
		m_descriptorLayout = m_device->getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_descriptorPool = m_descriptorLayout->createPool( uint32_t( ashes::endFmt() ) + 1 );
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

	void RenderPanel::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique< ashes::StagingBuffer >( m_device->getDevice()
			, 0u
			, 10000000u );
	}

	void RenderPanel::doCreatePipeline()
	{
		m_pipelineLayout = m_device->getDevice().createPipelineLayout( *m_descriptorLayout );
		std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "shader.vert" )
			|| !wxFileExists( shadersFolder / "shader.frag" ) )
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
		shaderStages.emplace_back( 0u
				, VK_SHADER_STAGE_VERTEX_BIT
				, m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
					, VK_SHADER_STAGE_VERTEX_BIT
					, shadersFolder / "shader.vert" ) )
				, "main"
				, ashes::nullopt );
		shaderStages.emplace_back( 0u
				, VK_SHADER_STAGE_FRAGMENT_BIT
				, m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
					, VK_SHADER_STAGE_FRAGMENT_BIT
					, shadersFolder / "shader.frag" ) )
				, "main"
				, ashes::nullopt );

		m_pipeline = m_device->getDevice().createPipeline( ashes::GraphicsPipelineCreateInfo
			{
				0u,
				std::move( shaderStages ),
				std::move( vertexLayout ),
				ashes::PipelineInputAssemblyStateCreateInfo{ 0u, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP },
				ashes::nullopt,
				ashes::PipelineViewportStateCreateInfo{},
				ashes::PipelineRasterizationStateCreateInfo{},
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
		m_queryPool = m_device->getDevice().createQueryPool( VK_QUERY_TYPE_TIMESTAMP
			, 2u
			, 0u );
		m_frameBuffers = m_swapChain->createFrameBuffers( *m_renderPass );
		doPrepareCommandBuffers();
	}

	void RenderPanel::doPrepareCommandBuffers()
	{
		auto dimensions = m_swapChain->getDimensions();
		m_commandBuffers = m_swapChain->createCommandBuffers();

		for ( size_t i = 0u; i < m_commandBuffers.size(); ++i )
		{
			auto const & frameBuffer = *m_frameBuffers[i];
			auto const & commandBuffer = *m_commandBuffers[i];

			commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT );
			m_gui->preUpdateCommandBuffer( commandBuffer );
			commandBuffer.resetQueryPool( *m_queryPool
				, 0u
				, 2u );
			commandBuffer.beginRenderPass( *m_renderPass
				, frameBuffer
				, { ashes::makeClearValue( m_clearColour ) }
				, VK_SUBPASS_CONTENTS_INLINE );
			commandBuffer.writeTimestamp( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, *m_queryPool
				, 0u );
			commandBuffer.bindPipeline( *m_pipeline );
			commandBuffer.setViewport( { 0.0f, 0.0f, float( dimensions.width ), float( dimensions.height ), 0.0f, 1.0f } );
			commandBuffer.setScissor( { { 0, 0 }, { dimensions.width, dimensions.height } } );
			commandBuffer.bindVertexBuffer( 0u, m_vertexBuffer->getBuffer(), 0u );
			commandBuffer.bindDescriptorSet( *m_descriptorSets[m_choicesIndex[m_curIndex]]
				, *m_pipelineLayout );
			commandBuffer.draw( 4u );
			m_gui->updateCommandBuffer( commandBuffer );
			commandBuffer.writeTimestamp( VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
				, *m_queryPool
				, 1u );
			commandBuffer.endRenderPass();

			m_gui->postUpdateCommandBuffer( commandBuffer );
			commandBuffer.end();
		}
	}

	void RenderPanel::doUpdateGui()
	{
		static utils::Clock::time_point save = utils::Clock::now();
		m_frameTime = std::chrono::duration_cast< std::chrono::microseconds >( utils::Clock::now() - save );

		auto size = GetClientSize();
		ImGuiIO & io = ImGui::GetIO();

		io.DisplaySize = ImVec2( float( size.GetWidth() ), float( size.GetHeight() ) );
		io.DeltaTime = float( m_frameTime.count() ) / 1000000.0f;

		io.MousePos = ImVec2( float( m_mouse.position.x ), float( m_mouse.position.y ) );
		io.MouseDown[0] = m_mouse.left;
		io.MouseDown[1] = m_mouse.right;

		ImGui::NewFrame();

		ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 10 );
		ImGui::SetNextWindowPos( ImVec2( 10, 10 ) );
		ImGui::SetNextWindowSize( ImVec2( 0, 0 ), ImGuiCond_FirstUseEver );
		ImGui::Begin( AppName.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0.0f, 5.0f * UIOverlay->scale ) );
#endif

		ImGui::PushItemWidth( 400.0f );
		auto index = int32_t( m_curIndex );
		m_gui->comboBox( "Format", &index, m_choices );
		ImGui::PopItemWidth();

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
		ImGui::PopStyleVar();
#endif

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::Render();

#if defined( VK_USE_PLATFORM_ANDROID_KHR )
		m_mouse.left = false;
#endif
		save = utils::Clock::now();
		bool invalid = false;

		if ( index != m_curIndex )
		{
			doCreateTextureDst( size_t( index ) );
			invalid = true;
		}

		invalid = m_gui->update() || invalid;

		if ( invalid )
		{
			m_device->getDevice().waitIdle();
			doPrepareCommandBuffers();
		}
	}

	void RenderPanel::doDraw()
	{
		doUpdateGui();
		auto resources = m_swapChain->getResources();

		if ( resources )
		{
			++m_frameCount;
			m_framesTimes[m_frameIndex] = m_frameTime;
			m_frameIndex = ++m_frameIndex % FrameSamplesCount;

			auto before = std::chrono::high_resolution_clock::now();
			m_graphicsQueue->submit( *m_commandBuffers[resources->getImageIndex()]
				, resources->getImageAvailableSemaphore()
				, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, resources->getRenderingFinishedSemaphore()
				, &resources->getFence() );
			m_swapChain->present( *resources, *m_presentQueue );

			ashes::UInt64Array values{ 0u, 0u };
			m_queryPool->getResults( 0u
				, 2u
				, 0u
				, VK_QUERY_RESULT_WAIT_BIT
				, values );
			// Elapsed time in nanoseconds
			auto elapsed = std::chrono::nanoseconds{ uint64_t( float( values[1] - values[0] ) / float( m_device->getDevice().getTimestampPeriod() ) ) };
			auto after = std::chrono::high_resolution_clock::now();
			wxGetApp().updateFps( std::chrono::duration_cast< std::chrono::microseconds >( elapsed )
				, std::chrono::duration_cast< std::chrono::microseconds >( after - before ) );
		}
		else
		{
			m_timer.Stop();
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
			event.Skip( false );
		}
	}

	void RenderPanel::onSize( wxSizeEvent & event )
	{
		m_timer.Stop();
		m_device->getDevice().waitIdle();
		wxSize size{ GetClientSize() };
		m_swapChain->reset( { uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ) } );
		m_timer.Start( TimerTimeMs );
		event.Skip();
	}

	void RenderPanel::onMouseLDown( wxMouseEvent & event )
	{
		m_mouse.left = true;
		m_mouse.position.x = event.GetPosition().x;
		m_mouse.position.y = event.GetPosition().y;
		event.Skip();
	}

	void RenderPanel::onMouseLUp( wxMouseEvent & event )
	{
		m_mouse.left = false;
		m_mouse.position.x = event.GetPosition().x;
		m_mouse.position.y = event.GetPosition().y;
		event.Skip();
	}

	void RenderPanel::onMouseLDClick( wxMouseEvent & event )
	{
		m_mouse.left = true;
		m_mouse.position.x = event.GetPosition().x;
		m_mouse.position.y = event.GetPosition().y;
		event.Skip();
	}

	void RenderPanel::onMouseRDown( wxMouseEvent & event )
	{
		m_mouse.right = true;
		m_mouse.position.x = event.GetPosition().x;
		m_mouse.position.y = event.GetPosition().y;
		event.Skip();
	}

	void RenderPanel::onMouseRUp( wxMouseEvent & event )
	{
		m_mouse.right = false;
		m_mouse.position.x = event.GetPosition().x;
		m_mouse.position.y = event.GetPosition().y;
		event.Skip();
	}

	void RenderPanel::onMouseRDClick( wxMouseEvent & event )
	{
		m_mouse.right = true;
		m_mouse.position.x = event.GetPosition().x;
		m_mouse.position.y = event.GetPosition().y;
		event.Skip();
	}

	void RenderPanel::onMouseMove( wxMouseEvent & event )
	{
		m_mouse.position.x = event.GetPosition().x;
		m_mouse.position.y = event.GetPosition().y;
		event.Skip();
	}

	void RenderPanel::onKeyUp( wxKeyEvent & event )
	{
		if ( auto key = event.GetUnicodeKey();
			key == wxT( 'F' )
				&& m_curIndex < m_choicesIndex.size() - 1u )
		{
			doCreateTextureDst( m_curIndex + 1u );
			doUpdateGui();
		}

		event.Skip();
	}
}
