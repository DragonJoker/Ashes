#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <Buffer/PushConstantsBuffer.hpp>
#include <Buffer/StagingBuffer.hpp>
#include <Buffer/UniformBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <Command/Queue.hpp>
#include <Core/Surface.hpp>
#include <Core/Device.hpp>
#include <Core/Instance.hpp>
#include <Core/SwapChain.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/StagingTexture.hpp>
#include <Image/Image.hpp>
#include <Image/ImageView.hpp>
#include <Miscellaneous/PushConstantRange.hpp>
#include <Miscellaneous/QueryPool.hpp>
#include <Pipeline/ComputePipeline.hpp>
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
		static ashes::Format const ColourFormat = ashes::Format::eR32G32B32A32_SFLOAT;
		static ashes::Format const DepthFormat = ashes::Format::eD32_SFLOAT;
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, utils::Instance const & instance )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
		, m_timer{ new wxTimer{ this, int( Ids::RenderTimer ) } }
		, m_offscreenVertexData
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
			doCreateUniformBuffers();
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
			doCreateComputeDescriptorSet();
			std::cout << "Compute descriptor set created." << std::endl;
			doCreateComputePipeline();
			std::cout << "Compute pipeline created." << std::endl;
			doPrepareCompute();
			std::cout << "Compute pipeline prepared." << std::endl;
			doCreateMainDescriptorSet();
			std::cout << "Main descriptor set created." << std::endl;
			doCreateMainRenderPass();
			std::cout << "Main render pass created." << std::endl;
			doCreateMainVertexBuffer();
			std::cout << "Main vertex buffer created." << std::endl;
			doCreateMainPipeline();
			std::cout << "Main pipeline created." << std::endl;
			doPrepareMainFrames();
			std::cout << "Main frames prepared." << std::endl;
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
			m_mainVertexLayout.reset();
			m_mainRenderPass.reset();

			m_computeQueryPool.reset();
			m_computePipeline.reset();
			m_computePipelineLayout.reset();
			m_computeDescriptorSet.reset();
			m_computeDescriptorPool.reset();
			m_computeDescriptorLayout.reset();
			m_computeCommandBuffer.reset();
			m_computeFence.reset();
			m_computeUbo.reset();

			m_offscreenQueryPool.reset();
			m_offscreenDescriptorSets[0].reset();
			m_offscreenDescriptorSets[1].reset();
			m_offscreenDescriptorPool.reset();
			m_offscreenDescriptorLayout.reset();
			m_offscreenPipeline.reset();
			m_offscreenPipelineLayout.reset();
			m_offscreenIndexBuffer.reset();
			m_offscreenVertexBuffer.reset();
			m_offscreenVertexLayout.reset();
			m_offscreenRenderPass.reset();

			m_frameBuffer.reset();
			m_renderTargetDepth.reset();
			m_renderTargetColour.reset();

			m_objectPcbs[0].reset();
			m_objectPcbs[1].reset();

			m_swapChain.reset();
			m_computeCommandPool.reset();
			m_commandPool.reset();
			m_computeQueue.reset();
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

	ashes::SurfacePtr RenderPanel::doCreateSurface( utils::Instance const & instance )
	{
		auto handle = common::makeWindowHandle( *this );
		auto & gpu = instance.getPhysicalDevice( 0u );
		return instance.getInstance().createSurface( gpu
			, std::move( handle ) );
	}

	void RenderPanel::doCreateDevice( utils::Instance const & instance
		, ashes::SurfacePtr surface )
	{
		m_device = std::make_unique< utils::Device >( instance.getInstance()
			, std::move( surface ) );
		m_graphicsQueue = m_device->getDevice().getQueue( m_device->getGraphicsQueueFamily(), 0u );
		m_presentQueue = m_device->getDevice().getQueue( m_device->getPresentQueueFamily(), 0u );
		m_computeQueue = m_device->getDevice().getQueue( m_device->getComputeQueueFamily(), 0u );
		m_commandPool = m_device->getDevice().createCommandPool( m_device->getGraphicsQueueFamily()
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );
		m_computeCommandPool = m_device->getDevice().createCommandPool( m_device->getComputeQueueFamily()
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );
		m_objectPcbs[0] = std::make_unique< ashes::PushConstantsBuffer< utils::Vec4 > >( ashes::ShaderStageFlag::eFragment
			, ashes::PushConstantArray{ { 0u, ashes::ConstantFormat::eVec4f } } );
		m_objectPcbs[1] = std::make_unique< ashes::PushConstantsBuffer< utils::Vec4 > >( ashes::ShaderStageFlag::eFragment
			, ashes::PushConstantArray{ { 0u, ashes::ConstantFormat::eVec4f } } );
		*m_objectPcbs[0]->getData() = utils::Vec4{ 1.0, 0.0, 0.0, 1.0 };
		*m_objectPcbs[1]->getData() = utils::Vec4{ 0.0, 1.0, 0.0, 1.0 };
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
			m_frame = 0u;
			doCreateFrameBuffer();
			doPrepareOffscreenFrame();
			doCreateComputeDescriptorSet();
			doCreateComputePipeline();
			doPrepareCompute();
			doCreateMainDescriptorSet();
			doPrepareMainFrames();
		} );
	}

	void RenderPanel::doCreateTexture()
	{
		std::string shadersFolder = utils::getPath( utils::getExecutableDirectory() ) / "share" / "Assets";
		auto image = common::loadImage( shadersFolder / "texture.png" );
		auto stagingTexture = m_device->getDevice().createStagingTexture( image.format
			, { image.size.width, image.size.height } );
		m_texture = m_device->createImage(
			{
				0u,
				ashes::ImageType::e2D,
				image.format,
				{ image.size.width, image.size.height, 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eTransferDst | ashes::ImageUsageFlag::eSampled
			}
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_view = m_texture->createView( ashes::ImageViewType::e2D
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

	void RenderPanel::doCreateUniformBuffers()
	{
		m_matrixUbo = utils::makeUniformBuffer< utils::Mat4 >( *m_device
			, 1u
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_objectUbo = utils::makeUniformBuffer< utils::Mat4 >( *m_device
			, 2u
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_computeUbo = utils::makeUniformBuffer< Configuration >( *m_device
			, 1u
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
	}

	void RenderPanel::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique< ashes::StagingBuffer >( m_device->getDevice()
			, 0u
			, 10000000u );
	}

	void RenderPanel::doCreateOffscreenDescriptorSet()
	{
		std::vector< ashes::DescriptorSetLayoutBinding > bindings
		{
			ashes::DescriptorSetLayoutBinding{ 0u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
			ashes::DescriptorSetLayoutBinding{ 1u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eVertex },
			ashes::DescriptorSetLayoutBinding{ 2u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eVertex },
		};
		m_offscreenDescriptorLayout = m_device->getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_offscreenDescriptorPool = m_offscreenDescriptorLayout->createPool( 2u );

		m_offscreenDescriptorSets[0] = m_offscreenDescriptorPool->createDescriptorSet();
		m_offscreenDescriptorSets[0]->createBinding( m_offscreenDescriptorLayout->getBinding( 0u )
			, *m_view
			, *m_sampler );
		m_offscreenDescriptorSets[0]->createBinding( m_offscreenDescriptorLayout->getBinding( 1u )
			, *m_matrixUbo
			, 0u
			, 1u );
		m_offscreenDescriptorSets[0]->createBinding( m_offscreenDescriptorLayout->getBinding( 2u )
			, *m_objectUbo
			, 0u
			, 1u );
		m_offscreenDescriptorSets[0]->update();

		m_offscreenDescriptorSets[1] = m_offscreenDescriptorPool->createDescriptorSet();
		m_offscreenDescriptorSets[1]->createBinding( m_offscreenDescriptorLayout->getBinding( 0u )
			, *m_view
			, *m_sampler );
		m_offscreenDescriptorSets[1]->createBinding( m_offscreenDescriptorLayout->getBinding( 1u )
			, *m_matrixUbo
			, 0u
			, 1u );
		m_offscreenDescriptorSets[1]->createBinding( m_offscreenDescriptorLayout->getBinding( 2u )
			, *m_objectUbo
			, 1u
			, 1u );
		m_offscreenDescriptorSets[1]->update();
	}

	void RenderPanel::doCreateOffscreenRenderPass()
	{
		ashes::AttachmentDescriptionArray attaches
		{
			{
				ColourFormat,
				ashes::SampleCountFlag::e1,
				ashes::AttachmentLoadOp::eClear,
				ashes::AttachmentStoreOp::eStore,
				ashes::AttachmentLoadOp::eDontCare,
				ashes::AttachmentStoreOp::eDontCare,
				ashes::ImageLayout::eUndefined,
				ashes::ImageLayout::eShaderReadOnlyOptimal,
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
		m_offscreenRenderPass = m_device->getDevice().createRenderPass( attaches
			, std::move( subpasses )
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eColourAttachmentOutput
				, ashes::AccessFlag::eColourAttachmentWrite }
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eColourAttachmentOutput
				, ashes::AccessFlag::eShaderRead } );
	}

	void RenderPanel::doCreateFrameBuffer()
	{
		auto size = GetClientSize();
		m_renderTargetColour = m_device->createImage(
			{
				0u,
				ashes::ImageType::e2D,
				ColourFormat,
				{ uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ), 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eColourAttachment | ashes::ImageUsageFlag::eSampled | ashes::ImageUsageFlag::eStorage
			}
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		
		m_renderTargetDepth = m_device->createImage(
			{
				0u,
				ashes::ImageType::e2D,
				DepthFormat,
				{ uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ), 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eDepthStencilAttachment
			}
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		ashes::FrameBufferAttachmentArray attaches;
		attaches.emplace_back( *( m_offscreenRenderPass->getAttachments().begin() + 0u )
			, m_renderTargetColour->createView( ashes::ImageViewType::e2D
				, m_renderTargetColour->getFormat() ) );
		attaches.emplace_back( *( m_offscreenRenderPass->getAttachments().begin() + 1u )
			, m_renderTargetDepth->createView( ashes::ImageViewType::e2D
				, m_renderTargetDepth->getFormat() ) );
		m_frameBuffer = m_offscreenRenderPass->createFrameBuffer( { uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ) }
		, std::move( attaches ) );
	}

	void RenderPanel::doCreateOffscreenVertexBuffer()
	{
		m_offscreenVertexLayout = ashes::makeLayout< TexturedVertexData >( 0 );
		m_offscreenVertexLayout->createAttribute( 0u
			, ashes::Format::eR32G32B32A32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, position ) ) );
		m_offscreenVertexLayout->createAttribute( 1u
			, ashes::Format::eR32G32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, uv ) ) );

		m_offscreenVertexBuffer = utils::makeVertexBuffer< TexturedVertexData >( *m_device
			, uint32_t( m_offscreenVertexData.size() )
			, ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_stagingBuffer->uploadVertexData( *m_graphicsQueue
			, *m_commandPool
			, m_offscreenVertexData
			, *m_offscreenVertexBuffer );

		m_offscreenIndexBuffer = utils::makeBuffer< uint16_t >( *m_device
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
		ashes::PushConstantRange range{ ashes::ShaderStageFlag::eFragment, 0u, m_objectPcbs[0]->getSize() };
		m_offscreenPipelineLayout = m_device->getDevice().createPipelineLayout( ashes::DescriptorSetLayoutCRefArray{ { *m_offscreenDescriptorLayout } }
		, ashes::PushConstantRangeArray{ { range } } );
		wxSize size{ GetClientSize() };
		std::string shadersFolder = utils::getPath( utils::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "offscreen.vert" )
			|| !wxFileExists( shadersFolder / "offscreen.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		std::vector< ashes::ShaderStageState > shaderStages;
		shaderStages.push_back( { m_device->getDevice().createShaderModule( ashes::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device->getDevice().createShaderModule( ashes::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( common::parseShaderFile( m_device->getDevice()
			, ashes::ShaderStageFlag::eVertex
			, shadersFolder / "offscreen.vert" ) );
		shaderStages[1].module->loadShader( common::parseShaderFile( m_device->getDevice()
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
			{ ashes::DynamicStateEnable::eViewport, ashes::DynamicStateEnable::eScissor },
			ashes::DepthStencilState{},
		} );
	}

	void RenderPanel::doCreateMainDescriptorSet()
	{
		std::vector< ashes::DescriptorSetLayoutBinding > bindings
		{
			ashes::DescriptorSetLayoutBinding{ 0u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
		};
		m_mainDescriptorLayout = m_device->getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_mainDescriptorPool = m_mainDescriptorLayout->createPool( 1u );
		m_mainDescriptorSet = m_mainDescriptorPool->createDescriptorSet();
		m_mainDescriptorSet->createBinding( m_mainDescriptorLayout->getBinding( 0u )
			, m_frameBuffer->begin()->getView()
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
		m_mainRenderPass = m_device->getDevice().createRenderPass( attaches
			, std::move( subpasses )
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eBottomOfPipe
				, ashes::AccessFlag::eMemoryRead }
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eBottomOfPipe
				, ashes::AccessFlag::eMemoryRead } );
	}

	void RenderPanel::doPrepareOffscreenFrame()
	{
		doUpdateProjection();
		m_offscreenQueryPool = m_device->getDevice().createQueryPool( ashes::QueryType::eTimestamp
			, 2u
			, 0u );
		m_commandBuffer = m_commandPool->createCommandBuffer();
		wxSize size{ GetClientSize() };
		auto & commandBuffer = *m_commandBuffer;
		auto & frameBuffer = *m_frameBuffer;

		commandBuffer.begin( ashes::CommandBufferUsageFlag::eSimultaneousUse );
		auto dimensions = m_swapChain->getDimensions();
		commandBuffer.resetQueryPool( *m_offscreenQueryPool
			, 0u
			, 2u );
		commandBuffer.beginRenderPass( *m_offscreenRenderPass
			, frameBuffer
			, { ashes::ClearValue{ m_clearColour }, ashes::ClearValue{ ashes::DepthStencilClearValue{ 1.0f, 0u } } }
			, ashes::SubpassContents::eInline );
		commandBuffer.writeTimestamp( ashes::PipelineStageFlag::eTopOfPipe
			, *m_offscreenQueryPool
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
		commandBuffer.bindDescriptorSet( *m_offscreenDescriptorSets[0]
			, *m_offscreenPipelineLayout );
		commandBuffer.pushConstants( *m_offscreenPipelineLayout
			, *m_objectPcbs[0] );
		commandBuffer.drawIndexed( uint32_t( m_offscreenIndexData.size() ) );
		commandBuffer.bindDescriptorSet( *m_offscreenDescriptorSets[1]
			, *m_offscreenPipelineLayout );
		commandBuffer.pushConstants( *m_offscreenPipelineLayout
			, *m_objectPcbs[1] );
		commandBuffer.drawIndexed( uint32_t( m_offscreenIndexData.size() ) );
		commandBuffer.writeTimestamp( ashes::PipelineStageFlag::eBottomOfPipe
			, *m_offscreenQueryPool
			, 1u );
		commandBuffer.endRenderPass();
		commandBuffer.end();
	}

	void RenderPanel::doCreateComputeDescriptorSet()
	{
		std::vector< ashes::DescriptorSetLayoutBinding > bindings
		{
			ashes::DescriptorSetLayoutBinding{ 0u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eCompute },
			ashes::DescriptorSetLayoutBinding{ 1u, ashes::DescriptorType::eStorageImage, ashes::ShaderStageFlag::eCompute },
		};
		m_computeDescriptorLayout = m_device->getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_computeDescriptorPool = m_computeDescriptorLayout->createPool( 1u );
		m_computeDescriptorSet = m_computeDescriptorPool->createDescriptorSet();
		m_computeDescriptorSet->createBinding( m_computeDescriptorLayout->getBinding( 0u )
			, *m_computeUbo
			, 0u
			, 1u );
		m_computeDescriptorSet->createBinding( m_computeDescriptorLayout->getBinding( 1u )
			, m_frameBuffer->begin()->getView()
			, 0u );
		m_computeDescriptorSet->update();
		m_computeUbo->getData( 0u ).textureSize = utils::IVec2{ m_swapChain->getDimensions().width, m_swapChain->getDimensions().height };
		m_stagingBuffer->uploadUniformData( *m_graphicsQueue
			, *m_commandPool
			, m_computeUbo->getDatas()
			, *m_computeUbo
			, ashes::PipelineStageFlag::eComputeShader );
	}

	void RenderPanel::doCreateComputePipeline()
	{
		m_computePipelineLayout = m_device->getDevice().createPipelineLayout( *m_computeDescriptorLayout );
		std::string shadersFolder = utils::getPath( utils::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "shader.comp" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		ashes::ShaderStageState shaderStage
		{
			{ m_device->getDevice().createShaderModule( ashes::ShaderStageFlag::eCompute ) }
		};
		shaderStage.module->loadShader( common::parseShaderFile( m_device->getDevice()
			, ashes::ShaderStageFlag::eCompute
			, shadersFolder / "shader.comp" ) );

		m_computePipeline = m_computePipelineLayout->createPipeline( std::move( shaderStage ) );
	}

	void RenderPanel::doPrepareCompute()
	{
		wxSize size{ GetClientSize() };
		m_computeQueryPool = m_device->getDevice().createQueryPool( ashes::QueryType::eTimestamp
			, 2u
			, 0u );
		m_computeFence = m_device->getDevice().createFence( ashes::FenceCreateFlag::eSignaled );
		m_computeCommandBuffer = m_computeCommandPool->createCommandBuffer();
		auto & commandBuffer = *m_computeCommandBuffer;

		commandBuffer.begin( ashes::CommandBufferUsageFlag::eSimultaneousUse );
		commandBuffer.resetQueryPool( *m_computeQueryPool
			, 0u
			, 2u );
		commandBuffer.writeTimestamp( ashes::PipelineStageFlag::eTopOfPipe
			, *m_computeQueryPool
			, 0u );
		commandBuffer.bindPipeline( *m_computePipeline );
		commandBuffer.bindDescriptorSet( *m_computeDescriptorSet
			, *m_computePipelineLayout
			, ashes::PipelineBindPoint::eCompute );
		commandBuffer.dispatch( size.GetWidth() / 16u
			, size.GetHeight() / 16u
			, 1u );
		commandBuffer.writeTimestamp( ashes::PipelineStageFlag::eBottomOfPipe
			, *m_computeQueryPool
			, 1u );
		commandBuffer.end();
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

		m_mainVertexBuffer = utils::makeVertexBuffer< TexturedVertexData >( *m_device
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
		m_mainPipelineLayout = m_device->getDevice().createPipelineLayout( *m_mainDescriptorLayout );
		wxSize size{ GetClientSize() };
		std::string shadersFolder = utils::getPath( utils::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "main.vert" )
			|| !wxFileExists( shadersFolder / "main.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		std::vector< ashes::ShaderStageState > shaderStages;
		shaderStages.push_back( { m_device->getDevice().createShaderModule( ashes::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device->getDevice().createShaderModule( ashes::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( common::parseShaderFile( m_device->getDevice()
			, ashes::ShaderStageFlag::eVertex
			, shadersFolder / "main.vert" ) );
		shaderStages[1].module->loadShader( common::parseShaderFile( m_device->getDevice()
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
		m_commandBuffers = m_swapChain->createCommandBuffers();

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
		static utils::Mat4 const originalTranslate1 = []()
		{
			utils::Mat4 result;
			result = utils::translate( result, { 2, 0, -5 } );
			return result;
		}( );
		static utils::Mat4 const originalTranslate2 = []()
		{
			utils::Mat4 result;
			result = utils::translate( result, { -2, 0, -5 } );
			return result;
		}( );
		static utils::Mat4 const originalRotate = []()
		{
			utils::Mat4 result;
			result = utils::rotate( result
				, float( utils::DegreeToRadian * 45.0 )
				, { 0, 0, 1 } );
			return result;
		}( );
		m_rotate[0] = utils::rotate( m_rotate[0]
			, float( utils::DegreeToRadian )
			, { 0, 1, 0 } );
		m_rotate[1] = utils::rotate( m_rotate[1]
			, -float( utils::DegreeToRadian )
			, { 0, 1, 0 } );
		m_objectUbo->getData( 0u ) = originalTranslate1 * m_rotate[0] * originalRotate;
		m_objectUbo->getData( 1u ) = originalTranslate2 * m_rotate[1] * originalRotate;
		m_stagingBuffer->uploadUniformData( *m_graphicsQueue
			, *m_commandPool
			, m_objectUbo->getDatas()
			, *m_objectUbo
			, ashes::PipelineStageFlag::eVertexShader );
		m_computeUbo->getData( 0u ).roll = ( m_frame % GetClientSize().GetWidth() ) * 0.03f;
		m_stagingBuffer->uploadUniformData( *m_graphicsQueue
			, *m_commandPool
			, m_computeUbo->getDatas()
			, *m_computeUbo
			, ashes::PipelineStageFlag::eComputeShader );
		++m_frame;
	}

	void RenderPanel::doDraw()
	{
		auto resources = m_swapChain->getResources();

		if ( resources )
		{
			auto before = std::chrono::high_resolution_clock::now();
			m_graphicsQueue->submit( *m_commandBuffer
				, nullptr );
			
			m_computeFence->wait( ~( 0u ) );
			m_computeFence->reset();
			m_computeQueue->submit( *m_computeCommandBuffer
				, m_computeFence.get() );

			m_graphicsQueue->submit( *m_commandBuffers[resources->getImageIndex()]
				, resources->getImageAvailableSemaphore()
				, ashes::PipelineStageFlag::eColourAttachmentOutput
				, resources->getRenderingFinishedSemaphore()
				, &resources->getFence() );
			m_swapChain->present( *resources, *m_presentQueue );
			ashes::UInt64Array values1{ 0u, 0u };
			ashes::UInt64Array values2{ 0u, 0u };
			m_offscreenQueryPool->getResults( 0u
				, 2u
				, 0u
				, ashes::QueryResultFlag::eWait
				, values1 );
			m_computeQueryPool->getResults( 0u
				, 2u
				, 0u
				, ashes::QueryResultFlag::eWait
				, values1 );

			// Elapsed time in nanoseconds
			auto elapsed1 = std::chrono::nanoseconds{ uint64_t( ( values1[1] - values1[0] ) / float( m_device->getDevice().getTimestampPeriod() ) ) };
			auto elapsed2 = std::chrono::nanoseconds{ uint64_t( ( values2[1] - values2[0] ) / float( m_device->getDevice().getTimestampPeriod() ) ) };
			auto after = std::chrono::high_resolution_clock::now();
			wxGetApp().updateFps( std::chrono::duration_cast< std::chrono::microseconds >( elapsed1 + elapsed2 )
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
