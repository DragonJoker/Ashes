#include "RenderPanel.hpp"
#include "Application.hpp"

#include <Buffer/VertexBuffer.hpp>
#include <Command/CommandBuffer.hpp>
#include <Core/BackBuffer.hpp>
#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Core/Instance.hpp>
#include <Core/RenderingResources.hpp>
#include <Core/SwapChain.hpp>
#include <Enum/SubpassContents.hpp>
#include <Image/StagingTexture.hpp>
#include <Image/Texture.hpp>
#include <Miscellaneous/QueryPool.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <GlslToSpv.hpp>
#include <Sync/BufferMemoryBarrier.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

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
		, ashes::Instance const & instance )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
		, m_timer{ new wxTimer{ this, int( Ids::RenderTimer ) } }
		, m_vertexData{
		{
			{
				{ -0.7f, -0.7f, 0.0f, 1.0f },
				-0.1f,
			},
			{
				{ -0.7f, 0.7f, 0.0f, 1.0f },
				-0.1f,
			},
			{
				{ 0.7f, -0.7f, 0.0f, 1.0f },
				1.1f,
			},
			{
				{ 0.7f, 0.7f, 0.0f, 1.0f },
				1.1f,
			}
		} }
	{
		try
		{
			auto surface = doCreateSurface( instance );
			std::cout << "Surface created." << std::endl;
			doCreateDevice( instance, std::move( surface ) );
			std::cout << "Logical device created." << std::endl;
			doCreateSwapChain();
			std::cout << "Swapchain created." << std::endl;
			doCreateStagingBuffer();
			std::cout << "Staging buffer created." << std::endl;
			doCreateTexture();
			std::cout << "Texture created." << std::endl;
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
			m_descriptorSet.reset();
			m_descriptorPool.reset();
			m_descriptorLayout.reset();
			m_sampler.reset();
			m_view.reset();
			m_texture.reset();
			m_stagingBuffer.reset();
			m_pipeline.reset();
			m_vertexLayout.reset();
			m_pipelineLayout.reset();
			m_commandBuffers.clear();
			m_frameBuffers.clear();
			m_vertexBuffer.reset();
			m_renderPass.reset();
			m_swapChain.reset();
			m_commandPool.reset();
			m_presentQueue.reset();
			m_graphicsQueue.reset();
			m_device.reset();
		}
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
		m_swapChain->setClearColour( ashes::ClearColorValue{ 1.0f, 0.8f, 0.4f, 0.0f } );
		m_swapChainReset = m_swapChain->onReset.connect( [this]()
		{
			doResetSwapChain();
		} );
	}

	void RenderPanel::doCreateTexture()
	{
		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / "Assets";
		auto image = common::loadImage( shadersFolder / "texture.png" );
		auto stagingTexture = m_device->createStagingTexture( image.format
			, { image.size.width, 1u } );
		m_texture = m_device->createTexture(
			{
				0u,
				ashes::TextureType::e1D,
				image.format,
				{ image.size.width, 1u, 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eTransferDst | ashes::ImageUsageFlag::eSampled
			}
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_view = m_texture->createView( ashes::TextureViewType::e1D
			, image.format );
		m_sampler = m_device->createSampler( ashes::WrapMode::eClampToEdge
			, ashes::WrapMode::eClampToEdge
			, ashes::WrapMode::eClampToEdge
			, ashes::Filter::eLinear
			, ashes::Filter::eLinear );
		stagingTexture->uploadTextureData( *m_graphicsQueue
			, *m_commandPool
			, image.format
			, image.data
			, *m_view );
	}

	void RenderPanel::doCreateDescriptorSet()
	{
		std::vector< ashes::DescriptorSetLayoutBinding > bindings
		{
			{ 0u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment }
		};
		m_descriptorLayout = m_device->createDescriptorSetLayout( std::move( bindings ) );
		m_descriptorPool = m_descriptorLayout->createPool( 1u );
		m_descriptorSet = m_descriptorPool->createDescriptorSet();
		m_descriptorSet->createBinding( m_descriptorLayout->getBinding( 0u )
			, *m_view
			, *m_sampler );
		m_descriptorSet->update();
	}

	void RenderPanel::doCreateRenderPass()
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
		m_renderPass = m_device->createRenderPass( attaches
			, std::move( subpasses )
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eBottomOfPipe
				, ashes::AccessFlag::eMemoryRead }
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eBottomOfPipe
				, ashes::AccessFlag::eMemoryRead } );
	}

	void RenderPanel::doCreateVertexBuffer()
	{
		m_vertexBuffer = ashes::makeVertexBuffer< TexturedVertexData >( *m_device
			, uint32_t( m_vertexData.size() )
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_vertexLayout = ashes::makeLayout< TexturedVertexData >( 0 );
		m_vertexLayout->createAttribute( 0u
			, ashes::Format::eR32G32B32A32_SFLOAT
			, 0u );
		m_vertexLayout->createAttribute( 1u
			, ashes::Format::eR32_SFLOAT
			, offsetof( TexturedVertexData, uv ) );
		m_stagingBuffer->uploadVertexData( *m_graphicsQueue
			, *m_commandPool
			, m_vertexData
			, *m_vertexBuffer );
	}

	void RenderPanel::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique< ashes::StagingBuffer >( *m_device
			, 0u
			, 1000000u );
	}

	void RenderPanel::doCreatePipeline()
	{
		m_pipelineLayout = m_device->createPipelineLayout( *m_descriptorLayout );
		wxSize size{ GetClientSize() };
		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "shader.vert" )
			|| !wxFileExists( shadersFolder / "shader.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		std::vector< ashes::ShaderStageState > shaderStages;
		shaderStages.push_back( { m_device->createShaderModule( ashes::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device->createShaderModule( ashes::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( common::parseShaderFile( *m_device
			, ashes::ShaderStageFlag::eVertex
			, shadersFolder / "shader.vert" ) );
		shaderStages[1].module->loadShader( common::parseShaderFile( *m_device
			, ashes::ShaderStageFlag::eFragment
			, shadersFolder / "shader.frag" ) );

		m_pipeline = m_pipelineLayout->createPipeline( ashes::GraphicsPipelineCreateInfo
		{
			std::move( shaderStages ),
			*m_renderPass,
			ashes::VertexInputState::create( *m_vertexLayout ),
			ashes::InputAssemblyState{ ashes::PrimitiveTopology::eTriangleStrip },
			ashes::RasterisationState{},
			ashes::MultisampleState{},
			ashes::ColourBlendState::createDefault(),
			{ ashes::DynamicStateEnable::eViewport, ashes::DynamicStateEnable::eScissor }
		} );
	}

	void RenderPanel::doPrepareFrames()
	{
		m_queryPool = m_device->createQueryPool( ashes::QueryType::eTimestamp
			, 2u
			, 0u );
		m_commandBuffers = m_swapChain->createCommandBuffers( *m_commandPool );
		m_frameBuffers = m_swapChain->createFrameBuffers( *m_renderPass );
		auto dimensions = m_swapChain->getDimensions();

		for ( size_t i = 0u; i < m_commandBuffers.size(); ++i )
		{
			auto & frameBuffer = *m_frameBuffers[i];
			auto & commandBuffer = *m_commandBuffers[i];

			commandBuffer.begin( ashes::CommandBufferUsageFlag::eSimultaneousUse );
			commandBuffer.resetQueryPool( *m_queryPool
				, 0u
				, 2u );
			commandBuffer.beginRenderPass( *m_renderPass
				, frameBuffer
				, { m_swapChain->getClearColour() }
				, ashes::SubpassContents::eInline );
			commandBuffer.writeTimestamp( ashes::PipelineStageFlag::eTopOfPipe
				, *m_queryPool
				, 0u );
			commandBuffer.bindPipeline( *m_pipeline );
			commandBuffer.setViewport( { dimensions.width
				, dimensions.height
				, 0
				, 0 } );
			commandBuffer.setScissor( { 0
				, 0
				, dimensions.width
				, dimensions.height } );
			commandBuffer.bindVertexBuffer( 0u, m_vertexBuffer->getBuffer(), 0u );
			commandBuffer.bindDescriptorSet( *m_descriptorSet
				, *m_pipelineLayout );
			commandBuffer.draw( 4u );
			commandBuffer.writeTimestamp( ashes::PipelineStageFlag::eBottomOfPipe
				, *m_queryPool
				, 1u );
			commandBuffer.endRenderPass();

			commandBuffer.end();
		}
	}

	void RenderPanel::doDraw()
	{
		auto resources = m_swapChain->getResources();

		if ( resources )
		{
			auto before = std::chrono::high_resolution_clock::now();
			m_graphicsQueue->submit( *m_commandBuffers[resources->getBackBuffer()]
				, resources->getImageAvailableSemaphore()
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
		m_swapChain->reset( { uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ) } );
		m_timer->Start( TimerTimeMs );
		event.Skip();
	}
}
