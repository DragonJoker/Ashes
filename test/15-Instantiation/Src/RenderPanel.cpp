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

		static int constexpr TimerTimeMs = 40;
		static VkFormat const DepthFormat = VK_FORMAT_D32_SFLOAT;
		static uint32_t constexpr ObjectCount = 100;
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, utils::Instance const & instance )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
		, m_timer{ new wxTimer{ this, int( Ids::RenderTimer ) } }
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
			doCreateDescriptorSet();
			std::cout << "Descriptor set created." << std::endl;
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
			, wxEVT_LEFT_DCLICK
			, wxMouseEventHandler( RenderPanel::onMouseLDoubleClick )
			, nullptr
			, this );
		Connect( GetId()
			, wxEVT_LEFT_UP
			, wxMouseEventHandler( RenderPanel::onMouseLUp )
			, nullptr
			, this );
		Connect( GetId()
			, wxEVT_MOTION
			, wxMouseEventHandler( RenderPanel::onMouseMove )
			, nullptr
			, this );
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
			m_device->getDevice().waitIdle();

			m_commandBuffers.clear();
			m_frameBuffers.clear();
			m_depthStencil.reset();
			m_sampler.reset();
			m_view.reset();
			m_texture.reset();
			m_stagingBuffer.reset();

			m_matrixUbo.reset();

			m_queryPool.reset();
			m_descriptorSet.reset();
			m_descriptorPool.reset();
			m_descriptorLayout.reset();
			m_pipeline.reset();
			m_pipelineLayout.reset();
			m_matrixBuffer.reset();
			m_indexBuffer.reset();
			m_vertexBuffer.reset();
			m_renderPass.reset();

			m_swapChainReset.disconnect();
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
		m_projection = utils::Mat4{ m_device->getDevice().perspective( float( utils::toRadians( 90.0_degrees ) )
			, width / height
			, 0.01f
			, 1000.0f ) };
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
			doPrepareFrames();
		} );
	}

	void RenderPanel::doCreateTexture()
	{
		std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / "Assets";
		auto image = common::loadImage( shadersFolder / "texture.png" );
		m_texture = m_device->createImage(
			{
				0u,
				VK_IMAGE_TYPE_2D,
				image.format,
				{ image.size.width, image.size.height, 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
			}
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_view = m_texture->createView( VK_IMAGE_VIEW_TYPE_2D
			, image.format );
		m_sampler = m_device->getDevice().createSampler( VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_FILTER_LINEAR
			, VK_FILTER_LINEAR );
		m_stagingBuffer->uploadTextureData( *m_graphicsQueue
			, *m_commandPool
			, image.format
			, image.data
			, *m_view );
	}

	void RenderPanel::doCreateUniformBuffer()
	{
		m_matrixUbo = utils::makeUniformBuffer< utils::Mat4 >( *m_device
			, 1u
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
	}

	void RenderPanel::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique< ashes::StagingBuffer >( m_device->getDevice()
			, 0u
			, ObjectCount * ObjectCount * ObjectCount * 64u );
	}

	void RenderPanel::doCreateDescriptorSet()
	{
		ashes::VkDescriptorSetLayoutBindingArray bindings
		{
			{ 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			{ 1u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_VERTEX_BIT, nullptr },
		};
		m_descriptorLayout = m_device->getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_descriptorPool = m_descriptorLayout->createPool( 1u );
		m_descriptorSet = m_descriptorPool->createDescriptorSet();
		m_descriptorSet->createBinding( m_descriptorLayout->getBinding( 0u )
			, *m_view
			, *m_sampler );
		m_descriptorSet->createBinding( m_descriptorLayout->getBinding( 1u )
			, *m_matrixUbo
			, 0u
			, 1u );
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
			},
			{
				1u,
				DepthFormat,
				VK_SAMPLE_COUNT_1_BIT,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_STORE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
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
				VkAttachmentReference{ 1u, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL },
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
		std::vector< TexturedVertexData > vertexData
		{
			// Front
			{ { -1.0, -1.0, +1.0, 1.0 }, { 0.0, 0.0 } },
			{ { -1.0, +1.0, +1.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, -1.0, +1.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, +1.0, +1.0, 1.0 }, { 1.0, 1.0 } },
			// Top
			{ { -1.0, +1.0, +1.0, 1.0 }, { 0.0, 0.0 } },
			{ { -1.0, +1.0, -1.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, +1.0, +1.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, +1.0, -1.0, 1.0 }, { 1.0, 1.0 } },
			// Back
			{ { -1.0, +1.0, -1.0, 1.0 }, { 1.0, 1.0 } },
			{ { -1.0, -1.0, -1.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, +1.0, -1.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, -1.0, -1.0, 1.0 }, { 0.0, 0.0 } },
			// Bottom
			{ { -1.0, -1.0, -1.0, 1.0 }, { 1.0, 1.0 } },
			{ { -1.0, -1.0, +1.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, -1.0, -1.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, -1.0, +1.0, 1.0 }, { 0.0, 0.0 } },
			// Right
			{ { +1.0, -1.0, +1.0, 1.0 }, { 0.0, 0.0 } },
			{ { +1.0, +1.0, +1.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, -1.0, -1.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, +1.0, -1.0, 1.0 }, { 1.0, 1.0 } },
			// Left
			{ { -1.0, -1.0, -1.0, 1.0 }, { 0.0, 0.0 } },
			{ { -1.0, +1.0, -1.0, 1.0 }, { 0.0, 1.0 } },
			{ { -1.0, -1.0, +1.0, 1.0 }, { 1.0, 0.0 } },
			{ { -1.0, +1.0, +1.0, 1.0 }, { 1.0, 1.0 } },
		};
		m_vertexBuffer = utils::makeVertexBuffer< TexturedVertexData >( *m_device
			, uint32_t( vertexData.size() )
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_stagingBuffer->uploadVertexData( *m_graphicsQueue
			, *m_commandPool
			, vertexData
			, *m_vertexBuffer );

		ashes::UInt16Array indexData
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
		};
		m_indexBuffer = utils::makeBuffer< uint16_t >( *m_device
			, uint32_t( indexData.size() )
			, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_stagingBuffer->uploadBufferData( *m_graphicsQueue
			, *m_commandPool
			, indexData
			, *m_indexBuffer );
		
		auto init = ObjectCount * -2.0f;
		utils::Vec3 position{ init, init, init };
		std::vector< utils::Mat4 > matrices;
		matrices.reserve( ObjectCount * ObjectCount * ObjectCount );

		for ( auto i = 0u; i < ObjectCount; ++i )
		{
			position[1] = init;

			for ( auto j = 0u; j < ObjectCount; ++j )
			{
				position[2] = init;

				for ( auto k = 0u; k < ObjectCount; ++k )
				{
					matrices.emplace_back(
						utils::Vec4{ 1, 0, 0, 0 },
						utils::Vec4{ 0, 1, 0, 0 },
						utils::Vec4{ 0, 0, 1, 0 },
						utils::Vec4{ position[0], position[1], position[2], 1 }
					);
					position[2] += 4;
				}

				position[1] += 4;
			}

			position[0] += 4;
		}

		m_matrixBuffer = utils::makeVertexBuffer< utils::Mat4 >( *m_device
			, ObjectCount * ObjectCount * ObjectCount
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_stagingBuffer->uploadVertexData( *m_graphicsQueue
			, *m_commandPool
			, matrices
			, *m_matrixBuffer );
	}

	void RenderPanel::doCreatePipeline()
	{
		m_pipelineLayout = m_device->getDevice().createPipelineLayout( *m_descriptorLayout );
		wxSize size{ GetClientSize() };
		std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "shader.vert" )
			|| !wxFileExists( shadersFolder / "shader.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}
		
		ashes::PipelineVertexInputStateCreateInfo vertexLayouts
		{
			0u,
			{
				{ 0u, sizeof( TexturedVertexData ), VK_VERTEX_INPUT_RATE_VERTEX },
				{ 1u, sizeof( utils::Mat4 ), VK_VERTEX_INPUT_RATE_INSTANCE },
			},
			{
				{ 0u, 0u, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof( TexturedVertexData, position ) },
				{ 1u, 0u, VK_FORMAT_R32G32_SFLOAT, offsetof( TexturedVertexData, uv ) },
				{ 2u, 1u, VK_FORMAT_R32G32B32A32_SFLOAT, uint32_t( 0u * sizeof( float ) * 4u ) },
				{ 3u, 1u, VK_FORMAT_R32G32B32A32_SFLOAT, uint32_t( 1u * sizeof( float ) * 4u ) },
				{ 4u, 1u, VK_FORMAT_R32G32B32A32_SFLOAT, uint32_t( 2u * sizeof( float ) * 4u ) },
				{ 5u, 1u, VK_FORMAT_R32G32B32A32_SFLOAT, uint32_t( 3u * sizeof( float ) * 4u ) },
			},
		};

		ashes::PipelineShaderStageCreateInfoArray shaderStages;
		shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
			{
				0u,
				VK_SHADER_STAGE_VERTEX_BIT,
				m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
					, VK_SHADER_STAGE_VERTEX_BIT
					, shadersFolder / "shader.vert" ) ),
				"main",
				std::nullopt,
			} );
		shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
			{
				0u,
				VK_SHADER_STAGE_FRAGMENT_BIT,
				m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
					, VK_SHADER_STAGE_FRAGMENT_BIT
					, shadersFolder / "shader.frag" ) ),
				"main",
				std::nullopt,
			} );

		m_pipeline = m_device->getDevice().createPipeline( ashes::GraphicsPipelineCreateInfo
			{
				0u,
				std::move( shaderStages ),
				std::move( vertexLayouts ),
				ashes::PipelineInputAssemblyStateCreateInfo{ 0u, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST },
				std::nullopt,
				ashes::PipelineViewportStateCreateInfo{},
				ashes::PipelineRasterizationStateCreateInfo{},
				ashes::PipelineMultisampleStateCreateInfo{},
				ashes::PipelineDepthStencilStateCreateInfo{},
				ashes::PipelineColorBlendStateCreateInfo{},
				ashes::PipelineDynamicStateCreateInfo{ 0u, { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR } },
				*m_pipelineLayout,
				*m_renderPass,
			} );
	}

	void RenderPanel::doCreateDepthStencil()
	{
		m_depthStencil = m_device->createImage(
			{
				0u,
				VK_IMAGE_TYPE_2D,
				DepthFormat,
				VkExtent3D{ m_swapChain->getDimensions().width, m_swapChain->getDimensions().height, 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				{},
				VK_IMAGE_LAYOUT_UNDEFINED,
			}
		, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
	}

	void RenderPanel::doPrepareFrames()
	{
		doUpdateProjection();
		doCreateDepthStencil();
		m_frameBuffers = m_swapChain->createFrameBuffers( *m_renderPass
			, *m_depthStencil );
		m_commandBuffers = m_swapChain->createCommandBuffers();
		m_queryPool = m_device->getDevice().createQueryPool( VK_QUERY_TYPE_TIMESTAMP
			, 2u
			, 0u );

		for ( size_t i = 0u; i < m_frameBuffers.size(); ++i )
		{
			auto & frameBuffer = *m_frameBuffers[i];
			auto & commandBuffer = *m_commandBuffers[i];
			wxSize size{ GetClientSize() };
			auto dimensions = m_swapChain->getDimensions();

			commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT );
			commandBuffer.resetQueryPool( *m_queryPool
				, 0u
				, 2u );
			commandBuffer.beginRenderPass( *m_renderPass
				, frameBuffer
				, { ashes::makeClearValue( m_clearColour ), ashes::makeClearValue( VkClearDepthStencilValue{ 1.0f, 0u } ) }
				, VK_SUBPASS_CONTENTS_INLINE );
			commandBuffer.writeTimestamp( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, *m_queryPool
				, 0u );
			commandBuffer.bindPipeline( *m_pipeline );
			commandBuffer.setViewport( { 0.0f, 0.0f, float( dimensions.width ), float( dimensions.height ), 0.0f, 1.0f } );
			commandBuffer.setScissor( { { 0, 0 }, { dimensions.width, dimensions.height } } );
			commandBuffer.bindVertexBuffers( 0u
				, { m_vertexBuffer->getBuffer(), m_matrixBuffer->getBuffer() }
			, { 0u, 0u } );
			commandBuffer.bindIndexBuffer( m_indexBuffer->getBuffer(), 0u, VK_INDEX_TYPE_UINT16 );
			commandBuffer.bindDescriptorSet( *m_descriptorSet
				, *m_pipelineLayout );
			commandBuffer.drawIndexed( m_indexBuffer->getCount()
				, m_matrixBuffer->getCount() );
			commandBuffer.writeTimestamp( VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
				, *m_queryPool
				, 1u );
			commandBuffer.endRenderPass();
			commandBuffer.end();
		}
	}

	void RenderPanel::doUpdate()
	{
		m_camera.update();
		m_matrixUbo->getData( 0u ) = m_projection * m_camera.getView();
		m_stagingBuffer->uploadUniformData( *m_graphicsQueue
			, *m_commandPool
			, m_matrixUbo->getDatas()
			, *m_matrixUbo
			, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT );
	}

	void RenderPanel::doDraw()
	{
		auto resources = m_swapChain->getResources();

		if ( resources )
		{
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

	void RenderPanel::onMouseLDoubleClick( wxMouseEvent & event )
	{
		m_moveCamera = false;
		m_camera.reset();
	}

	void RenderPanel::onMouseLDown( wxMouseEvent & event )
	{
		m_moveCamera = true;
		m_previousMousePosition[0] = event.GetPosition().x;
		m_previousMousePosition[1] = event.GetPosition().y;
	}

	void RenderPanel::onMouseLUp( wxMouseEvent & event )
	{
		m_moveCamera = false;
	}

	void RenderPanel::onMouseMove( wxMouseEvent & event )
	{
		if ( m_moveCamera )
		{
			auto size = GetClientSize();
			auto currentPosition = utils::IVec2{ event.GetPosition().x, event.GetPosition().y };
			auto delta = currentPosition - m_previousMousePosition;
			auto & result = m_camera.getRotation();
			result = utils::pitch( result, utils::Radians{ float( delta[1] ) / size.GetHeight() } );
			result = utils::yaw( result, utils::Radians{ float( -delta[0] ) / size.GetWidth() } );
			m_previousMousePosition[0] = event.GetPosition().x;
			m_previousMousePosition[1] = event.GetPosition().y;
		}
	}
}
