#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <Buffer/StagingBuffer.hpp>
#include <Buffer/UniformBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <Command/Queue.hpp>
#include <Core/BackBuffer.hpp>
#include <Core/Surface.hpp>
#include <Core/Device.hpp>
#include <Core/Instance.hpp>
#include <Core/SwapChain.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/StagingTexture.hpp>
#include <Image/Texture.hpp>
#include <Miscellaneous/QueryPool.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <GlslToSpv.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <Transform.hpp>

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
		static ashes::Format const DepthFormat = ashes::Format::eD32_SFLOAT;
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, ashes::Instance const & instance )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
		, m_timer{ new wxTimer{ this, int( Ids::RenderTimer ) } }

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
			doCreateDescriptorSet();
			std::cout << "Descriptor set created." << std::endl;
			doCreateRenderPass();
			std::cout << "Render pass created." << std::endl;
			doCreateVertexBuffer();
			std::cout << "Vertex buffer created." << std::endl;
			doCreatePipeline();
			std::cout << "Pipeline created." << std::endl;
			doPrepareFrames();
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
		delete m_timer;

		if ( m_device )
		{
			m_device->getDevice().waitIdle();
			m_queryPool.reset();
			m_commandBuffers.clear();
			m_frameBuffers.clear();
			m_descriptorSet.reset();
			m_descriptorPool.reset();
			m_descriptorLayout.reset();
			m_sampler.reset();
			m_view.reset();
			m_texture.reset();
			m_stagingBuffer.reset();

			m_matrixUbo.reset();
			m_objectUbo.reset();

			m_pipeline.reset();
			m_pipelineLayout.reset();
			m_indexBuffer.reset();
			m_vertexBuffer.reset();
			m_renderPass.reset();

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
		m_matrixUbo->getData( 0u ) = utils::Mat4{ m_device->getDevice().perspective( float( utils::toRadians( 90.0_degrees ) )
			, width / height
			, 0.01f
			, 100.0f ) };
		m_stagingBuffer->uploadUniformData( *m_graphicsQueue
			, *m_commandPool
			, m_matrixUbo->getDatas()
			, *m_matrixUbo
			, ashes::PipelineStageFlag::eVertexShader );
	}

	ashes::SurfacePtr RenderPanel::doCreateSurface( ashes::Instance const & instance )
	{
		auto handle = common::makeWindowHandle( *this );
		auto & gpu = instance.getPhysicalDevice( 0u );
		return instance.createSurface( gpu
			, std::move( handle ) );
	}

	void RenderPanel::doCreateDevice( ashes::Instance const & instance
		, ashes::SurfacePtr surface )
	{
		m_device = std::make_unique< utils::Device >( instance
			, std::move( surface ) );
		m_graphicsQueue = m_device->getDevice().getQueue( m_device->getGraphicsQueueFamily(), 0u );
		m_presentQueue = m_device->getDevice().getQueue( m_device->getPresentQueueFamily(), 0u );
		m_commandPool = m_device->getDevice().createCommandPool( m_device->getGraphicsQueueFamily()
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );
	}

	void RenderPanel::doCreateSwapChain()
	{
		wxSize size{ GetClientSize() };
		m_swapChain = std::make_unique< utils::SwapChain >( m_device->getDevice()
			, *m_commandPool
			, ashes::Extent2D{ uint32_t( size.x ), uint32_t( size.y ) } );
		m_clearColour = ashes::ClearColorValue{ 1.0f, 0.8f, 0.4f, 0.0f };
		m_swapChainReset = m_swapChain->onReset.connect( [this]()
		{
			doCreateDescriptorSet();
			doPrepareFrames();
		} );
	}

	void RenderPanel::doCreateTexture()
	{
		std::string shadersFolder = utils::getPath( utils::getExecutableDirectory() ) / "share" / "Assets";
		auto image = common::loadImage( shadersFolder / "texture.png" );
		auto stagingTexture = m_device->getDevice().createStagingTexture( image.format
			, { image.size.width, image.size.height } );
		m_texture = m_device->getDevice().createTexture(
			{
				0u,
				ashes::TextureType::e2D,
				image.format,
				{ image.size.width, image.size.height, 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eTransferDst | ashes::ImageUsageFlag::eSampled
			}
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_view = m_texture->createView( ashes::TextureViewType::e2D
			, image.format );
		m_sampler = m_device->getDevice().createSampler( ashes::WrapMode::eClampToEdge
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

	void RenderPanel::doCreateUniformBuffer()
	{
		m_matrixUbo = std::make_unique< ashes::UniformBuffer< utils::Mat4 > >( m_device->getDevice()
			, 1u
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_objectUbo = std::make_unique< ashes::UniformBuffer< utils::Mat4 > >( m_device->getDevice()
			, 1u
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
	}

	void RenderPanel::doCreateDescriptorSet()
	{
		std::vector< ashes::DescriptorSetLayoutBinding > bindings
		{
			ashes::DescriptorSetLayoutBinding{ 0u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
			ashes::DescriptorSetLayoutBinding{ 1u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eVertex },
			ashes::DescriptorSetLayoutBinding{ 2u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eVertex },
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
		m_descriptorSet->createBinding( m_descriptorLayout->getBinding( 2u )
			, *m_objectUbo
			, 0u
			, 1u );
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
			},
			{
				DepthFormat,
				ashes::SampleCountFlag::e1,
				ashes::AttachmentLoadOp::eClear,
				ashes::AttachmentStoreOp::eStore,
				ashes::AttachmentLoadOp::eDontCare,
				ashes::AttachmentStoreOp::eDontCare,
				ashes::ImageLayout::eUndefined,
				ashes::ImageLayout::eDepthStencilAttachmentOptimal,
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
			, subAttaches
			, ashes::AttachmentReference{ 1u, ashes::ImageLayout::eDepthStencilAttachmentOptimal } ) );
		m_renderPass = m_device->getDevice().createRenderPass( attaches
			, std::move( subpasses )
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eBottomOfPipe
				, ashes::AccessFlag::eMemoryRead }
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eBottomOfPipe
				, ashes::AccessFlag::eMemoryRead } );
	}

	void RenderPanel::doCreateVertexBuffer()
	{
		m_vertexLayout = ashes::makeLayout< TexturedVertexData >( 0 );
		m_vertexLayout->createAttribute( 0u
			, ashes::Format::eR32G32B32A32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, position ) ) );
		m_vertexLayout->createAttribute( 1u
			, ashes::Format::eR32G32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, uv ) ) );
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
		m_vertexBuffer = ashes::makeVertexBuffer< TexturedVertexData >( m_device->getDevice()
			, uint32_t( vertexData.size() )
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_stagingBuffer->uploadVertexData( *m_graphicsQueue
			, *m_commandPool
			, vertexData
			, *m_vertexBuffer );

		std::vector< uint16_t > indexData
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
		m_indexBuffer = ashes::makeBuffer< uint16_t >( m_device->getDevice()
			, uint32_t( indexData.size() )
			, ashes::BufferTarget::eIndexBuffer | ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_stagingBuffer->uploadBufferData( *m_graphicsQueue
			, *m_commandPool
			, indexData
			, *m_indexBuffer );
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
		wxSize size{ GetClientSize() };
		std::string shadersFolder = utils::getPath( utils::getExecutableDirectory() ) / "share" / AppName / "Shaders";


		if ( !wxFileExists( shadersFolder / "shader.vert" )
			|| !wxFileExists( shadersFolder / "shader.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		std::vector< ashes::ShaderStageState > shaderStages;
		shaderStages.push_back( { m_device->getDevice().createShaderModule( ashes::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device->getDevice().createShaderModule( ashes::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( common::parseShaderFile( m_device->getDevice()
			, ashes::ShaderStageFlag::eVertex
			, shadersFolder / "shader.vert" ) );
		shaderStages[1].module->loadShader( common::parseShaderFile( m_device->getDevice()
			, ashes::ShaderStageFlag::eFragment
			, shadersFolder / "shader.frag" ) );

		ashes::RasterisationState rasterisationState;
		rasterisationState.cullMode = ashes::CullModeFlag::eNone;

		m_pipeline = m_pipelineLayout->createPipeline( ashes::GraphicsPipelineCreateInfo
		{
			std::move( shaderStages ),
			*m_renderPass,
			ashes::VertexInputState::create( *m_vertexLayout ),
			ashes::InputAssemblyState{ ashes::PrimitiveTopology::eTriangleList },
			rasterisationState,
			ashes::MultisampleState{},
			ashes::ColourBlendState::createDefault(),
			{ ashes::DynamicStateEnable::eViewport, ashes::DynamicStateEnable::eScissor },
			ashes::DepthStencilState{}
		} );
	}

	void RenderPanel::doPrepareFrames()
	{
		doUpdateProjection();
		m_queryPool = m_device->getDevice().createQueryPool( ashes::QueryType::eTimestamp
			, 2u
			, 0u );
		m_swapChain->createDepthStencil( DepthFormat );
		m_frameBuffers = m_swapChain->createFrameBuffers( *m_renderPass );
		m_commandBuffers = m_swapChain->createCommandBuffers();

		for ( size_t i = 0u; i < m_frameBuffers.size(); ++i )
		{
			auto & frameBuffer = *m_frameBuffers[i];
			auto & commandBuffer = *m_commandBuffers[i];
			wxSize size{ GetClientSize() };
			auto dimensions = m_swapChain->getDimensions();

			commandBuffer.begin( ashes::CommandBufferUsageFlag::eSimultaneousUse );
			commandBuffer.resetQueryPool( *m_queryPool
				, 0u
				, 2u );
			commandBuffer.beginRenderPass( *m_renderPass
				, frameBuffer
				, { ashes::ClearValue{ m_clearColour }, ashes::ClearValue{ ashes::DepthStencilClearValue{ 1.0f, 0u } } }
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
			commandBuffer.bindIndexBuffer( m_indexBuffer->getBuffer(), 0u, ashes::IndexType::eUInt16 );
			commandBuffer.bindDescriptorSet( *m_descriptorSet
				, *m_pipelineLayout );
			commandBuffer.drawIndexed( m_indexBuffer->getCount() );
			commandBuffer.writeTimestamp( ashes::PipelineStageFlag::eBottomOfPipe
				, *m_queryPool
				, 1u );
			commandBuffer.endRenderPass();
			commandBuffer.end();
		}
	}

	void RenderPanel::doUpdate()
	{
		static utils::Mat4 const originalTranslate = []()
		{
			utils::Mat4 result;
			result = utils::translate( result, { 0, 0, -5 } );
			return result;
		}();
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
		m_objectUbo->getData( 0 ) = originalTranslate * m_rotate * originalRotate;
		m_stagingBuffer->uploadUniformData( *m_graphicsQueue
			, *m_commandPool
			, m_objectUbo->getDatas()
			, *m_objectUbo
			, ashes::PipelineStageFlag::eVertexShader );
	}

	void RenderPanel::doDraw()
	{
		auto resources = m_swapChain->getResources();

		if ( resources )
		{
			auto before = std::chrono::high_resolution_clock::now();
			m_graphicsQueue->submit( *m_commandBuffers[resources->getImageIndex()]
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
