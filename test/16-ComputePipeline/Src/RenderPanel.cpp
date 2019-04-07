#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <AshesPP/Buffer/PushConstantsBuffer.hpp>
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
#include <AshesPP/Pipeline/ComputePipeline.hpp>
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
		static VkFormat const ColourFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
		static VkFormat const DepthFormat = VK_FORMAT_D32_SFLOAT;
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
			doCreateDevice( instance, *surface );
			std::cout << "Logical device created." << std::endl;
			doCreateSwapChain( std::move( surface ) );
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
		m_computeQueue = m_device->getDevice().getQueue( m_device->getComputeQueueFamily(), 0u );
		m_commandPool = m_device->getDevice().createCommandPool( m_device->getGraphicsQueueFamily()
			, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT );
		m_computeCommandPool = m_device->getDevice().createCommandPool( m_device->getComputeQueueFamily()
			, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT );
		m_objectPcbs[0] = std::make_unique< ashes::PushConstantsBuffer< utils::Vec4 > >( VK_SHADER_STAGE_FRAGMENT_BIT
			, ashes::PushConstantArray{ { 0u, ashes::ConstantFormat::eVec4f } } );
		m_objectPcbs[1] = std::make_unique< ashes::PushConstantsBuffer< utils::Vec4 > >( VK_SHADER_STAGE_FRAGMENT_BIT
			, ashes::PushConstantArray{ { 0u, ashes::ConstantFormat::eVec4f } } );
		*m_objectPcbs[0]->getData() = utils::Vec4{ 1.0, 0.0, 0.0, 1.0 };
		*m_objectPcbs[1]->getData() = utils::Vec4{ 0.0, 1.0, 0.0, 1.0 };
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

	void RenderPanel::doCreateUniformBuffers()
	{
		m_matrixUbo = utils::makeUniformBuffer< utils::Mat4 >( *m_device
			, 1u
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_objectUbo = utils::makeUniformBuffer< utils::Mat4 >( *m_device
			, 2u
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_computeUbo = utils::makeUniformBuffer< Configuration >( *m_device
			, 1u
			, VK_BUFFER_USAGE_TRANSFER_DST_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
	}

	void RenderPanel::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique< ashes::StagingBuffer >( m_device->getDevice()
			, 0u
			, 10000000u );
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
		ashes::VkAttachmentDescriptionArray attaches
		{
			{
				ColourFormat,
				VK_SAMPLE_COUNT_1_BIT,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_STORE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			},
			{
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
		ashes::VkAttachmentReferenceArray subAttaches
		{
			{ 0u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }
		};
		ashes::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( std::make_unique< ashes::RenderSubpass >( VkPipelineBindPoint::eGraphics
			, ashes::RenderSubpassState{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT }
			, subAttaches
			, VkAttachmentReference{ 1u, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL } ) );
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
				ColourFormat,
				{ uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ), 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::eStorage
			}
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		
		m_renderTargetDepth = m_device->createImage(
			{
				0u,
				VK_IMAGE_TYPE_2D,
				DepthFormat,
				{ uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ), 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
			}
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		ashes::ImageViewPtrArray attaches;
		attaches.emplace_back( *( m_offscreenRenderPass->getAttachments().begin() + 0u )
			, m_renderTargetColour->createView( VK_IMAGE_VIEW_TYPE_2D
				, m_renderTargetColour->getFormat() ) );
		attaches.emplace_back( *( m_offscreenRenderPass->getAttachments().begin() + 1u )
			, m_renderTargetDepth->createView( VK_IMAGE_VIEW_TYPE_2D
				, m_renderTargetDepth->getFormat() ) );
		m_frameBuffer = m_offscreenRenderPass->createFrameBuffer( { uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ) }
		, std::move( attaches ) );
	}

	void RenderPanel::doCreateOffscreenVertexBuffer()
	{
		m_offscreenVertexLayout = ashes::makeLayout< TexturedVertexData >( 0 );
		m_offscreenVertexLayout->createAttribute( 0u
			, VK_FORMAT_R32G32B32A32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, position ) ) );
		m_offscreenVertexLayout->createAttribute( 1u
			, VK_FORMAT_R32G32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, uv ) ) );

		m_offscreenVertexBuffer = utils::makeVertexBuffer< TexturedVertexData >( *m_device
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
		VkPushConstantRange range{ VK_SHADER_STAGE_FRAGMENT_BIT, 0u, m_objectPcbs[0]->getSize() };
		m_offscreenPipelineLayout = m_device->getDevice().createPipelineLayout( ashes::DescriptorSetLayoutCRefArray{ { *m_offscreenDescriptorLayout } }
		, ashes::VkPushConstantRangeArray{ { range } } );
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
			{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR },
			ashes::DepthStencilState{},
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
			, m_frameBuffer->begin()->getView()
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
		m_offscreenQueryPool = m_device->getDevice().createQueryPool( VK_QUERY_TYPE_TIMESTAMP
			, 2u
			, 0u );
		m_commandBuffer = m_commandPool->createCommandBuffer();
		wxSize size{ GetClientSize() };
		auto & commandBuffer = *m_commandBuffer;
		auto & frameBuffer = *m_frameBuffer;

		commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT );
		auto dimensions = m_swapChain->getDimensions();
		commandBuffer.resetQueryPool( *m_offscreenQueryPool
			, 0u
			, 2u );
		commandBuffer.beginRenderPass( *m_offscreenRenderPass
			, frameBuffer
			, { VkClearValue{ m_clearColour }, VkClearValue{ VkClearDepthStencilValue{ 1.0f, 0u } } }
			, VK_SUBPASS_CONTENTS_INLINE );
		commandBuffer.writeTimestamp( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, *m_offscreenQueryPool
			, 0u );
		commandBuffer.bindPipeline( *m_offscreenPipeline );
		commandBuffer.setViewport( { 0.0f, 0.0f, float( dimensions.width ), float( dimensions.height ), 0.0f, 1.0f } );
		commandBuffer.setScissor( { { 0, 0 }, { dimensions.width, dimensions.height } } );
		commandBuffer.bindVertexBuffer( 0u, m_offscreenVertexBuffer->getBuffer(), 0u );
		commandBuffer.bindIndexBuffer( m_offscreenIndexBuffer->getBuffer(), 0u, VK_INDEX_TYPE_UINT16 );
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
		commandBuffer.writeTimestamp( VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
			, *m_offscreenQueryPool
			, 1u );
		commandBuffer.endRenderPass();
		commandBuffer.end();
	}

	void RenderPanel::doCreateComputeDescriptorSet()
	{
		ashes::VkDescriptorSetLayoutBindingArray bindings
		{
			VkDescriptorSetLayoutBinding{ 0u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT },
			VkDescriptorSetLayoutBinding{ 1u, VkDescriptorType::eStorageImage, VK_SHADER_STAGE_COMPUTE_BIT },
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
			, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT );
	}

	void RenderPanel::doCreateComputePipeline()
	{
		m_computePipelineLayout = m_device->getDevice().createPipelineLayout( *m_computeDescriptorLayout );
		std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders";

		if ( !wxFileExists( shadersFolder / "shader.comp" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		ashes::ShaderStageState shaderStage
		{
			{ m_device->getDevice().createShaderModule( common::parseShaderFile( m_device->getDevice()
				, VK_SHADER_STAGE_COMPUTE_BIT
				, shadersFolder / "shader.comp" ) ) }
		};
		shaderStage.module->loadShader(  );

		m_computePipeline = m_computePipelineLayout->createPipeline( std::move( shaderStage ) );
	}

	void RenderPanel::doPrepareCompute()
	{
		wxSize size{ GetClientSize() };
		m_computeQueryPool = m_device->getDevice().createQueryPool( VK_QUERY_TYPE_TIMESTAMP
			, 2u
			, 0u );
		m_computeFence = m_device->getDevice().createFence( VK_FENCE_CREATE_SIGNALED_BIT );
		m_computeCommandBuffer = m_computeCommandPool->createCommandBuffer();
		auto & commandBuffer = *m_computeCommandBuffer;

		commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT );
		commandBuffer.resetQueryPool( *m_computeQueryPool
			, 0u
			, 2u );
		commandBuffer.writeTimestamp( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, *m_computeQueryPool
			, 0u );
		commandBuffer.bindPipeline( *m_computePipeline );
		commandBuffer.bindDescriptorSet( *m_computeDescriptorSet
			, *m_computePipelineLayout
			, VK_PIPELINE_BIND_POINT_COMPUTE );
		commandBuffer.dispatch( size.GetWidth() / 16u
			, size.GetHeight() / 16u
			, 1u );
		commandBuffer.writeTimestamp( VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
			, *m_computeQueryPool
			, 1u );
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
			, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT );
		m_computeUbo->getData( 0u ).roll = ( m_frame % GetClientSize().GetWidth() ) * 0.03f;
		m_stagingBuffer->uploadUniformData( *m_graphicsQueue
			, *m_commandPool
			, m_computeUbo->getDatas()
			, *m_computeUbo
			, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT );
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
				, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, resources->getRenderingFinishedSemaphore()
				, &resources->getFence() );
			m_swapChain->present( *resources, *m_presentQueue );
			ashes::UInt64Array values1{ 0u, 0u };
			ashes::UInt64Array values2{ 0u, 0u };
			m_offscreenQueryPool->getResults( 0u
				, 2u
				, 0u
				, VK_QUERY_RESULT_WAIT_BIT
				, values1 );
			m_computeQueryPool->getResults( 0u
				, 2u
				, 0u
				, VK_QUERY_RESULT_WAIT_BIT
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
