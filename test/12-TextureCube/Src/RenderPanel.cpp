#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <AshesPP/Buffer/StagingBuffer.hpp>
#include <AshesPP/Buffer/UniformBuffer.hpp>
#include <AshesPP/Buffer/VertexBuffer.hpp>
#include <AshesPP/Command/Queue.hpp>
#include <AshesPP/Core/Surface.hpp>
#include <AshesPP/Core/Device.hpp>
#include <AshesPP/Core/Instance.hpp>
#include <AshesPP/Core/SwapChain.hpp>
#include <AshesPP/Descriptor/DescriptorSet.hpp>
#include <AshesPP/Descriptor/DescriptorSetLayout.hpp>
#include <AshesPP/Descriptor/DescriptorSetPool.hpp>
#include <AshesPP/Image/Image.hpp>
#include <AshesPP/Image/ImageView.hpp>
#include <AshesPP/Miscellaneous/QueryPool.hpp>
#include <AshesPP/RenderPass/FrameBuffer.hpp>
#include <AshesPP/RenderPass/RenderPass.hpp>
#include <Utils/GlslToSpv.hpp>

#include <Utils/Transform.hpp>

#include <FileUtils.hpp>

#include <chrono>

namespace vkapp
{
	namespace
	{
		enum class Ids
		{
			RenderTimer = 42
		}	Ids;

		static int const TimerTimeMs = 20;
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, utils::Instance const & instance )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
		, m_timer{ new wxTimer{ this, int( Ids::RenderTimer ) } }
		, m_offscreenVertexData
		{
			// Front
			{ { -1.0, -1.0, +1.0 } },
			{ { -1.0, +1.0, +1.0 } },
			{ { +1.0, -1.0, +1.0 } },
			{ { +1.0, +1.0, +1.0 } },
			// Top
			{ { -1.0, +1.0, +1.0 } },
			{ { -1.0, +1.0, -1.0 } },
			{ { +1.0, +1.0, +1.0 } },
			{ { +1.0, +1.0, -1.0 } },
			// Back
			{ { -1.0, +1.0, -1.0 } },
			{ { -1.0, -1.0, -1.0 } },
			{ { +1.0, +1.0, -1.0 } },
			{ { +1.0, -1.0, -1.0 } },
			// Bottom
			{ { -1.0, -1.0, -1.0 } },
			{ { -1.0, -1.0, +1.0 } },
			{ { +1.0, -1.0, -1.0 } },
			{ { +1.0, -1.0, +1.0 } },
			// Right
			{ { +1.0, -1.0, +1.0 } },
			{ { +1.0, +1.0, +1.0 } },
			{ { +1.0, -1.0, -1.0 } },
			{ { +1.0, +1.0, -1.0 } },
			// Left
			{ { -1.0, -1.0, -1.0 } },
			{ { -1.0, +1.0, -1.0 } },
			{ { -1.0, -1.0, +1.0 } },
			{ { -1.0, +1.0, +1.0 } },
		}
		, m_offscreenIndexData
		{
			// Front
			0, 1, 2, 2, 1, 3,
			// Top
			4, 5, 6, 6, 5, 7,
			// Back
			8, 9, 10, 10, 9, 11,
			// Bottom
			12, 13, 14, 14, 13, 15,
			// Right
			16, 17, 18, 18, 17, 19,
			// Left
			20, 21, 22, 22, 21, 23,
		}
		, m_mainVertexData
		{
			{ { -1.0, -1.0, 0.0, 1.0 }, { 0.0, 0.0 } },
			{ { -1.0, +1.0, 0.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, -1.0, 0.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, +1.0, 0.0, 1.0 }, { 1.0, 1.0 } },
		}
	{
		try
		{
			auto surface = doCreateSurface( instance );
			std::cout << "Surface created." << std::endl;
			doCreateDevice( instance, *surface );
			std::cout << "Logical device created." << std::endl;
			doCreateSwapChain( std::move( surface ) );
			std::cout << "Swap chain created." << std::endl;
			doCreateStagingBuffer();
			std::cout << "Staging buffer created." << std::endl;
			doCreateTexture();
			std::cout << "Truck texture created." << std::endl;
			doCreateUniformBuffer();
			std::cout << "Uniform buffer created." << std::endl;
			doCreateOffscreenDescriptorSet();
			std::cout << "Offscreen descriptor set created." << std::endl;
			doCreateOffscreenRenderPass();
			std::cout << "Offscreen render pass created." << std::endl;
			doCreateFrameBuffer();
			std::cout << "Frame buffer created." << std::endl;
			doCreateOffscreenVertexBuffer();
			std::cout << "Offscreen vertex buffer created." << std::endl;
			doCreateOffscreenPipeline();
			std::cout << "Offscreen pipeline created." << std::endl;
			doPrepareOffscreenFrame();
			doCreateMainDescriptorSet();
			std::cout << "Main descriptor set created." << std::endl;
			doCreateMainRenderPass();
			std::cout << "Main render pass created." << std::endl;
			doCreateMainVertexBuffer();
			std::cout << "Main vertex buffer created." << std::endl;
			doCreateMainPipeline();
			std::cout << "Main pipeline created." << std::endl;
			doPrepareMainFrames();
		}
		catch ( std::exception & )
		{
			doCleanup();
			throw;
		}

		m_timer->Start( TimerTimeMs );

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
	}

	RenderPanel::~RenderPanel()
	{
		doCleanup();
	}

	void RenderPanel::doCleanup()
	{
		std::cout << "Cleanup" << std::endl;
		delete m_timer;

		if ( m_device )
		{
			m_device->getDevice().waitIdle();

			m_commandBuffer.reset();
			m_commandBuffers.clear();
			m_frameBuffers.clear();
			m_sampler.reset();
			m_view.reset();
			m_texture.reset();
			m_stagingBuffer.reset();

			m_matrixUbo.reset();
			m_objectUbo.reset();
			m_mainDescriptorSet.reset();
			m_mainDescriptorPool.reset();
			m_mainDescriptorLayout.reset();
			m_mainPipeline.reset();
			m_mainPipelineLayout.reset();
			m_mainVertexBuffer.reset();
			m_mainRenderPass.reset();

			m_queryPool.reset();
			m_offscreenDescriptorSet.reset();
			m_offscreenDescriptorPool.reset();
			m_offscreenDescriptorLayout.reset();
			m_offscreenPipeline.reset();
			m_offscreenPipelineLayout.reset();
			m_offscreenIndexBuffer.reset();
			m_offscreenVertexBuffer.reset();
			m_offscreenRenderPass.reset();

			m_frameBuffer.reset();
			m_renderTargetColour.reset();

			m_swapChain.reset();
			m_commandPool.reset();
			m_presentQueue.reset();
			m_graphicsQueue.reset();
			m_device.reset();
		}
	}

	void RenderPanel::doUpdateProjection()
	{
		auto size = m_swapChain->getDimensions();
		auto width = float( size.width );
		auto height = float( size.height );
		auto ratio = width / height;
		m_matrixUbo->getData( 0u ) = utils::Mat4{ m_device->getDevice().perspective( float( utils::toRadians( 90.0_degrees ) / ratio )
			, width / height
			, 0.01f
			, 100.0f ) };
		m_stagingBuffer->uploadUniformData( *m_graphicsQueue
			, *m_commandPool
			, m_matrixUbo->getDatas()
			, *m_matrixUbo
			, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT );
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
		m_clearColour = VkClearColorValue{ 1.0f, 0.8f, 0.4f, 0.0f };
		m_swapChainReset = m_swapChain->onReset.connect( [this]()
		{
			doCreateFrameBuffer();
			doPrepareOffscreenFrame();
			doCreateMainDescriptorSet();
			doPrepareMainFrames();
		} );
	}

	void RenderPanel::doCreateTexture()
	{
		m_texture = m_device->createImage(
			{
				VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
				VK_IMAGE_TYPE_2D,
				VK_FORMAT_R8G8B8A8_UNORM,
				{ 512u, 512u, 1u },
				1u,
				6u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
			}
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_view = m_texture->createView( VK_IMAGE_VIEW_TYPE_CUBE
			, VK_FORMAT_R8G8B8A8_UNORM
			, 0u
			, 1u
			, 0u
			, 6u );
		m_sampler = m_device->getDevice().createSampler( VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_FILTER_LINEAR
			, VK_FILTER_LINEAR );

		std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / "Assets";
		std::array< std::string, 6u > paths
		{
			"01-right.png",
			"02-left.png",
			"03-top.png",
			"04-bottom.png",
			"05-front.png",
			"06-back.png"
		};

		for ( size_t i = 0u; i < paths.size(); ++i )
		{
			auto image = common::loadImage( shadersFolder / paths[i] );
			m_stagingBuffer->uploadTextureData( *m_graphicsQueue
				, *m_commandPool
				, {
					m_view->getSubResourceRange().aspectMask,
					m_view->getSubResourceRange().baseMipLevel,
					uint32_t( i ),
					1u,
				}
				, image.format
				, { 0, 0, 0 }
				, { image.size.width, image.size.height }
				, image.data
				, *m_view );
		}
	}

	void RenderPanel::doCreateUniformBuffer()
	{
		m_matrixUbo = utils::makeUniformBuffer< utils::Mat4 >( *m_device
			, 1u
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_objectUbo = utils::makeUniformBuffer< utils::Mat4 >( *m_device
			, 1u
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
	}

	void RenderPanel::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique< ashes::StagingBuffer >( m_device->getDevice()
			, 0u
			, 20000000u );
	}

	void RenderPanel::doCreateOffscreenDescriptorSet()
	{
		ashes::VkDescriptorSetLayoutBindingArray bindings
		{
			VkDescriptorSetLayoutBinding{ 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT },
			VkDescriptorSetLayoutBinding{ 1u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT },
			VkDescriptorSetLayoutBinding{ 2u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT },
		};
		m_offscreenDescriptorLayout = m_device->getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_offscreenDescriptorPool = m_offscreenDescriptorLayout->createPool( 1u );
		m_offscreenDescriptorSet = m_offscreenDescriptorPool->createDescriptorSet();
		m_offscreenDescriptorSet->createBinding( m_offscreenDescriptorLayout->getBinding( 0u )
			, *m_view
			, *m_sampler );
		m_offscreenDescriptorSet->createBinding( m_offscreenDescriptorLayout->getBinding( 1u )
			, *m_matrixUbo
			, 0u
			, 1u );
		m_offscreenDescriptorSet->createBinding( m_offscreenDescriptorLayout->getBinding( 2u )
			, *m_objectUbo
			, 0u
			, 1u );
		m_offscreenDescriptorSet->update();
	}

	void RenderPanel::doCreateOffscreenRenderPass()
	{
		ashes::VkAttachmentDescriptionArray attaches
		{
			{
				VK_FORMAT_R8G8B8A8_UNORM,
				VK_SAMPLE_COUNT_1_BIT,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_STORE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			}
		};
		ashes::VkAttachmentReferenceArray subAttaches
		{
			{ 0u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }
		};
		ashes::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( std::make_unique< ashes::RenderSubpass >( VkPipelineBindPoint::eGraphics
			, ashes::RenderSubpassState{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT }
			, subAttaches ) );
		m_offscreenRenderPass = m_device->getDevice().createRenderPass( attaches
			, std::move( subpasses )
			, ashes::RenderSubpassState{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT }
			, ashes::RenderSubpassState{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, VK_ACCESS_SHADER_READ_BIT } );
	}

	void RenderPanel::doCreateFrameBuffer()
	{
		auto size = GetClientSize();
		m_renderTargetColour = m_device->createImage(
			{
				0u,
				VK_IMAGE_TYPE_2D,
				VK_FORMAT_R8G8B8A8_UNORM,
				{ uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ), 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
			}
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		ashes::ImageViewPtrArray attaches;
		attaches.emplace_back( *( m_offscreenRenderPass->getAttachments().begin() + 0u )
			, m_renderTargetColour->createView( VK_IMAGE_VIEW_TYPE_2D
				, m_renderTargetColour->getFormat() ) );
		m_frameBuffer = m_offscreenRenderPass->createFrameBuffer( { uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ) }
			, std::move( attaches ) );
	}

	void RenderPanel::doCreateOffscreenVertexBuffer()
	{
		m_offscreenVertexLayout = ashes::makeLayout< NonTexturedVertexData >( 0 );
		m_offscreenVertexLayout->createAttribute( 0u
			, VK_FORMAT_R32G32B32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, position ) ) );

		m_offscreenVertexBuffer = utils::makeVertexBuffer< NonTexturedVertexData >( *m_device
			, uint32_t( m_offscreenVertexData.size() )
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_stagingBuffer->uploadVertexData( *m_graphicsQueue
			, *m_commandPool
			, m_offscreenVertexData
			, *m_offscreenVertexBuffer );

		m_offscreenIndexBuffer = utils::makeBuffer< uint16_t >( *m_device
			, uint32_t( m_offscreenIndexData.size() )
			, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_stagingBuffer->uploadBufferData( *m_graphicsQueue
			, *m_commandPool
			, m_offscreenIndexData
			, *m_offscreenIndexBuffer );
	}

	void RenderPanel::doCreateOffscreenPipeline()
	{
		m_offscreenPipelineLayout = m_device->getDevice().createPipelineLayout( *m_offscreenDescriptorLayout );
		wxSize size{ GetClientSize() };
		std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "offscreen.vert" )
			|| !wxFileExists( shadersFolder / "offscreen.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		ashes::PipelineShaderStageCreateInfoArray shaderStages;
		shaderStages.push_back( { m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
			, VK_SHADER_STAGE_VERTEX_BIT
			, shadersFolder / "offscreen.vert" ) ) } );
		shaderStages.push_back( { m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
			, VK_SHADER_STAGE_FRAGMENT_BIT
			, shadersFolder / "offscreen.frag" ) ) } );

		ashes::RasterisationState rasterisationState;
		rasterisationState.cullMode = VK_CULL_MODE_NONE;

		m_offscreenPipeline = m_offscreenPipelineLayout->createPipeline( ashes::GraphicsPipelineCreateInfo
		{
			std::move( shaderStages ),
			*m_offscreenRenderPass,
			ashes::VertexInputState::create( *m_offscreenVertexLayout ),
			ashes::InputAssemblyState{ VkPrimitiveTopology::eTriangleList },
			rasterisationState,
			ashes::MultisampleState{},
			ashes::ColourBlendState::createDefault(),
			{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR }
		} );
	}

	void RenderPanel::doCreateMainDescriptorSet()
	{
		ashes::VkDescriptorSetLayoutBindingArray bindings
		{
			VkDescriptorSetLayoutBinding{ 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT },
		};
		m_mainDescriptorLayout = m_device->getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_mainDescriptorPool = m_mainDescriptorLayout->createPool( 1u );
		m_mainDescriptorSet = m_mainDescriptorPool->createDescriptorSet();
		m_mainDescriptorSet->createBinding( m_mainDescriptorLayout->getBinding( 0u )
			, *( *m_frameBuffer->begin() )
			, *m_sampler );
		m_mainDescriptorSet->update();
	}

	void RenderPanel::doCreateMainRenderPass()
	{
		ashes::VkAttachmentDescriptionArray attaches
		{
			{
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
		ashes::VkAttachmentReferenceArray subAttaches
		{
			{ 0u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }
		};
		ashes::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( std::make_unique< ashes::RenderSubpass >( VkPipelineBindPoint::eGraphics
			, ashes::RenderSubpassState{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT }
			, subAttaches ) );
		m_mainRenderPass = m_device->getDevice().createRenderPass( attaches
			, std::move( subpasses )
			, ashes::RenderSubpassState{ VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
				, VK_ACCESS_MEMORY_READ_BIT }
			, ashes::RenderSubpassState{ VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
				, VK_ACCESS_MEMORY_READ_BIT } );
	}

	void RenderPanel::doPrepareOffscreenFrame()
	{
		doUpdateProjection();
		m_queryPool = m_device->getDevice().createQueryPool( VK_QUERY_TYPE_TIMESTAMP
			, 2u
			, 0u );
		m_commandBuffer = m_commandPool->createCommandBuffer();
		wxSize size{ GetClientSize() };
		auto & commandBuffer = *m_commandBuffer;
		auto & frameBuffer = *m_frameBuffer;

		commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT );
		auto dimensions = m_swapChain->getDimensions();
		commandBuffer.resetQueryPool( *m_queryPool
			, 0u
			, 2u );
		commandBuffer.beginRenderPass( *m_offscreenRenderPass
			, frameBuffer
			, { VkClearValue{ m_clearColour } }
			, VK_SUBPASS_CONTENTS_INLINE );
		commandBuffer.writeTimestamp( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, *m_queryPool
			, 0u );
		commandBuffer.bindPipeline( *m_offscreenPipeline );
		commandBuffer.setViewport( { 0.0f, 0.0f, float( dimensions.width ), float( dimensions.height ), 0.0f, 1.0f } );
		commandBuffer.setScissor( { { 0, 0 }, { dimensions.width, dimensions.height } } );
		commandBuffer.bindVertexBuffer( 0u, m_offscreenVertexBuffer->getBuffer(), 0u );
		commandBuffer.bindIndexBuffer( m_offscreenIndexBuffer->getBuffer(), 0u, VK_INDEX_TYPE_UINT16 );
		commandBuffer.bindDescriptorSet( *m_offscreenDescriptorSet
			, *m_offscreenPipelineLayout );
		commandBuffer.drawIndexed( uint32_t( m_offscreenIndexData.size() ) );
		commandBuffer.writeTimestamp( VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
			, *m_queryPool
			, 1u );
		commandBuffer.endRenderPass();
		commandBuffer.end();
	}

	void RenderPanel::doCreateMainVertexBuffer()
	{
		m_mainVertexLayout = ashes::makeLayout< TexturedVertexData >( 0 );
		m_mainVertexLayout->createAttribute( 0u
			, VK_FORMAT_R32G32B32A32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, position ) ) );
		m_mainVertexLayout->createAttribute( 1u
			, VK_FORMAT_R32G32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, uv ) ) );

		m_mainVertexBuffer = utils::makeVertexBuffer< TexturedVertexData >( *m_device
			, uint32_t( m_mainVertexData.size() )
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_stagingBuffer->uploadVertexData( *m_graphicsQueue
			, *m_commandPool
			, m_mainVertexData
			, *m_mainVertexBuffer );
	}

	void RenderPanel::doCreateMainPipeline()
	{
		m_mainPipelineLayout = m_device->getDevice().createPipelineLayout( *m_mainDescriptorLayout );
		wxSize size{ GetClientSize() };
		std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "main.vert" )
			|| !wxFileExists( shadersFolder / "main.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		ashes::PipelineShaderStageCreateInfoArray shaderStages;
		shaderStages.push_back( { m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
			, VK_SHADER_STAGE_VERTEX_BIT
			, shadersFolder / "main.vert" ) ) } );
		shaderStages.push_back( { m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
			, VK_SHADER_STAGE_FRAGMENT_BIT
			, shadersFolder / "main.frag" ) ) } );

		m_mainPipeline = m_mainPipelineLayout->createPipeline( ashes::GraphicsPipelineCreateInfo
		{
			std::move( shaderStages ),
			*m_mainRenderPass,
			ashes::VertexInputState::create( *m_mainVertexLayout ),
			ashes::InputAssemblyState{ VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP },
			ashes::RasterisationState{},
			ashes::MultisampleState{},
			ashes::ColourBlendState::createDefault(),
			{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR }
		} );
	}

	void RenderPanel::doPrepareMainFrames()
	{
		m_frameBuffers = m_swapChain->createFrameBuffers( *m_mainRenderPass );
		m_commandBuffers = m_swapChain->createCommandBuffers();

		for ( size_t i = 0u; i < m_frameBuffers.size(); ++i )
		{
			auto & frameBuffer = *m_frameBuffers[i];
			auto & commandBuffer = *m_commandBuffers[i];

			wxSize size{ GetClientSize() };

			commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT );
			auto dimensions = m_swapChain->getDimensions();
			commandBuffer.beginRenderPass( *m_mainRenderPass
				, frameBuffer
				, { VkClearValue{ { 1.0, 0.0, 0.0, 1.0 } } }
				, VK_SUBPASS_CONTENTS_INLINE );
			commandBuffer.bindPipeline( *m_mainPipeline );
			commandBuffer.setViewport( { 0.0f, 0.0f, float( dimensions.width ), float( dimensions.height ), 0.0f, 1.0f } );
			commandBuffer.setScissor( { { 0, 0 }, { dimensions.width, dimensions.height } } );
			commandBuffer.bindVertexBuffer( 0u, m_mainVertexBuffer->getBuffer(), 0u );
			commandBuffer.bindDescriptorSet( *m_mainDescriptorSet
				, *m_mainPipelineLayout );
			commandBuffer.draw( 4u );
			commandBuffer.endRenderPass();

			commandBuffer.end();
		}
	}

	void RenderPanel::doUpdate()
	{
		static utils::Mat4 const originalRotate = []()
		{
			utils::Mat4 result;
			result = utils::rotate( result
				, float( utils::DegreeToRadian * 45.0 )
				, { 0, 0, 1 } );
			return result;
		}();
		m_rotate = utils::rotate( m_rotate
			, float( utils::DegreeToRadian )
			, { 0, 1, 0 } );
		m_objectUbo->getData( 0 ) = m_rotate * originalRotate;
		m_stagingBuffer->uploadUniformData( *m_graphicsQueue
			, *m_commandPool
			, m_objectUbo->getDatas()
			, *m_objectUbo
			, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT );
	}

	void RenderPanel::doDraw()
	{
		auto resources = m_swapChain->getResources();

		if ( resources )
		{
			auto before = std::chrono::high_resolution_clock::now();
			m_graphicsQueue->submit( *m_commandBuffer
				, nullptr );
			m_graphicsQueue->waitIdle();

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
			auto elapsed = std::chrono::nanoseconds{ uint64_t( ( values[1] - values[0] ) / float( m_device->getDevice().getTimestampPeriod() ) ) };
			auto after = std::chrono::high_resolution_clock::now();
			wxGetApp().updateFps( std::chrono::duration_cast< std::chrono::microseconds >( elapsed )
				, std::chrono::duration_cast< std::chrono::microseconds >( after - before ) );
		}
		else
		{
			m_timer->Stop();
		}
	}

	void RenderPanel::doResetSwapChain()
	{
		m_device->getDevice().waitIdle();
		wxSize size{ GetClientSize() };
		m_swapChain->reset( { uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ) } );
	}

	void RenderPanel::onTimer( wxTimerEvent & event )
	{
		if ( event.GetId() == int( Ids::RenderTimer ) )
		{
			doUpdate();
			doDraw();
		}
	}

	void RenderPanel::onSize( wxSizeEvent & event )
	{
		m_timer->Stop();
		doResetSwapChain();
		m_timer->Start( TimerTimeMs );
		event.Skip();
	}
}
