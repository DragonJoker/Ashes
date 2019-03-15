#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <Buffer/StagingBuffer.hpp>
#include <Buffer/UniformBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <Command/Queue.hpp>
#include <Core/BackBuffer.hpp>
#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Core/Instance.hpp>
#include <Core/SwapChain.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/StagingTexture.hpp>
#include <Image/Texture.hpp>
#include <Image/TextureView.hpp>
#include <Miscellaneous/QueryPool.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderPassCreateInfo.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <GlslToSpv.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <Transform.hpp>

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
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, ashes::Instance const & instance )
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
			doCreateDevice( instance, std::move( surface ) );
			std::cout << "Logical device created." << std::endl;
			doCreateSwapChain();
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
			m_device->waitIdle();

			m_blurMipView.reset();
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
			m_blurConfiguration.reset();
			m_blurDirection.reset();

			for ( auto & sampler : m_blurSamplers )
			{
				sampler.reset();
			}

			m_commandBuffer.reset();
			m_commandBuffers.clear();
			m_frameBuffers.clear();
			m_sampler.reset();
			m_view.reset();
			m_texture.reset();
			m_stagingBuffer.reset();
			m_stagingTexture.reset();

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
			m_renderTargetColourView.reset();
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
		m_matrixUbo->getData( 0u ) = utils::Mat4{ m_device->perspective( float( utils::toRadians( 90.0_degrees ) / ratio )
			, width / height
			, 0.01f
			, 100.0f ) };
		m_stagingBuffer->uploadUniformData( *m_graphicsQueue
			, *m_commandPool
			, m_matrixUbo->getDatas()
			, *m_matrixUbo
			, ashes::PipelineStageFlag::eVertexShader );
	}

	ashes::ConnectionPtr RenderPanel::doCreateSurface( ashes::Instance const & instance )
	{
		auto handle = common::makeWindowHandle( *this );
		auto & gpu = instance.getPhysicalDevice( 0u );
		return instance.createConnection( gpu
			, std::move( handle ) );
	}

	void RenderPanel::doInitialiseQueues( ashes::Instance const & instance
		, ashes::Connection const & surface )
	{
		auto & gpu = instance.getPhysicalDevice( 0u );
		std::vector< bool > supportsPresent( static_cast< uint32_t >( gpu.getQueueProperties().size() ) );
		uint32_t i{ 0u };
		m_graphicsQueueFamilyIndex = std::numeric_limits< uint32_t >::max();
		m_presentQueueFamilyIndex = std::numeric_limits< uint32_t >::max();

		for ( auto & present : supportsPresent )
		{
			auto present = surface.getSurfaceSupport( i );

			if ( gpu.getQueueProperties()[i].queueCount > 0 )
			{
				if ( gpu.getQueueProperties()[i].queueFlags & ashes::QueueFlag::eGraphics )
				{
					if ( m_graphicsQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
					{
						m_graphicsQueueFamilyIndex = i;
					}

					if ( present )
					{
						m_graphicsQueueFamilyIndex = i;
						m_presentQueueFamilyIndex = i;
						break;
					}
				}
			}

			++i;
		}

		if ( m_presentQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
		{
			for ( size_t i = 0; i < gpu.getQueueProperties().size(); ++i )
			{
				if ( supportsPresent[i] )
				{
					m_presentQueueFamilyIndex = static_cast< uint32_t >( i );
					break;
				}
			}
		}

		if ( m_graphicsQueueFamilyIndex == std::numeric_limits< uint32_t >::max()
			|| m_presentQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
		{
			throw ashes::Exception{ ashes::Result::eErrorInitializationFailed
				, "Queue families retrieval" };
		}
	}

	void RenderPanel::doCreateDevice( ashes::Instance const & instance
		, ashes::ConnectionPtr surface )
	{
		doInitialiseQueues( instance, *surface );
		m_device = instance.createDevice( std::move( surface )
			, m_graphicsQueueFamilyIndex
			, m_presentQueueFamilyIndex );
		m_graphicsQueue = m_device->getQueue( m_graphicsQueueFamilyIndex, 0u );
		m_presentQueue = m_device->getQueue( m_presentQueueFamilyIndex, 0u );
		m_commandPool = m_device->createCommandPool( m_graphicsQueueFamilyIndex
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );
	}

	void RenderPanel::doCreateSwapChain()
	{
		wxSize size{ GetClientSize() };
		m_swapChain = m_device->createSwapChain( *m_commandPool
			, { uint32_t( size.x ), uint32_t( size.y ) } );
		m_swapChain->setClearColour( { 1.0f, 0.8f, 0.4f, 0.0f } );
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
		m_stagingTexture = m_device->createStagingTexture( ashes::Format::eR8G8B8A8_UNORM
			, { 512u, 512u } );
		m_texture = m_device->createTexture(
			{
				ashes::ImageCreateFlag::eCubeCompatible,
				ashes::TextureType::e2D,
				ashes::Format::eR8G8B8A8_UNORM,
				{ 512u, 512u, 1u },
				1u,
				6u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eTransferDst | ashes::ImageUsageFlag::eSampled
			}
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_view = m_texture->createView( ashes::TextureViewType::eCube
			, ashes::Format::eR8G8B8A8_UNORM
			, 0u
			, 1u
			, 0u
			, 6u );
		m_sampler = m_device->createSampler( ashes::WrapMode::eClampToEdge
			, ashes::WrapMode::eClampToEdge
			, ashes::WrapMode::eClampToEdge
			, ashes::Filter::eLinear
			, ashes::Filter::eLinear );

		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / "Assets";
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
			m_stagingTexture->uploadTextureData( *m_graphicsQueue
				, *m_commandPool
				, {
					m_view->getSubResourceRange().aspectMask,
					m_view->getSubResourceRange().baseMipLevel,
					uint32_t( i ),
					1u,
				}
				, image.format
				, { 0, 0 }
				, { image.size.width, image.size.height }
				, image.data
				, *m_view );
		}
	}

	void RenderPanel::doCreateUniformBuffer()
	{
		m_matrixUbo = std::make_unique< ashes::UniformBuffer< utils::Mat4 > >( *m_device
			, 1u
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_objectUbo = std::make_unique< ashes::UniformBuffer< utils::Mat4 > >( *m_device
			, 1u
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
	}

	void RenderPanel::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique< ashes::StagingBuffer >( *m_device
			, 0u
			, 20000000u );
	}

	void RenderPanel::doCreateOffscreenDescriptorSet()
	{
		std::vector< ashes::DescriptorSetLayoutBinding > bindings
		{
			ashes::DescriptorSetLayoutBinding{ 0u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
			ashes::DescriptorSetLayoutBinding{ 1u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eVertex },
			ashes::DescriptorSetLayoutBinding{ 2u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eVertex },
		};
		m_offscreenDescriptorLayout = m_device->createDescriptorSetLayout( std::move( bindings ) );
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
		ashes::AttachmentDescriptionArray attaches
		{
			{
				ashes::Format::eR8G8B8A8_UNORM,
				ashes::SampleCountFlag::e1,
				ashes::AttachmentLoadOp::eClear,
				ashes::AttachmentStoreOp::eStore,
				ashes::AttachmentLoadOp::eDontCare,
				ashes::AttachmentStoreOp::eDontCare,
				ashes::ImageLayout::eUndefined,
				ashes::ImageLayout::eShaderReadOnlyOptimal,
			}
		};
		ashes::AttachmentReferenceArray subAttaches
		{
			{ 0u, ashes::ImageLayout::eColourAttachmentOptimal }
		};
		ashes::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( std::make_unique< ashes::RenderSubpass >( ashes::PipelineBindPoint::eGraphics
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eColourAttachmentOutput
				, ashes::AccessFlag::eColourAttachmentWrite }
			, subAttaches ) );
		m_offscreenRenderPass = m_device->createRenderPass( attaches
			, std::move( subpasses )
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eColourAttachmentOutput
				, ashes::AccessFlag::eColourAttachmentWrite }
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eColourAttachmentOutput
				, ashes::AccessFlag::eShaderRead } );
	}

	void RenderPanel::doCreateFrameBuffer()
	{
		auto size = GetClientSize();
		m_renderTargetColour = m_device->createTexture(
			{
				0u,
				ashes::TextureType::e2D,
				ashes::Format::eR8G8B8A8_UNORM,
				{ uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ), 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eColourAttachment | ashes::ImageUsageFlag::eSampled
			}
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_renderTargetColourView = m_renderTargetColour->createView( ashes::TextureViewType::e2D
			, m_renderTargetColour->getFormat() );
		ashes::FrameBufferAttachmentArray attaches;
		attaches.emplace_back( *( m_offscreenRenderPass->getAttachments().begin() + 0u ), *m_renderTargetColourView );
		m_frameBuffer = m_offscreenRenderPass->createFrameBuffer( { uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ) }
			, std::move( attaches ) );
	}

	void RenderPanel::doCreateOffscreenVertexBuffer()
	{
		m_offscreenVertexLayout = ashes::makeLayout< NonTexturedVertexData >( 0 );
		m_offscreenVertexLayout->createAttribute( 0u
			, ashes::Format::eR32G32B32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, position ) ) );

		m_offscreenVertexBuffer = ashes::makeVertexBuffer< NonTexturedVertexData >( *m_device
			, uint32_t( m_offscreenVertexData.size() )
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_stagingBuffer->uploadVertexData( *m_graphicsQueue
			, *m_commandPool
			, m_offscreenVertexData
			, *m_offscreenVertexBuffer );

		m_offscreenIndexBuffer = ashes::makeBuffer< uint16_t >( *m_device
			, uint32_t( m_offscreenIndexData.size() )
			, ashes::BufferTarget::eIndexBuffer | ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_stagingBuffer->uploadBufferData( *m_graphicsQueue
			, *m_commandPool
			, m_offscreenIndexData
			, *m_offscreenIndexBuffer );
	}

	void RenderPanel::doCreateOffscreenPipeline()
	{
		m_semaphore = m_device->createSemaphore();
		m_offscreenPipelineLayout = m_device->createPipelineLayout( *m_offscreenDescriptorLayout );
		wxSize size{ GetClientSize() };
		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "offscreen.vert" )
			|| !wxFileExists( shadersFolder / "offscreen.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		std::vector< ashes::ShaderStageState > shaderStages;
		shaderStages.push_back( { m_device->createShaderModule( ashes::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device->createShaderModule( ashes::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( common::parseShaderFile( *m_device
			, ashes::ShaderStageFlag::eVertex
			, shadersFolder / "offscreen.vert" ) );
		shaderStages[1].module->loadShader( common::parseShaderFile( *m_device
			, ashes::ShaderStageFlag::eFragment
			, shadersFolder / "offscreen.frag" ) );

		ashes::RasterisationState rasterisationState;
		rasterisationState.cullMode = ashes::CullModeFlag::eNone;

		m_offscreenPipeline = m_offscreenPipelineLayout->createPipeline( ashes::GraphicsPipelineCreateInfo
		{
			std::move( shaderStages ),
			*m_offscreenRenderPass,
			ashes::VertexInputState::create( *m_offscreenVertexLayout ),
			ashes::InputAssemblyState{ ashes::PrimitiveTopology::eTriangleList },
			rasterisationState,
			ashes::MultisampleState{},
			ashes::ColourBlendState::createDefault(),
			{ ashes::DynamicStateEnable::eViewport, ashes::DynamicStateEnable::eScissor }
		} );
	}

	void RenderPanel::doCreateMainDescriptorSet()
	{
		std::vector< ashes::DescriptorSetLayoutBinding > bindings
		{
			ashes::DescriptorSetLayoutBinding{ 0u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
		};
		m_mainDescriptorLayout = m_device->createDescriptorSetLayout( std::move( bindings ) );
		m_mainDescriptorPool = m_mainDescriptorLayout->createPool( 1u );
		m_mainDescriptorSet = m_mainDescriptorPool->createDescriptorSet();
		m_mainDescriptorSet->createBinding( m_mainDescriptorLayout->getBinding( 0u )
			, *m_passes.combine.views[0]
			, *m_sampler );
		m_mainDescriptorSet->update();
	}

	void RenderPanel::doCreateMainRenderPass()
	{
		ashes::AttachmentDescriptionArray attaches
		{
			{
				m_swapChain->getFormat(),
				ashes::SampleCountFlag::e1,
				ashes::AttachmentLoadOp::eClear,
				ashes::AttachmentStoreOp::eStore,
				ashes::AttachmentLoadOp::eDontCare,
				ashes::AttachmentStoreOp::eDontCare,
				ashes::ImageLayout::eUndefined,
				ashes::ImageLayout::ePresentSrc,
			}
		};
		ashes::AttachmentReferenceArray subAttaches
		{
			{ 0u, ashes::ImageLayout::eColourAttachmentOptimal }
		};
		ashes::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( std::make_unique< ashes::RenderSubpass >( ashes::PipelineBindPoint::eGraphics
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eColourAttachmentOutput
				, ashes::AccessFlag::eColourAttachmentWrite }
			, subAttaches ) );
		m_mainRenderPass = m_device->createRenderPass( attaches
			, std::move( subpasses )
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eBottomOfPipe
				, ashes::AccessFlag::eMemoryRead }
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eBottomOfPipe
				, ashes::AccessFlag::eMemoryRead } );
	}

	void RenderPanel::doPrepareOffscreenFrame()
	{
		doUpdateProjection();
		m_queryPool = m_device->createQueryPool( ashes::QueryType::eTimestamp
			, 2u
			, 0u );
		m_commandBuffer = m_commandPool->createCommandBuffer();
		wxSize size{ GetClientSize() };
		auto & commandBuffer = *m_commandBuffer;
		auto & frameBuffer = *m_frameBuffer;

		commandBuffer.begin( ashes::CommandBufferUsageFlag::eSimultaneousUse );
		auto dimensions = m_swapChain->getDimensions();
		commandBuffer.resetQueryPool( *m_queryPool
			, 0u
			, 2u );
		commandBuffer.beginRenderPass( *m_offscreenRenderPass
			, frameBuffer
			, { ashes::ClearValue{ m_swapChain->getClearColour() } }
			, ashes::SubpassContents::eInline );
		commandBuffer.writeTimestamp( ashes::PipelineStageFlag::eTopOfPipe
			, *m_queryPool
			, 0u );
		commandBuffer.bindPipeline( *m_offscreenPipeline );
		commandBuffer.setViewport( { dimensions.width
			, dimensions.height
			, 0
			, 0 } );
		commandBuffer.setScissor( { 0
			, 0
			, dimensions.width
			, dimensions.height } );
		commandBuffer.bindVertexBuffer( 0u, m_offscreenVertexBuffer->getBuffer(), 0u );
		commandBuffer.bindIndexBuffer( m_offscreenIndexBuffer->getBuffer(), 0u, ashes::IndexType::eUInt16 );
		commandBuffer.bindDescriptorSet( *m_offscreenDescriptorSet
			, *m_offscreenPipelineLayout );
		commandBuffer.drawIndexed( uint32_t( m_offscreenIndexData.size() ) );
		commandBuffer.writeTimestamp( ashes::PipelineStageFlag::eBottomOfPipe
			, *m_queryPool
			, 1u );
		commandBuffer.endRenderPass();
		commandBuffer.end();
	}

	void RenderPanel::doPrepareHiPass()
	{
		auto dimensions = m_swapChain->getDimensions();
		std::vector< ashes::DescriptorSetLayoutBinding > bindings
		{
			ashes::DescriptorSetLayoutBinding{ 0u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
		};
		m_passes.hi.descriptorLayout = m_device->createDescriptorSetLayout( std::move( bindings ) );
		m_passes.hi.descriptorPool = m_passes.hi.descriptorLayout->createPool( 1u );
		m_passes.hi.descriptorSet = m_passes.hi.descriptorPool->createDescriptorSet();
		m_passes.hi.descriptorSet->createBinding( m_passes.hi.descriptorLayout->getBinding( 0u )
			, *m_renderTargetColourView
			, *m_sampler );
		m_passes.hi.descriptorSet->update();
		m_passes.hi.semaphore = m_device->createSemaphore();
		m_passes.hi.pipelineLayout = m_device->createPipelineLayout( *m_passes.hi.descriptorLayout );

		ashes::ImageCreateInfo image{};
		image.arrayLayers = 1u;
		image.extent = ashes::Extent3D{ dimensions.width, dimensions.height, 1u };
		image.flags = 0u;
		image.format = m_renderTargetColourView->getFormat();
		image.imageType = ashes::TextureType::e2D;
		image.initialLayout = ashes::ImageLayout::eUndefined;
		image.mipLevels = uint32_t( m_passes.blurX.size() );
		image.samples = ashes::SampleCountFlag::e1;
		image.tiling = ashes::ImageTiling::eOptimal;
		image.usage = ashes::ImageUsageFlag::eColourAttachment
			| ashes::ImageUsageFlag::eSampled
			| ashes::ImageUsageFlag::eTransferDst
			| ashes::ImageUsageFlag::eTransferSrc;
		m_passes.hi.image = m_device->createTexture( image, ashes::MemoryPropertyFlag::eDeviceLocal );

		ashes::ImageViewCreateInfo view{};
		view.format = image.format;
		view.viewType = ashes::TextureViewType::e2D;
		view.subresourceRange.aspectMask = ashes::ImageAspectFlag::eColour;
		view.subresourceRange.baseArrayLayer = 0u;
		view.subresourceRange.layerCount = 1u;
		view.subresourceRange.levelCount = 1u;

		for ( auto i = 0u; i < m_passes.blurX.size(); ++i )
		{
			view.subresourceRange.baseMipLevel = uint32_t( i );
			m_passes.hi.views.emplace_back( m_passes.hi.image->createView( view ) );
		}

		ashes::RenderPassCreateInfo renderPass{};
		renderPass.attachments.resize( 1u );
		renderPass.attachments[0].format = image.format;
		renderPass.attachments[0].loadOp = ashes::AttachmentLoadOp::eClear;
		renderPass.attachments[0].storeOp = ashes::AttachmentStoreOp::eStore;
		renderPass.attachments[0].stencilLoadOp = ashes::AttachmentLoadOp::eDontCare;
		renderPass.attachments[0].stencilStoreOp = ashes::AttachmentStoreOp::eDontCare;
		renderPass.attachments[0].samples = ashes::SampleCountFlag::e1;
		renderPass.attachments[0].initialLayout = ashes::ImageLayout::eUndefined;
		renderPass.attachments[0].finalLayout = ashes::ImageLayout::eColourAttachmentOptimal;

		renderPass.subpasses.resize( 1u );
		renderPass.subpasses[0].colorAttachments = { { 0u, ashes::ImageLayout::eColourAttachmentOptimal } };

		renderPass.dependencies.resize( 2u );
		renderPass.dependencies[0].srcSubpass = ashes::ExternalSubpass;
		renderPass.dependencies[0].dstSubpass = 0u;
		renderPass.dependencies[0].dependencyFlags = ashes::DependencyFlag::eByRegion;
		renderPass.dependencies[0].srcAccessMask = ashes::AccessFlag::eMemoryRead;
		renderPass.dependencies[0].dstAccessMask = ashes::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[0].srcStageMask = ashes::PipelineStageFlag::eHost;
		renderPass.dependencies[0].dstStageMask = ashes::PipelineStageFlag::eColourAttachmentOutput;

		renderPass.dependencies[1].srcSubpass = 0u;
		renderPass.dependencies[1].dstSubpass = ashes::ExternalSubpass;
		renderPass.dependencies[1].dependencyFlags = ashes::DependencyFlag::eByRegion;
		renderPass.dependencies[1].srcAccessMask = ashes::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[1].dstAccessMask = ashes::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[1].srcStageMask = ashes::PipelineStageFlag::eColourAttachmentOutput;
		renderPass.dependencies[1].dstStageMask = ashes::PipelineStageFlag::eColourAttachmentOutput;

		m_passes.hi.renderPass = m_device->createRenderPass( renderPass );

		ashes::FrameBufferAttachmentArray attaches
		{
			{ *m_passes.hi.renderPass->getAttachments().begin(), *m_passes.hi.views[0] }
		};
		m_passes.hi.frameBuffer = m_passes.hi.renderPass->createFrameBuffer( dimensions
			, std::move( attaches ) );

		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "hipass.vert" )
			|| !wxFileExists( shadersFolder / "hipass.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		ashes::ShaderStageStateArray shaderStages;
		shaderStages.push_back( { m_device->createShaderModule( ashes::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device->createShaderModule( ashes::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( common::parseShaderFile( *m_device
			, ashes::ShaderStageFlag::eVertex
			, shadersFolder / "hipass.vert" ) );
		shaderStages[1].module->loadShader( common::parseShaderFile( *m_device
			, ashes::ShaderStageFlag::eFragment
			, shadersFolder / "hipass.frag" ) );

		ashes::GraphicsPipelineCreateInfo pipeline
		{
			shaderStages,
			*m_passes.hi.renderPass,
			ashes::VertexInputState::create( *m_mainVertexLayout ),
			ashes::InputAssemblyState{ ashes::PrimitiveTopology::eTriangleStrip },
			ashes::RasterisationState{},
			ashes::MultisampleState{},
			ashes::ColourBlendState::createDefault(),
			{},
			std::nullopt,
			std::nullopt,
			ashes::Viewport{ { 0, 0 }, dimensions },
			ashes::Scissor{ { 0, 0 }, dimensions }
		};
		m_passes.hi.pipeline = m_passes.hi.pipelineLayout->createPipeline( pipeline );

		m_passes.hi.commandBuffer = m_commandPool->createCommandBuffer( true );
		auto & cmd = *m_passes.hi.commandBuffer;

		cmd.begin();
		cmd.beginRenderPass( *m_passes.hi.renderPass
			, *m_passes.hi.frameBuffer
			, { ashes::ClearColorValue{ 0.0, 0.0, 0.0, 0.0 } }
			, ashes::SubpassContents::eInline );
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
		m_blurConfiguration = ashes::makeUniformBuffer< Configuration >( *m_device
			, uint32_t( m_passes.blurX.size() )
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eHostVisible );
		m_blurDirection = ashes::makeUniformBuffer< int >( *m_device
			, 2u
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eHostVisible );
		m_blurDirection->getData( 0 ) = 0;
		m_blurDirection->getData( 1 ) = 1;
		m_blurDirection->upload( 0u, 2u );
		auto coefficientsCount = 5u;
		auto kernel = doCreateKernel( coefficientsCount );

		for ( auto i = 0u; i < uint32_t( m_passes.blurX.size() ); ++i )
		{
			auto & data = m_blurConfiguration->getData( i );
			data.textureSize = { dimensions.width >> i, dimensions.height >> i };
			data.coefficientsCount = coefficientsCount;
			data.coefficients = kernel;

			m_blurSamplers[i] = m_device->createSampler( ashes::WrapMode::eClampToBorder
				, ashes::WrapMode::eClampToBorder
				, ashes::WrapMode::eClampToBorder
				, ashes::Filter::eNearest
				, ashes::Filter::eNearest
				, ashes::MipmapMode::eNearest
				, float( i )
				, float( i + 1u ) );
		}

		m_blurConfiguration->upload( 0u, uint32_t( m_passes.blurX.size() ) );

		ashes::ImageCreateInfo image{};
		image.arrayLayers = 1u;
		image.extent = ashes::Extent3D{ dimensions.width, dimensions.height, 1u };
		image.flags = 0u;
		image.format = m_renderTargetColourView->getFormat();
		image.imageType = ashes::TextureType::e2D;
		image.initialLayout = ashes::ImageLayout::eUndefined;
		image.mipLevels = uint32_t( m_passes.blurX.size() );
		image.samples = ashes::SampleCountFlag::e1;
		image.tiling = ashes::ImageTiling::eOptimal;
		image.usage = ashes::ImageUsageFlag::eColourAttachment
			| ashes::ImageUsageFlag::eSampled
			| ashes::ImageUsageFlag::eTransferDst
			| ashes::ImageUsageFlag::eTransferSrc;
		m_passes.blurX[0].image = m_device->createTexture( image, ashes::MemoryPropertyFlag::eDeviceLocal );

		ashes::RenderPassCreateInfo renderPass{};
		renderPass.attachments.resize( 1u );
		renderPass.attachments[0].format = image.format;
		renderPass.attachments[0].loadOp = ashes::AttachmentLoadOp::eClear;
		renderPass.attachments[0].storeOp = ashes::AttachmentStoreOp::eStore;
		renderPass.attachments[0].stencilLoadOp = ashes::AttachmentLoadOp::eDontCare;
		renderPass.attachments[0].stencilStoreOp = ashes::AttachmentStoreOp::eDontCare;
		renderPass.attachments[0].samples = ashes::SampleCountFlag::e1;
		renderPass.attachments[0].initialLayout = ashes::ImageLayout::eUndefined;
		renderPass.attachments[0].finalLayout = ashes::ImageLayout::eShaderReadOnlyOptimal;

		renderPass.subpasses.resize( 1u );
		renderPass.subpasses[0].colorAttachments = { { 0u, ashes::ImageLayout::eColourAttachmentOptimal } };

		renderPass.dependencies.resize( 2u );
		renderPass.dependencies[0].srcSubpass = ashes::ExternalSubpass;
		renderPass.dependencies[0].dstSubpass = 0u;
		renderPass.dependencies[0].dependencyFlags = ashes::DependencyFlag::eByRegion;
		renderPass.dependencies[0].srcAccessMask = ashes::AccessFlag::eHostWrite;
		renderPass.dependencies[0].dstAccessMask = ashes::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[0].srcStageMask = ashes::PipelineStageFlag::eHost;
		renderPass.dependencies[0].dstStageMask = ashes::PipelineStageFlag::eColourAttachmentOutput;

		renderPass.dependencies[1].srcSubpass = 0u;
		renderPass.dependencies[1].dstSubpass = ashes::ExternalSubpass;
		renderPass.dependencies[1].dependencyFlags = ashes::DependencyFlag::eByRegion;
		renderPass.dependencies[1].srcAccessMask = ashes::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[1].dstAccessMask = ashes::AccessFlag::eShaderRead;
		renderPass.dependencies[1].srcStageMask = ashes::PipelineStageFlag::eColourAttachmentOutput;
		renderPass.dependencies[1].dstStageMask = ashes::PipelineStageFlag::eFragmentShader;

		m_passes.blurX[0].renderPass = m_device->createRenderPass( renderPass );

		for ( auto i = 0u; i < m_passes.blurX.size(); ++i )
		{
			auto & blur = m_passes.blurX[i];
			std::vector< ashes::DescriptorSetLayoutBinding > bindings
			{
				ashes::DescriptorSetLayoutBinding{ 0u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
				ashes::DescriptorSetLayoutBinding{ 1u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eFragment },
				ashes::DescriptorSetLayoutBinding{ 2u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eFragment },
			};
			blur.descriptorLayout = m_device->createDescriptorSetLayout( std::move( bindings ) );
			blur.descriptorPool = blur.descriptorLayout->createPool( 1u );
			blur.descriptorSet = blur.descriptorPool->createDescriptorSet();
			blur.descriptorSet->createBinding( blur.descriptorLayout->getBinding( 0u )
				, *m_passes.hi.views[i]
				, *m_blurSamplers[i] );
			blur.descriptorSet->createBinding( blur.descriptorLayout->getBinding( 1u )
				, *m_blurConfiguration
				, i );
			blur.descriptorSet->createBinding( blur.descriptorLayout->getBinding( 2u )
				, *m_blurDirection
				, 0u );
			blur.descriptorSet->update();
			blur.semaphore = m_device->createSemaphore();
			blur.pipelineLayout = m_device->createPipelineLayout( *blur.descriptorLayout );

			ashes::ImageViewCreateInfo view{};
			view.format = image.format;
			view.viewType = ashes::TextureViewType::e2D;
			view.subresourceRange.aspectMask = ashes::ImageAspectFlag::eColour;
			view.subresourceRange.baseArrayLayer = 0u;
			view.subresourceRange.layerCount = 1u;
			view.subresourceRange.levelCount = 1u;
			view.subresourceRange.baseMipLevel = uint32_t( i );
			blur.views.emplace_back( m_passes.blurX[0].image->createView( view ) );

			ashes::FrameBufferAttachmentArray attaches
			{
				{ *m_passes.blurX[0].renderPass->getAttachments().begin(), *blur.views.back() }
			};
			blur.frameBuffer = m_passes.blurX[0].renderPass->createFrameBuffer( dimensions
				, std::move( attaches ) );

			std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

			if ( !wxFileExists( shadersFolder / "blur.vert" )
				|| !wxFileExists( shadersFolder / "blur.frag" ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			ashes::ShaderStageStateArray shaderStages;
			shaderStages.push_back( { m_device->createShaderModule( ashes::ShaderStageFlag::eVertex ) } );
			shaderStages.push_back( { m_device->createShaderModule( ashes::ShaderStageFlag::eFragment ) } );
			shaderStages[0].module->loadShader( common::parseShaderFile( *m_device
				, ashes::ShaderStageFlag::eVertex
				, shadersFolder / "blur.vert" ) );
			shaderStages[1].module->loadShader( common::parseShaderFile( *m_device
				, ashes::ShaderStageFlag::eFragment
				, shadersFolder / "blur.frag" ) );

			ashes::GraphicsPipelineCreateInfo pipeline
			{
				shaderStages,
				*m_passes.blurX[0].renderPass,
				ashes::VertexInputState::create( *m_mainVertexLayout ),
				ashes::InputAssemblyState{ ashes::PrimitiveTopology::eTriangleStrip },
				ashes::RasterisationState{},
				ashes::MultisampleState{},
				ashes::ColourBlendState::createDefault(),
				{},
				std::nullopt,
				std::nullopt,
				ashes::Viewport{ { 0, 0 }, dimensions },
				ashes::Scissor{ { 0, 0 }, dimensions }
			};
			blur.pipeline = blur.pipelineLayout->createPipeline( pipeline );

			blur.commandBuffer = m_commandPool->createCommandBuffer( true );
			auto & cmd = *blur.commandBuffer;

			cmd.begin();
			cmd.beginRenderPass( *m_passes.blurX[0].renderPass
				, *blur.frameBuffer
				, { ashes::ClearColorValue{ 0.0, 0.0, 0.0, 0.0 } }
			, ashes::SubpassContents::eInline );
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
		ashes::RenderPassCreateInfo renderPass{};
		renderPass.attachments.resize( 1u );
		renderPass.attachments[0].format = m_passes.hi.views[0]->getFormat();
		renderPass.attachments[0].loadOp = ashes::AttachmentLoadOp::eClear;
		renderPass.attachments[0].storeOp = ashes::AttachmentStoreOp::eStore;
		renderPass.attachments[0].stencilLoadOp = ashes::AttachmentLoadOp::eDontCare;
		renderPass.attachments[0].stencilStoreOp = ashes::AttachmentStoreOp::eDontCare;
		renderPass.attachments[0].samples = ashes::SampleCountFlag::e1;
		renderPass.attachments[0].initialLayout = ashes::ImageLayout::eUndefined;
		renderPass.attachments[0].finalLayout = ashes::ImageLayout::eShaderReadOnlyOptimal;

		renderPass.subpasses.resize( 1u );
		renderPass.subpasses[0].colorAttachments = { { 0u, ashes::ImageLayout::eColourAttachmentOptimal } };

		renderPass.dependencies.resize( 2u );
		renderPass.dependencies[0].srcSubpass = ashes::ExternalSubpass;
		renderPass.dependencies[0].dstSubpass = 0u;
		renderPass.dependencies[0].dependencyFlags = ashes::DependencyFlag::eByRegion;
		renderPass.dependencies[0].srcAccessMask = ashes::AccessFlag::eHostWrite;
		renderPass.dependencies[0].dstAccessMask = ashes::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[0].srcStageMask = ashes::PipelineStageFlag::eHost;
		renderPass.dependencies[0].dstStageMask = ashes::PipelineStageFlag::eColourAttachmentOutput;

		renderPass.dependencies[1].srcSubpass = 0u;
		renderPass.dependencies[1].dstSubpass = ashes::ExternalSubpass;
		renderPass.dependencies[1].dependencyFlags = ashes::DependencyFlag::eByRegion;
		renderPass.dependencies[1].srcAccessMask = ashes::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[1].dstAccessMask = ashes::AccessFlag::eShaderRead;
		renderPass.dependencies[1].srcStageMask = ashes::PipelineStageFlag::eColourAttachmentOutput;
		renderPass.dependencies[1].dstStageMask = ashes::PipelineStageFlag::eFragmentShader;

		m_passes.blurY[0].renderPass = m_device->createRenderPass( renderPass );

		for ( auto i = 0u; i < m_passes.blurY.size(); ++i )
		{
			auto & blur = m_passes.blurY[i];
			std::vector< ashes::DescriptorSetLayoutBinding > bindings
			{
				ashes::DescriptorSetLayoutBinding{ 0u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
				ashes::DescriptorSetLayoutBinding{ 1u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eFragment },
				ashes::DescriptorSetLayoutBinding{ 2u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eFragment },
			};
			blur.descriptorLayout = m_device->createDescriptorSetLayout( std::move( bindings ) );
			blur.descriptorPool = blur.descriptorLayout->createPool( 1u );
			blur.descriptorSet = blur.descriptorPool->createDescriptorSet();
			blur.descriptorSet->createBinding( blur.descriptorLayout->getBinding( 0u )
				, *m_passes.blurX[i].views[0]
				, *m_blurSamplers[i] );
			blur.descriptorSet->createBinding( blur.descriptorLayout->getBinding( 1u )
				, *m_blurConfiguration
				, i );
			blur.descriptorSet->createBinding( blur.descriptorLayout->getBinding( 2u )
				, *m_blurDirection
				, 1u );
			blur.descriptorSet->update();
			blur.semaphore = m_device->createSemaphore();
			blur.pipelineLayout = m_device->createPipelineLayout( *blur.descriptorLayout );

			ashes::FrameBufferAttachmentArray attaches
			{
				{ *m_passes.blurY[0].renderPass->getAttachments().begin(), *m_passes.hi.views[i] }
			};
			blur.frameBuffer = m_passes.blurY[0].renderPass->createFrameBuffer( dimensions
				, std::move( attaches ) );

			std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

			if ( !wxFileExists( shadersFolder / "blur.vert" )
				|| !wxFileExists( shadersFolder / "blur.frag" ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			ashes::ShaderStageStateArray shaderStages;
			shaderStages.push_back( { m_device->createShaderModule( ashes::ShaderStageFlag::eVertex ) } );
			shaderStages.push_back( { m_device->createShaderModule( ashes::ShaderStageFlag::eFragment ) } );
			shaderStages[0].module->loadShader( common::parseShaderFile( *m_device
				, ashes::ShaderStageFlag::eVertex
				, shadersFolder / "blur.vert" ) );
			shaderStages[1].module->loadShader( common::parseShaderFile( *m_device
				, ashes::ShaderStageFlag::eFragment
				, shadersFolder / "blur.frag" ) );

			ashes::GraphicsPipelineCreateInfo pipeline
			{
				shaderStages,
				*m_passes.blurY[0].renderPass,
				ashes::VertexInputState::create( *m_mainVertexLayout ),
				ashes::InputAssemblyState{ ashes::PrimitiveTopology::eTriangleStrip },
				ashes::RasterisationState{},
				ashes::MultisampleState{},
				ashes::ColourBlendState::createDefault(),
				{},
				std::nullopt,
				std::nullopt,
				ashes::Viewport{ { 0, 0 }, dimensions },
				ashes::Scissor{ { 0, 0 }, dimensions }
			};
			blur.pipeline = blur.pipelineLayout->createPipeline( pipeline );

			blur.commandBuffer = m_commandPool->createCommandBuffer( true );
			auto & cmd = *blur.commandBuffer;

			cmd.begin();
			cmd.beginRenderPass( *m_passes.blurY[0].renderPass
				, *blur.frameBuffer
				, { ashes::ClearColorValue{ 0.0, 0.0, 0.0, 0.0 } }
			, ashes::SubpassContents::eInline );
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
		ashes::ImageViewCreateInfo view{};
		view.format = m_renderTargetColourView->getFormat();
		view.viewType = ashes::TextureViewType::e2D;
		view.subresourceRange.aspectMask = ashes::ImageAspectFlag::eColour;
		view.subresourceRange.baseArrayLayer = 0u;
		view.subresourceRange.layerCount = 1u;
		view.subresourceRange.levelCount = uint32_t( m_passes.blurY.size() );
		view.subresourceRange.baseMipLevel = 0u;
		m_blurMipView = m_passes.hi.image->createView( view );

		m_mipSampler = m_device->createSampler( ashes::WrapMode::eClampToEdge
			, ashes::WrapMode::eClampToEdge
			, ashes::WrapMode::eClampToEdge
			, ashes::Filter::eLinear
			, ashes::Filter::eLinear
			, ashes::MipmapMode::eNearest
			, 0.0f
			, float( m_passes.blurY.size() ) );

		auto dimensions = m_swapChain->getDimensions();
		std::vector< ashes::DescriptorSetLayoutBinding > bindings
		{
			ashes::DescriptorSetLayoutBinding{ 0u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
			ashes::DescriptorSetLayoutBinding{ 1u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
		};

		m_passes.combine.descriptorLayout = m_device->createDescriptorSetLayout( std::move( bindings ) );
		m_passes.combine.descriptorPool = m_passes.combine.descriptorLayout->createPool( 1u );
		m_passes.combine.descriptorSet = m_passes.combine.descriptorPool->createDescriptorSet();
		m_passes.combine.descriptorSet->createBinding( m_passes.combine.descriptorLayout->getBinding( 0u )
			, *m_renderTargetColourView
			, *m_sampler );
		m_passes.combine.descriptorSet->createBinding( m_passes.combine.descriptorLayout->getBinding( 1u )
			, *m_blurMipView
			, *m_mipSampler );

		m_passes.combine.descriptorSet->update();
		m_passes.combine.semaphore = m_device->createSemaphore();
		m_passes.combine.pipelineLayout = m_device->createPipelineLayout( *m_passes.combine.descriptorLayout );

		ashes::ImageCreateInfo image{};
		image.arrayLayers = 1u;
		image.extent = ashes::Extent3D{ dimensions.width, dimensions.height, 1u };
		image.flags = 0u;
		image.format = m_renderTargetColourView->getFormat();
		image.imageType = ashes::TextureType::e2D;
		image.initialLayout = ashes::ImageLayout::eUndefined;
		image.mipLevels = uint32_t( m_passes.blurX.size() );
		image.samples = ashes::SampleCountFlag::e1;
		image.tiling = ashes::ImageTiling::eOptimal;
		image.usage = ashes::ImageUsageFlag::eColourAttachment
			| ashes::ImageUsageFlag::eSampled
			| ashes::ImageUsageFlag::eTransferDst
			| ashes::ImageUsageFlag::eTransferSrc;
		m_passes.combine.image = m_device->createTexture( image, ashes::MemoryPropertyFlag::eDeviceLocal );

		view = ashes::ImageViewCreateInfo{};
		view.format = image.format;
		view.viewType = ashes::TextureViewType::e2D;
		view.subresourceRange.aspectMask = ashes::ImageAspectFlag::eColour;
		view.subresourceRange.baseArrayLayer = 0u;
		view.subresourceRange.layerCount = 1u;
		view.subresourceRange.levelCount = 1u;
		view.subresourceRange.baseMipLevel = 0u;
		m_passes.combine.views.emplace_back( m_passes.combine.image->createView( view ) );

		ashes::RenderPassCreateInfo renderPass{};
		renderPass.attachments.resize( 1u );
		renderPass.attachments[0].format = image.format;
		renderPass.attachments[0].loadOp = ashes::AttachmentLoadOp::eClear;
		renderPass.attachments[0].storeOp = ashes::AttachmentStoreOp::eStore;
		renderPass.attachments[0].stencilLoadOp = ashes::AttachmentLoadOp::eDontCare;
		renderPass.attachments[0].stencilStoreOp = ashes::AttachmentStoreOp::eDontCare;
		renderPass.attachments[0].samples = ashes::SampleCountFlag::e1;
		renderPass.attachments[0].initialLayout = ashes::ImageLayout::eUndefined;
		renderPass.attachments[0].finalLayout = ashes::ImageLayout::eColourAttachmentOptimal;

		renderPass.subpasses.resize( 1u );
		renderPass.subpasses[0].colorAttachments = { { 0u, ashes::ImageLayout::eColourAttachmentOptimal } };

		renderPass.dependencies.resize( 2u );
		renderPass.dependencies[0].srcSubpass = ashes::ExternalSubpass;
		renderPass.dependencies[0].dstSubpass = 0u;
		renderPass.dependencies[0].dependencyFlags = ashes::DependencyFlag::eByRegion;
		renderPass.dependencies[0].srcAccessMask = ashes::AccessFlag::eHostWrite;
		renderPass.dependencies[0].dstAccessMask = ashes::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[0].srcStageMask = ashes::PipelineStageFlag::eHost;
		renderPass.dependencies[0].dstStageMask = ashes::PipelineStageFlag::eColourAttachmentOutput;

		renderPass.dependencies[1].srcSubpass = 0u;
		renderPass.dependencies[1].dstSubpass = ashes::ExternalSubpass;
		renderPass.dependencies[1].dependencyFlags = ashes::DependencyFlag::eByRegion;
		renderPass.dependencies[1].srcAccessMask = ashes::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[1].dstAccessMask = ashes::AccessFlag::eShaderRead;
		renderPass.dependencies[1].srcStageMask = ashes::PipelineStageFlag::eColourAttachmentOutput;
		renderPass.dependencies[1].dstStageMask = ashes::PipelineStageFlag::eFragmentShader;

		m_passes.combine.renderPass = m_device->createRenderPass( renderPass );

		ashes::FrameBufferAttachmentArray attaches
		{
			{ *m_passes.combine.renderPass->getAttachments().begin(), *m_passes.combine.views[0] }
		};
		m_passes.combine.frameBuffer = m_passes.combine.renderPass->createFrameBuffer( dimensions
			, std::move( attaches ) );

		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "combine.vert" )
			|| !wxFileExists( shadersFolder / "combine.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		ashes::ShaderStageStateArray shaderStages;
		shaderStages.push_back( { m_device->createShaderModule( ashes::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device->createShaderModule( ashes::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( common::parseShaderFile( *m_device
			, ashes::ShaderStageFlag::eVertex
			, shadersFolder / "combine.vert" ) );
		shaderStages[1].module->loadShader( common::parseShaderFile( *m_device
			, ashes::ShaderStageFlag::eFragment
			, shadersFolder / "combine.frag" ) );

		ashes::GraphicsPipelineCreateInfo pipeline
		{
			shaderStages,
			*m_passes.combine.renderPass,
			ashes::VertexInputState::create( *m_mainVertexLayout ),
			ashes::InputAssemblyState{ ashes::PrimitiveTopology::eTriangleStrip },
			ashes::RasterisationState{},
			ashes::MultisampleState{},
			ashes::ColourBlendState::createDefault(),
			{},
			std::nullopt,
			std::nullopt,
			ashes::Viewport{ { 0, 0 }, dimensions },
			ashes::Scissor{ { 0, 0 }, dimensions }
		};
		m_passes.combine.pipeline = m_passes.combine.pipelineLayout->createPipeline( pipeline );

		m_passes.combine.commandBuffer = m_commandPool->createCommandBuffer( true );
		auto & cmd = *m_passes.combine.commandBuffer;

		cmd.begin();
		cmd.beginRenderPass( *m_passes.combine.renderPass
			, *m_passes.combine.frameBuffer
			, { ashes::ClearColorValue{ 0.0, 0.0, 0.0, 0.0 } }
			, ashes::SubpassContents::eInline );
		cmd.bindPipeline( *m_passes.combine.pipeline );
		cmd.bindDescriptorSet( *m_passes.combine.descriptorSet, *m_passes.combine.pipelineLayout );
		cmd.bindVertexBuffer( 0u, m_mainVertexBuffer->getBuffer(), 0u );
		cmd.draw( m_mainVertexBuffer->getCount() );
		cmd.endRenderPass();
		cmd.end();
	}

	void RenderPanel::doCreateMainVertexBuffer()
	{
		m_mainVertexLayout = ashes::makeLayout< TexturedVertexData >( 0 );
		m_mainVertexLayout->createAttribute( 0u
			, ashes::Format::eR32G32B32A32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, position ) ) );
		m_mainVertexLayout->createAttribute( 1u
			, ashes::Format::eR32G32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, uv ) ) );

		m_mainVertexBuffer = ashes::makeVertexBuffer< TexturedVertexData >( *m_device
			, uint32_t( m_mainVertexData.size() )
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_stagingBuffer->uploadVertexData( *m_graphicsQueue
			, *m_commandPool
			, m_mainVertexData
			, *m_mainVertexBuffer );
	}

	void RenderPanel::doCreateMainPipeline()
	{
		m_mainPipelineLayout = m_device->createPipelineLayout( *m_mainDescriptorLayout );
		wxSize size{ GetClientSize() };
		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "main.vert" )
			|| !wxFileExists( shadersFolder / "main.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		std::vector< ashes::ShaderStageState > shaderStages;
		shaderStages.push_back( { m_device->createShaderModule( ashes::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device->createShaderModule( ashes::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( common::parseShaderFile( *m_device
			, ashes::ShaderStageFlag::eVertex
			, shadersFolder / "main.vert" ) );
		shaderStages[1].module->loadShader( common::parseShaderFile( *m_device
			, ashes::ShaderStageFlag::eFragment
			, shadersFolder / "main.frag" ) );

		m_mainPipeline = m_mainPipelineLayout->createPipeline( ashes::GraphicsPipelineCreateInfo
		{
			std::move( shaderStages ),
			*m_mainRenderPass,
			ashes::VertexInputState::create( *m_mainVertexLayout ),
			ashes::InputAssemblyState{ ashes::PrimitiveTopology::eTriangleStrip },
			ashes::RasterisationState{},
			ashes::MultisampleState{},
			ashes::ColourBlendState::createDefault(),
			{ ashes::DynamicStateEnable::eViewport, ashes::DynamicStateEnable::eScissor }
		} );
	}

	void RenderPanel::doPrepareMainFrames()
	{
		m_frameBuffers = m_swapChain->createFrameBuffers( *m_mainRenderPass );
		m_commandBuffers = m_swapChain->createCommandBuffers( *m_commandPool );

		for ( size_t i = 0u; i < m_frameBuffers.size(); ++i )
		{
			auto & frameBuffer = *m_frameBuffers[i];
			auto & commandBuffer = *m_commandBuffers[i];

			wxSize size{ GetClientSize() };

			commandBuffer.begin( ashes::CommandBufferUsageFlag::eSimultaneousUse );
			auto dimensions = m_swapChain->getDimensions();
			commandBuffer.beginRenderPass( *m_mainRenderPass
				, frameBuffer
				, { ashes::ClearValue{ { 1.0, 0.0, 0.0, 1.0 } } }
				, ashes::SubpassContents::eInline );
			commandBuffer.bindPipeline( *m_mainPipeline );
			commandBuffer.setViewport( { dimensions.width
				, dimensions.height
				, 0
				, 0 } );
			commandBuffer.setScissor( { 0
				, 0
				, dimensions.width
				, dimensions.height } );
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
		m_objectUbo->getData( 0 ) = m_rotate * originalRotate;
		m_stagingBuffer->uploadUniformData( *m_graphicsQueue
			, *m_commandPool
			, m_objectUbo->getDatas()
			, *m_objectUbo
			, ashes::PipelineStageFlag::eVertexShader );
		save = std::chrono::high_resolution_clock::now();
	}

	void RenderPanel::doDraw()
	{
		auto resources = m_swapChain->getResources();

		if ( resources )
		{
			auto before = std::chrono::high_resolution_clock::now();
			auto toWait = &resources->getImageAvailableSemaphore();
			m_graphicsQueue->submit( *m_commandBuffer
				, *toWait
				, ashes::PipelineStageFlag::eColourAttachmentOutput
				, *m_semaphore
				, nullptr );
			toWait = m_semaphore.get();

			m_graphicsQueue->submit( *m_passes.hi.commandBuffer
				, *toWait
				, ashes::PipelineStageFlag::eColourAttachmentOutput
				, *m_passes.hi.semaphore
				, nullptr );
			toWait = m_passes.hi.semaphore.get();

			for ( auto & blur : m_passes.blurX )
			{
				m_graphicsQueue->submit( *blur.commandBuffer
					, *toWait
					, ashes::PipelineStageFlag::eColourAttachmentOutput
					, *blur.semaphore
					, nullptr );
				toWait = blur.semaphore.get();
			}

			for ( auto & blur : m_passes.blurY )
			{
				m_graphicsQueue->submit( *blur.commandBuffer
					, *toWait
					, ashes::PipelineStageFlag::eColourAttachmentOutput
					, *blur.semaphore
					, nullptr );
				toWait = blur.semaphore.get();
			}

			m_graphicsQueue->submit( *m_passes.combine.commandBuffer
				, *toWait
				, ashes::PipelineStageFlag::eColourAttachmentOutput
				, *m_passes.combine.semaphore
				, nullptr );
			toWait = m_passes.combine.semaphore.get();

			m_graphicsQueue->submit( *m_commandBuffers[resources->getBackBuffer()]
				, *toWait
				, ashes::PipelineStageFlag::eColourAttachmentOutput
				, resources->getRenderingFinishedSemaphore()
				, &resources->getFence() );
			m_swapChain->present( *resources, *m_presentQueue );
			ashes::UInt64Array values{ 0u, 0u };
			m_queryPool->getResults( 0u
				, 2u
				, 0u
				, ashes::QueryResultFlag::eWait
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
		}
	}

	void RenderPanel::doResetSwapChain()
	{
		m_device->waitIdle();
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
