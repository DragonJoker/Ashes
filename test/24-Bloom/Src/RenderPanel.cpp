#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <ashespp/Buffer/StagingBuffer.hpp>
#include <ashespp/Buffer/UniformBuffer.hpp>
#include <ashespp/Buffer/VertexBuffer.hpp>
#include <ashespp/Command/Queue.hpp>
#include <ashespp/Core/Surface.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Core/Instance.hpp>
#include <ashespp/Core/SwapChain.hpp>
#include <ashespp/Descriptor/DescriptorSet.hpp>
#include <ashespp/Descriptor/DescriptorSetLayout.hpp>
#include <ashespp/Descriptor/DescriptorSetPool.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/Miscellaneous/QueryPool.hpp>
#include <ashespp/RenderPass/FrameBuffer.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>
#include <ashespp/RenderPass/RenderPassCreateInfo.hpp>
#include <util/GlslToSpv.hpp>

#include <util/Transform.hpp>

#include <FileUtils.hpp>

#include <algorithm>
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

		std::vector< float > getHalfPascal( uint32_t height )
		{
			std::vector< float > result;
			result.resize( height );
			auto x = 1.0f;
			auto max = 1 + height;

			for ( uint32_t i = 0u; i <= max; ++i )
			{
				auto index = max - i;

				if ( index < height )
				{
					result[index] = x;
				}

				x = x * ( ( height + 1 ) * 2 - i ) / ( i + 1 );
			}

			// Normalize kernel coefficients
			float sum = result[0];

			for ( uint32_t i = 1u; i < height; ++i )
			{
				sum += result[i] * 2;
			}

			std::transform( std::begin( result )
				, std::end( result )
				, std::begin( result )
				, [&sum]( float & value )
				{
					return value /= sum;
				} );

			return result;
		}

		std::array< utils::Vec4, 15u > doCreateKernel( uint32_t count )
		{
			std::array< utils::Vec4, 15u > result;
			auto kernel = getHalfPascal( count );
			std::memcpy( result.data()->ptr()
				, kernel.data()
				, sizeof( float ) * std::min( size_t( 60u ), kernel.size() ) );
			return result;
		}

		ashes::RenderPassPtr createRenderPass( ashes::Device const & device
			, VkFormat format
			, VkImageLayout initialImageLayout
			, VkImageLayout finalImageLayout )
		{
			ashes::VkAttachmentDescriptionArray attaches
			{
				{
					0u,
					format,
					VK_SAMPLE_COUNT_1_BIT,
					VK_ATTACHMENT_LOAD_OP_CLEAR,
					VK_ATTACHMENT_STORE_OP_STORE,
					VK_ATTACHMENT_LOAD_OP_DONT_CARE,
					VK_ATTACHMENT_STORE_OP_DONT_CARE,
					initialImageLayout,
					finalImageLayout,
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
					std::nullopt,
					{},
				} );
			ashes::VkSubpassDependencyArray dependencies
			{
				{
					VK_SUBPASS_EXTERNAL,
					0u,
					ashes::getStageMask( initialImageLayout ),
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					ashes::getAccessMask( initialImageLayout ),
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
					VK_DEPENDENCY_BY_REGION_BIT,
				},
				{
					0u,
					VK_SUBPASS_EXTERNAL,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					ashes::getStageMask( finalImageLayout ),
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
					ashes::getAccessMask( finalImageLayout ),
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
			return device.createRenderPass( std::move( createInfo ) );
		}
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
			std::cout << "Offscreen frame prepared." << std::endl;
			doCreateMainVertexBuffer();
			std::cout << "Main vertex buffer created." << std::endl;
			doPrepareHiPass();
			std::cout << "Hi pass created." << std::endl;
			doPrepareBlurXPass();
			std::cout << "Horizontal blur pass created." << std::endl;
			doPrepareBlurYPass();
			std::cout << "Vertical blur pass created." << std::endl;
			doPrepareCombinePass();
			std::cout << "Combine pass created." << std::endl;
			doCreateMainDescriptorSet();
			std::cout << "Main descriptor set created." << std::endl;
			doCreateMainRenderPass();
			std::cout << "Main render pass created." << std::endl;
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

			m_mipSampler.reset();

			{
				Pass dummyCombine;
				std::swap( m_passes.combine, dummyCombine );
			}

			for ( auto & blur : m_passes.blurY )
			{
				Pass dummy;
				std::swap( blur, dummy );
			}

			for ( auto & blur : m_passes.blurX )
			{
				Pass dummy;
				std::swap( blur, dummy );
			}

			{
				Pass dummyHi;
				std::swap( m_passes.hi, dummyHi );
			}
			m_blurConfigurationUbo.reset();
			m_blurDirectionUbo.reset();

			for ( auto & sampler : m_blurSamplers )
			{
				sampler.reset();
			}

			m_commandBuffer.reset();
			m_commandBuffers.clear();
			m_frameBuffers.clear();
			m_sampler.reset();
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
			m_semaphore.reset();
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
		m_matrixData = utils::Mat4{ m_device->getDevice().perspective( float( utils::toRadians( 90.0_degrees ) / ratio )
			, width / height
			, 0.01f
			, 100.0f ) };
		m_stagingBuffer->uploadUniformData( *m_graphicsQueue
			, *m_commandPool
			, &m_matrixData
			, 1u
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
			m_mainDescriptorSet.reset();
			{
				Pass dummyCombine;
				std::swap( m_passes.combine, dummyCombine );
			}

			for ( auto & blur : m_passes.blurY )
			{
				Pass dummy;
				std::swap( blur, dummy );
			}

			for ( auto & blur : m_passes.blurX )
			{
				Pass dummy;
				std::swap( blur, dummy );
			}

			{
				Pass dummyHi;
				std::swap( m_passes.hi, dummyHi );
			}
			doPrepareHiPass();
			doPrepareBlurXPass();
			doPrepareBlurYPass();
			doPrepareCombinePass();
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
					m_view->subresourceRange.aspectMask,
					m_view->subresourceRange.baseMipLevel,
					uint32_t( i ),
					1u,
				}
				, image.format
				, { 0, 0, 0 }
				, { image.size.width, image.size.height }
				, image.data
				, m_view );
		}
	}

	void RenderPanel::doCreateUniformBuffer()
	{
		m_matrixUbo = utils::makeUniformBuffer( *m_device
			, 1u
			, uint32_t( sizeof( utils::Mat4 ) )
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_objectUbo = utils::makeUniformBuffer( *m_device
			, 1u
			, uint32_t( sizeof( utils::Mat4 ) )
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
			{ 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			{ 1u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_VERTEX_BIT, nullptr },
			{ 2u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_VERTEX_BIT, nullptr },
		};
		m_offscreenDescriptorLayout = m_device->getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_offscreenDescriptorPool = m_offscreenDescriptorLayout->createPool( 1u );
		m_offscreenDescriptorSet = m_offscreenDescriptorPool->createDescriptorSet();
		m_offscreenDescriptorSet->createBinding( m_offscreenDescriptorLayout->getBinding( 0u )
			, m_view
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
				0u,
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
		ashes::SubpassDescriptionArray subpasses;
		subpasses.emplace_back( ashes::SubpassDescription
			{
				0u,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				{},
				{ { 0u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } },
				{},
				std::nullopt,
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
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_ACCESS_SHADER_READ_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
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
		m_offscreenRenderPass = m_device->getDevice().createRenderPass( std::move( createInfo ) );
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
		m_renderTargetColourView = m_renderTargetColour->createView( VK_IMAGE_VIEW_TYPE_2D
			, m_renderTargetColour->getFormat() );
		ashes::ImageViewCRefArray attaches;
		attaches.emplace_back( m_renderTargetColourView );
		m_frameBuffer = m_offscreenRenderPass->createFrameBuffer( { uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ) }
			, std::move( attaches ) );
	}

	void RenderPanel::doCreateOffscreenVertexBuffer()
	{
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
		m_semaphore = m_device->getDevice().createSemaphore();
		m_offscreenPipelineLayout = m_device->getDevice().createPipelineLayout( *m_offscreenDescriptorLayout );
		wxSize size{ GetClientSize() };
		std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "offscreen.vert" )
			|| !wxFileExists( shadersFolder / "offscreen.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}
		
		ashes::PipelineVertexInputStateCreateInfo vertexLayout
		{
			0u,
			{
				{ 0u, sizeof( NonTexturedVertexData ), VK_VERTEX_INPUT_RATE_VERTEX },
			},
			{
				{ 0u, 0u, VK_FORMAT_R32G32B32_SFLOAT, offsetof( NonTexturedVertexData, position ) },
			},
		};

		ashes::PipelineShaderStageCreateInfoArray shaderStages;
		shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
			{
				0u,
				VK_SHADER_STAGE_VERTEX_BIT,
				m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
					, VK_SHADER_STAGE_VERTEX_BIT
					, shadersFolder / "offscreen.vert" ) ),
				"main",
				std::nullopt,
			} );
		shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
			{
				0u,
				VK_SHADER_STAGE_FRAGMENT_BIT,
				m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
					, VK_SHADER_STAGE_FRAGMENT_BIT
					, shadersFolder / "offscreen.frag" ) ),
				"main",
				std::nullopt,
			} );

		m_offscreenPipeline = m_device->getDevice().createPipeline( ashes::GraphicsPipelineCreateInfo
			{
				0u,
				std::move( shaderStages ),
				std::move( vertexLayout ),
				ashes::PipelineInputAssemblyStateCreateInfo{ 0u, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST },
				std::nullopt,
				ashes::PipelineViewportStateCreateInfo{},
				ashes::PipelineRasterizationStateCreateInfo{ 0u, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE },
				ashes::PipelineMultisampleStateCreateInfo{},
				std::nullopt,
				ashes::PipelineColorBlendStateCreateInfo{},
				ashes::PipelineDynamicStateCreateInfo{ 0u, { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR } },
				*m_offscreenPipelineLayout,
				*m_offscreenRenderPass,
			} );
	}

	void RenderPanel::doCreateMainDescriptorSet()
	{
		ashes::VkDescriptorSetLayoutBindingArray bindings
		{
			{ 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
		};
		m_mainDescriptorLayout = m_device->getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_mainDescriptorPool = m_mainDescriptorLayout->createPool( 1u );
		m_mainDescriptorSet = m_mainDescriptorPool->createDescriptorSet();
		m_mainDescriptorSet->createBinding( m_mainDescriptorLayout->getBinding( 0u )
			, m_passes.combine.views[0]
			, *m_sampler );
		m_mainDescriptorSet->update();
	}

	void RenderPanel::doCreateMainRenderPass()
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
				std::nullopt,
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
		m_mainRenderPass = m_device->getDevice().createRenderPass( std::move( createInfo ) );
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

	void RenderPanel::doPrepareHiPass()
	{
		auto dimensions = m_swapChain->getDimensions();
		ashes::VkDescriptorSetLayoutBindingArray bindings
		{
			{ 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
		};
		m_passes.hi.descriptorLayout = m_device->getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_passes.hi.descriptorPool = m_passes.hi.descriptorLayout->createPool( 1u );
		m_passes.hi.descriptorSet = m_passes.hi.descriptorPool->createDescriptorSet();
		m_passes.hi.descriptorSet->createBinding( m_passes.hi.descriptorLayout->getBinding( 0u )
			, m_renderTargetColourView
			, *m_sampler );
		m_passes.hi.descriptorSet->update();
		m_passes.hi.semaphore = m_device->getDevice().createSemaphore();
		m_passes.hi.pipelineLayout = m_device->getDevice().createPipelineLayout( *m_passes.hi.descriptorLayout );

		m_passes.hi.image = m_device->createImage( ashes::ImageCreateInfo
			{
				0u,
				VK_IMAGE_TYPE_2D,
				m_renderTargetColourView->format,
				VkExtent3D{ dimensions.width, dimensions.height, 1u },
				uint32_t( m_passes.blurX.size() ),
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				( VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
					| VK_IMAGE_USAGE_SAMPLED_BIT
					| VK_IMAGE_USAGE_TRANSFER_DST_BIT
					| VK_IMAGE_USAGE_TRANSFER_SRC_BIT ),
			}, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );

		ashes::ImageViewCreateInfo view
		{
			0u,
			*m_passes.hi.image,
			VK_IMAGE_VIEW_TYPE_2D,
			m_passes.hi.image->getFormat(),
			{},
			{ VK_IMAGE_ASPECT_COLOR_BIT, 0u, 1u, 0u, 1u },
		};

		for ( auto i = 0u; i < m_passes.blurX.size(); ++i )
		{
			view->subresourceRange.baseMipLevel = uint32_t( i );
			m_passes.hi.views.emplace_back( m_passes.hi.image->createView( view ) );
		}

		m_passes.hi.renderPass = createRenderPass( *m_device
			, m_passes.hi.image->getFormat()
			, VK_IMAGE_LAYOUT_UNDEFINED
			, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

		ashes::ImageViewCRefArray views
		{
			m_passes.hi.views[0]
		};
		m_passes.hi.frameBuffer = m_passes.hi.renderPass->createFrameBuffer( dimensions
			, std::move( views ) );

		std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "hipass.vert" )
			|| !wxFileExists( shadersFolder / "hipass.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		ashes::PipelineShaderStageCreateInfoArray shaderStages;
		shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
			{
				0u,
				VK_SHADER_STAGE_VERTEX_BIT,
				m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
					, VK_SHADER_STAGE_VERTEX_BIT
					, shadersFolder / "hipass.vert" ) ),
				"main",
				std::nullopt,
			} );
		shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
			{
				0u,
				VK_SHADER_STAGE_FRAGMENT_BIT,
				m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
					, VK_SHADER_STAGE_FRAGMENT_BIT
					, shadersFolder / "hipass.frag" ) ),
				"main",
				std::nullopt,
			} );
		
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

		m_passes.hi.pipeline = m_device->getDevice().createPipeline( ashes::GraphicsPipelineCreateInfo
			{
				0u,
				std::move( shaderStages ),
				std::move( vertexLayout ),
				ashes::PipelineInputAssemblyStateCreateInfo{ 0u, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP },
				std::nullopt,
				ashes::PipelineViewportStateCreateInfo
				{
					0u,
					1u,
					{ VkViewport{ 0.0f, 0.0f, float( dimensions.width ), float( dimensions.height ), 0.0f, 1.0f } },
					1u,
					{ VkRect2D{ { 0, 0 }, dimensions } },
				},
				ashes::PipelineRasterizationStateCreateInfo{},
				ashes::PipelineMultisampleStateCreateInfo{},
				std::nullopt,
				ashes::PipelineColorBlendStateCreateInfo{},
				ashes::nullopt,
				*m_passes.hi.pipelineLayout,
				*m_passes.hi.renderPass,
			} );

		m_passes.hi.commandBuffer = m_commandPool->createCommandBuffer( true );
		auto & cmd = *m_passes.hi.commandBuffer;

		cmd.begin();
		cmd.beginRenderPass( *m_passes.hi.renderPass
			, *m_passes.hi.frameBuffer
			, { ashes::makeClearValue( VkClearColorValue{ 0.0, 0.0, 0.0, 0.0 } ) }
			, VK_SUBPASS_CONTENTS_INLINE );
		cmd.bindPipeline( *m_passes.hi.pipeline );
		cmd.bindDescriptorSet( *m_passes.hi.descriptorSet, *m_passes.hi.pipelineLayout );
		cmd.bindVertexBuffer( 0u, m_mainVertexBuffer->getBuffer(), 0u );
		cmd.draw( m_mainVertexBuffer->getCount() );
		cmd.endRenderPass();
		m_passes.hi.image->generateMipmaps( cmd );
		cmd.end();
	}

	void RenderPanel::doPrepareBlurXPass()
	{
		auto dimensions = m_swapChain->getDimensions();
		m_blurConfigurationUbo = utils::makeUniformBuffer( *m_device
			, uint32_t( m_passes.blurX.size() )
			, uint32_t( sizeof( Configuration ) )
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
		m_blurConfigurationData.resize( m_passes.blurX.size() );
		m_blurDirectionUbo = utils::makeUniformBuffer( *m_device
			, 2u
			, uint32_t( sizeof( int ) )
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
		m_blurDirectionData[0] = 0;
		m_blurDirectionData[1] = 1;

		if ( auto buffer = m_blurDirectionUbo->getBuffer().lock( 0u, VK_WHOLE_SIZE, 0u ) )
		{
			for ( auto & data : m_blurDirectionData )
			{
				std::memcpy( buffer, &data, sizeof( int ) );
				buffer += m_blurDirectionUbo->getAlignedSize();
			}

			m_blurDirectionUbo->getBuffer().flush( 0u, VK_WHOLE_SIZE );
			m_blurDirectionUbo->getBuffer().unlock();
		}

		auto coefficientsCount = 5u;
		auto kernel = doCreateKernel( coefficientsCount );

		for ( auto i = 0u; i < uint32_t( m_passes.blurX.size() ); ++i )
		{
			auto & data = m_blurConfigurationData[i];
			data.textureSize = { dimensions.width >> i, dimensions.height >> i };
			data.coefficientsCount = coefficientsCount;
			data.coefficients = kernel;

			m_blurSamplers[i] = m_device->getDevice().createSampler( VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER
				, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER
				, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER
				, VK_FILTER_NEAREST
				, VK_FILTER_NEAREST
				, VK_SAMPLER_MIPMAP_MODE_NEAREST
				, float( i )
				, float( i + 1u ) );
		}

		if ( auto buffer = m_blurConfigurationUbo->getBuffer().lock( 0u, VK_WHOLE_SIZE, 0u ) )
		{
			for ( auto & data : m_blurConfigurationData )
			{
				std::memcpy( buffer, &data, sizeof( int ) );
				buffer += m_blurConfigurationUbo->getAlignedSize();
			}

			m_blurConfigurationUbo->getBuffer().flush( 0u, VK_WHOLE_SIZE );
			m_blurConfigurationUbo->getBuffer().unlock();
		}

		m_passes.blurX[0].image = m_device->createImage( ashes::ImageCreateInfo
			{
				0u,
				VK_IMAGE_TYPE_2D,
				m_renderTargetColourView->format,
				VkExtent3D{ dimensions.width, dimensions.height, 1u },
				uint32_t( m_passes.blurX.size() ),
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				( VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
					| VK_IMAGE_USAGE_SAMPLED_BIT
					| VK_IMAGE_USAGE_TRANSFER_DST_BIT
					| VK_IMAGE_USAGE_TRANSFER_SRC_BIT ),
			}, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );

		m_passes.blurX[0].renderPass = createRenderPass( *m_device
			, m_passes.blurX[0].image->getFormat()
			, VK_IMAGE_LAYOUT_UNDEFINED
			, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

		ashes::ImageViewCreateInfo view
		{
			0u,
			*m_passes.blurX[0].image,
			VK_IMAGE_VIEW_TYPE_2D,
			m_passes.blurX[0].image->getFormat(),
			{},
			{ VK_IMAGE_ASPECT_COLOR_BIT,0u, 1u, 0u, 1u },
		};

		for ( uint32_t i = 0u; i < m_passes.blurX.size(); ++i )
		{
			auto & blur = m_passes.blurX[i];
			ashes::VkDescriptorSetLayoutBindingArray bindings
			{
				{ 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
				{ 1u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
				{ 2u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			};
			blur.descriptorLayout = m_device->getDevice().createDescriptorSetLayout( std::move( bindings ) );
			blur.descriptorPool = blur.descriptorLayout->createPool( 1u );
			blur.descriptorSet = blur.descriptorPool->createDescriptorSet();
			blur.descriptorSet->createBinding( blur.descriptorLayout->getBinding( 0u )
				, m_passes.hi.views[i]
				, *m_blurSamplers[i] );
			blur.descriptorSet->createBinding( blur.descriptorLayout->getBinding( 1u )
				, *m_blurConfigurationUbo
				, i
				, 1u );
			blur.descriptorSet->createBinding( blur.descriptorLayout->getBinding( 2u )
				, *m_blurDirectionUbo
				, 0u
				, 1u );
			blur.descriptorSet->update();
			blur.semaphore = m_device->getDevice().createSemaphore();
			blur.pipelineLayout = m_device->getDevice().createPipelineLayout( *blur.descriptorLayout );
			
			view->subresourceRange.baseMipLevel = i;
			blur.views.emplace_back( m_passes.blurX[0].image->createView( view ) );

			ashes::ImageViewCRefArray views
			{
				blur.views.back()
			};
			blur.frameBuffer = m_passes.blurX[0].renderPass->createFrameBuffer( dimensions
				, std::move( views ) );

			std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders";

			if ( !wxFileExists( shadersFolder / "blur.vert" )
				|| !wxFileExists( shadersFolder / "blur.frag" ) )
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
					m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
						, VK_SHADER_STAGE_VERTEX_BIT
						, shadersFolder / "blur.vert" ) ),
					"main",
					std::nullopt,
				} );
			shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
				{
					0u,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
						, VK_SHADER_STAGE_FRAGMENT_BIT
						, shadersFolder / "blur.frag" ) ),
					"main",
					std::nullopt,
				} );

			blur.pipeline = m_device->getDevice().createPipeline( ashes::GraphicsPipelineCreateInfo
				{
					0u,
					std::move( shaderStages ),
					std::move( vertexLayout ),
					ashes::PipelineInputAssemblyStateCreateInfo{ 0u, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP },
					std::nullopt,
					ashes::PipelineViewportStateCreateInfo
					{
						0u,
						1u,
						{ VkViewport{ 0.0f, 0.0f, float( dimensions.width ), float( dimensions.height ), 0.0f, 1.0f } },
						1u,
						{ VkRect2D{ { 0, 0 }, dimensions } },
					},
					ashes::PipelineRasterizationStateCreateInfo{},
					ashes::PipelineMultisampleStateCreateInfo{},
					std::nullopt,
					ashes::PipelineColorBlendStateCreateInfo{},
					std::nullopt,
					*blur.pipelineLayout,
					*m_passes.blurX[0].renderPass,
				} );

			blur.commandBuffer = m_commandPool->createCommandBuffer( true );
			auto & cmd = *blur.commandBuffer;

			cmd.begin();
			cmd.beginRenderPass( *m_passes.blurX[0].renderPass
				, *blur.frameBuffer
				, { ashes::makeClearValue( VkClearColorValue{ 0.0, 0.0, 0.0, 0.0 } ) }
				, VK_SUBPASS_CONTENTS_INLINE );
			cmd.bindPipeline( *blur.pipeline );
			cmd.bindDescriptorSet( *blur.descriptorSet, *blur.pipelineLayout );
			cmd.bindVertexBuffer( 0u, m_mainVertexBuffer->getBuffer(), 0u );
			cmd.draw( m_mainVertexBuffer->getCount() );
			cmd.endRenderPass();
			cmd.end();

			dimensions.width >>= 1;
			dimensions.height >>= 1;
		}
	}

	void RenderPanel::doPrepareBlurYPass()
	{
		auto dimensions = m_swapChain->getDimensions();
		m_passes.blurY[0].renderPass = createRenderPass( *m_device
			, m_passes.hi.views[0]->format
			, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

		for ( auto i = 0u; i < m_passes.blurY.size(); ++i )
		{
			auto & blur = m_passes.blurY[i];
			ashes::VkDescriptorSetLayoutBindingArray bindings
			{
				{ 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
				{ 1u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
				{ 2u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			};
			blur.descriptorLayout = m_device->getDevice().createDescriptorSetLayout( std::move( bindings ) );
			blur.descriptorPool = blur.descriptorLayout->createPool( 1u );
			blur.descriptorSet = blur.descriptorPool->createDescriptorSet();
			blur.descriptorSet->createBinding( blur.descriptorLayout->getBinding( 0u )
				, m_passes.blurX[i].views[0]
				, *m_blurSamplers[i] );
			blur.descriptorSet->createBinding( blur.descriptorLayout->getBinding( 1u )
				, *m_blurConfigurationUbo
				, i
				, 1u );
			blur.descriptorSet->createBinding( blur.descriptorLayout->getBinding( 2u )
				, *m_blurDirectionUbo
				, 1u
				, 1u );
			blur.descriptorSet->update();
			blur.semaphore = m_device->getDevice().createSemaphore();
			blur.pipelineLayout = m_device->getDevice().createPipelineLayout( *blur.descriptorLayout );

			ashes::ImageViewCRefArray attaches
			{
				m_passes.hi.views[i]
			};
			blur.frameBuffer = m_passes.blurY[0].renderPass->createFrameBuffer( dimensions
				, std::move( attaches ) );

			std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders";

			if ( !wxFileExists( shadersFolder / "blur.vert" )
				|| !wxFileExists( shadersFolder / "blur.frag" ) )
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
					m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
						, VK_SHADER_STAGE_VERTEX_BIT
						, shadersFolder / "blur.vert" ) ),
					"main",
					std::nullopt,
				} );
			shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
				{
					0u,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
						, VK_SHADER_STAGE_FRAGMENT_BIT
						, shadersFolder / "blur.frag" ) ),
					"main",
					std::nullopt,
				} );

			blur.pipeline = m_device->getDevice().createPipeline( ashes::GraphicsPipelineCreateInfo
				{
					0u,
					std::move( shaderStages ),
					std::move( vertexLayout ),
					ashes::PipelineInputAssemblyStateCreateInfo{ 0u, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP },
					std::nullopt,
					ashes::PipelineViewportStateCreateInfo
					{
						0u,
						1u,
						{ VkViewport{ 0.0f, 0.0f, float( dimensions.width ), float( dimensions.height ), 0.0f, 1.0f } },
						1u,
						{ VkRect2D{ { 0, 0 }, dimensions } },
					},
					ashes::PipelineRasterizationStateCreateInfo{},
					ashes::PipelineMultisampleStateCreateInfo{},
					std::nullopt,
					ashes::PipelineColorBlendStateCreateInfo{},
					std::nullopt,
					*blur.pipelineLayout,
					*m_passes.blurY[0].renderPass,
				} );

			blur.commandBuffer = m_commandPool->createCommandBuffer( true );
			auto & cmd = *blur.commandBuffer;

			cmd.begin();
			cmd.beginRenderPass( *m_passes.blurY[0].renderPass
				, *blur.frameBuffer
				, { ashes::makeClearValue( VkClearColorValue{ 0.0, 0.0, 0.0, 0.0 } ) }
				, VK_SUBPASS_CONTENTS_INLINE );
			cmd.bindPipeline( *blur.pipeline );
			cmd.bindDescriptorSet( *blur.descriptorSet, *blur.pipelineLayout );
			cmd.bindVertexBuffer( 0u, m_mainVertexBuffer->getBuffer(), 0u );
			cmd.draw( m_mainVertexBuffer->getCount() );
			cmd.endRenderPass();
			cmd.end();

			dimensions.width >>= 1;
			dimensions.height >>= 1;
		}
	}

	void RenderPanel::doPrepareCombinePass()
	{
		m_blurMipView = m_passes.hi.image->createView( ashes::ImageViewCreateInfo
			{
				0u,
				*m_passes.hi.image,
				VK_IMAGE_VIEW_TYPE_2D,
				m_renderTargetColourView->format,
				{},
				{ VK_IMAGE_ASPECT_COLOR_BIT, 0u, uint32_t( m_passes.blurY.size() ), 0u, 1u },
			} );

		m_mipSampler = m_device->getDevice().createSampler( VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_FILTER_LINEAR
			, VK_FILTER_LINEAR
			, VK_SAMPLER_MIPMAP_MODE_NEAREST
			, 0.0f
			, float( m_passes.blurY.size() ) );

		auto dimensions = m_swapChain->getDimensions();
		ashes::VkDescriptorSetLayoutBindingArray bindings
		{
			{ 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			{ 1u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
		};

		m_passes.combine.descriptorLayout = m_device->getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_passes.combine.descriptorPool = m_passes.combine.descriptorLayout->createPool( 1u );
		m_passes.combine.descriptorSet = m_passes.combine.descriptorPool->createDescriptorSet();
		m_passes.combine.descriptorSet->createBinding( m_passes.combine.descriptorLayout->getBinding( 0u )
			, m_renderTargetColourView
			, *m_sampler );
		m_passes.combine.descriptorSet->createBinding( m_passes.combine.descriptorLayout->getBinding( 1u )
			, m_blurMipView
			, *m_mipSampler );

		m_passes.combine.descriptorSet->update();
		m_passes.combine.semaphore = m_device->getDevice().createSemaphore();
		m_passes.combine.pipelineLayout = m_device->getDevice().createPipelineLayout( *m_passes.combine.descriptorLayout );

		m_passes.combine.image = m_device->createImage( ashes::ImageCreateInfo
			{
				0u,
				VK_IMAGE_TYPE_2D,
				m_renderTargetColourView->format,
				VkExtent3D{ dimensions.width, dimensions.height, 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				( VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
				| VK_IMAGE_USAGE_SAMPLED_BIT
					| VK_IMAGE_USAGE_TRANSFER_DST_BIT
					| VK_IMAGE_USAGE_TRANSFER_SRC_BIT ),
			}, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_passes.combine.views.emplace_back( m_passes.combine.image->createView( ashes::ImageViewCreateInfo
			{
				0u,
				*m_passes.combine.image,
				VK_IMAGE_VIEW_TYPE_2D,
				m_passes.combine.image->getFormat(),
				{},
				{ VK_IMAGE_ASPECT_COLOR_BIT, 0u, 1u, 0u, 1u },
			} ) );
		m_passes.combine.renderPass = createRenderPass( *m_device
			, m_passes.combine.views[0]->format
			, VK_IMAGE_LAYOUT_UNDEFINED
			, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

		ashes::ImageViewCRefArray attaches
		{
			m_passes.combine.views[0]
		};
		m_passes.combine.frameBuffer = m_passes.combine.renderPass->createFrameBuffer( dimensions
			, std::move( attaches ) );

		std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "combine.vert" )
			|| !wxFileExists( shadersFolder / "combine.frag" ) )
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
				m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
					, VK_SHADER_STAGE_VERTEX_BIT
					, shadersFolder / "combine.vert" ) ),
				"main",
				std::nullopt,
			} );
		shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
			{
				0u,
				VK_SHADER_STAGE_FRAGMENT_BIT,
				m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
					, VK_SHADER_STAGE_FRAGMENT_BIT
					, shadersFolder / "combine.frag" ) ),
				"main",
				std::nullopt,
			} );

		m_passes.combine.pipeline = m_device->getDevice().createPipeline( ashes::GraphicsPipelineCreateInfo
			{
				0u,
				std::move( shaderStages ),
				std::move( vertexLayout ),
				ashes::PipelineInputAssemblyStateCreateInfo{ 0u, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP },
				std::nullopt,
				ashes::PipelineViewportStateCreateInfo
				{
					0u,
					1u,
					{ VkViewport{ 0.0f, 0.0f, float( dimensions.width ), float( dimensions.height ), 0.0f, 1.0f } },
					1u,
					{ VkRect2D{ { 0, 0 }, dimensions } },
				},
				ashes::PipelineRasterizationStateCreateInfo{},
				ashes::PipelineMultisampleStateCreateInfo{},
				std::nullopt,
				ashes::PipelineColorBlendStateCreateInfo{},
				std::nullopt,
				*m_passes.combine.pipelineLayout,
				*m_passes.combine.renderPass,
			} );

		m_passes.combine.commandBuffer = m_commandPool->createCommandBuffer( true );
		auto & cmd = *m_passes.combine.commandBuffer;

		cmd.begin();
		cmd.beginRenderPass( *m_passes.combine.renderPass
			, *m_passes.combine.frameBuffer
			, { ashes::makeClearValue( VkClearColorValue{ 0.0, 0.0, 0.0, 0.0 } ) }
			, VK_SUBPASS_CONTENTS_INLINE );
		cmd.bindPipeline( *m_passes.combine.pipeline );
		cmd.bindDescriptorSet( *m_passes.combine.descriptorSet, *m_passes.combine.pipelineLayout );
		cmd.bindVertexBuffer( 0u, m_mainVertexBuffer->getBuffer(), 0u );
		cmd.draw( m_mainVertexBuffer->getCount() );
		cmd.endRenderPass();
		cmd.end();
	}

	void RenderPanel::doCreateMainVertexBuffer()
	{
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
				m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
					, VK_SHADER_STAGE_VERTEX_BIT
					, shadersFolder / "main.vert" ) ),
				"main",
				std::nullopt,
			} );
		shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
			{
				0u,
				VK_SHADER_STAGE_FRAGMENT_BIT,
				m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
					, VK_SHADER_STAGE_FRAGMENT_BIT
					, shadersFolder / "main.frag" ) ),
				"main",
				std::nullopt,
			} );

		m_mainPipeline = m_device->getDevice().createPipeline( ashes::GraphicsPipelineCreateInfo
			{
				0u,
				std::move( shaderStages ),
				std::move( vertexLayout ),
				ashes::PipelineInputAssemblyStateCreateInfo{ 0u, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP },
				std::nullopt,
				ashes::PipelineViewportStateCreateInfo{},
				ashes::PipelineRasterizationStateCreateInfo{},
				ashes::PipelineMultisampleStateCreateInfo{},
				std::nullopt,
				ashes::PipelineColorBlendStateCreateInfo{},
				ashes::PipelineDynamicStateCreateInfo{ 0u, { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR } },
				*m_mainPipelineLayout,
				*m_mainRenderPass,
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
		static std::chrono::high_resolution_clock::time_point save = std::chrono::high_resolution_clock::now();
		static utils::Mat4 const originalRotate = []()
		{
			utils::Mat4 result;
			result = utils::rotate( result
				, float( utils::DegreeToRadian * 45.0 )
				, { 0, 0, 1 } );
			return result;
		}();
		auto duration = std::chrono::high_resolution_clock::now() - save;;
		auto factor = std::chrono::duration_cast< std::chrono::milliseconds >( duration ).count() / 1000.0f;
		m_rotate = utils::rotate( m_rotate
			, float( utils::DegreeToRadian ) * factor
			, { 0, 1.0f, 0 } );
		m_objectData = m_rotate * originalRotate;
		m_stagingBuffer->uploadUniformData( *m_graphicsQueue
			, *m_commandPool
			, &m_objectData
			, 1u
			, *m_objectUbo
			, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT );
		save = std::chrono::high_resolution_clock::now();
	}

	void RenderPanel::doDraw()
	{
		auto resources = m_swapChain->getResources();

		if ( resources )
		{
			auto before = std::chrono::high_resolution_clock::now();
			m_graphicsQueue->submit( *m_commandBuffer
				, nullptr );
			m_graphicsQueue->submit( *m_passes.hi.commandBuffer
				, nullptr );

			for ( auto & blur : m_passes.blurX )
			{
				m_graphicsQueue->submit( *blur.commandBuffer
					, nullptr );
			}

			for ( auto & blur : m_passes.blurY )
			{
				m_graphicsQueue->submit( *blur.commandBuffer
					, nullptr );
			}

			m_graphicsQueue->submit( *m_passes.combine.commandBuffer
				, nullptr );

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
