#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <Buffer/GeometryBuffers.hpp>
#include <Buffer/PushConstantsBuffer.hpp>
#include <Buffer/StagingBuffer.hpp>
#include <Buffer/UniformBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <Command/Queue.hpp>
#include <Core/BackBuffer.hpp>
#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Core/Renderer.hpp>
#include <Core/SwapChain.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/Texture.hpp>
#include <Image/TextureView.hpp>
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
#include <Shader/ShaderProgram.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

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
		static renderer::PixelFormat const ColourFormat = renderer::PixelFormat::eRGBA32F;
		static renderer::PixelFormat const DepthFormat = renderer::PixelFormat::eD32F;
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, renderer::Renderer const & renderer )
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
	, m_objectPcbs
	{
		{ renderer::ShaderStageFlag::eFragment, { { 3u, 0u, renderer::AttributeFormat::eVec4f } } },
		{ renderer::ShaderStageFlag::eFragment, { { 3u, 0u, renderer::AttributeFormat::eVec4f } } },
	}
	{
		*m_objectPcbs[0].getData() = renderer::RgbaColour{ 1.0, 0.0, 0.0, 1.0 };
		*m_objectPcbs[1].getData() = renderer::RgbaColour{ 0.0, 1.0, 0.0, 1.0 };
		try
		{
			doCreateDevice( renderer );
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
			m_device->waitIdle();

			m_updateCommandBuffer.reset();
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
			m_mainProgram.reset();
			m_mainVertexBuffer.reset();
			m_mainVertexLayout.reset();
			m_mainGeometryBuffers.reset();
			m_mainRenderPass.reset();

			m_computeQueryPool.reset();
			m_computePipeline.reset();
			m_computePipelineLayout.reset();
			m_computeProgram.reset();
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
			m_offscreenProgram.reset();
			m_offscreenIndexBuffer.reset();
			m_offscreenVertexBuffer.reset();
			m_offscreenGeometryBuffers.reset();
			m_offscreenVertexLayout.reset();
			m_offscreenRenderPass.reset();

			m_frameBuffer.reset();
			m_renderTargetDepthView.reset();
			m_renderTargetDepth.reset();
			m_renderTargetColourView.reset();
			m_renderTargetColour.reset();

			m_swapChain.reset();
			m_device->disable();
			m_device.reset();
		}
	}

	void RenderPanel::doUpdateProjection()
	{
		auto size = m_swapChain->getDimensions();
#if 0
		float halfWidth = static_cast< float >( size.x ) * 0.5f;
		float halfHeight = static_cast< float >( size.y ) * 0.5f;
		float wRatio = 1.0f;
		float hRatio = 1.0f;

		if ( halfHeight > halfWidth )
		{
			hRatio = halfHeight / halfWidth;
		}
		else
		{
			wRatio = halfWidth / halfHeight;
		}

		m_matrixUbo->getData( 0u ) = m_device->ortho( -2.0f * wRatio
			, 2.0f * wRatio
			, -2.0f * hRatio
			, 2.0f * hRatio
			, 0.0f
			, 10.0f );
#else
		auto width = float( size.x );
		auto height = float( size.y );
		m_matrixUbo->getData( 0u ) = m_device->perspective( utils::toRadians( 90.0_degrees )
			, width / height
			, 0.01f
			, 100.0f );
#endif
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_matrixUbo->getDatas()
			, *m_matrixUbo
			, renderer::PipelineStageFlag::eVertexShader );
	}

	void RenderPanel::doCreateDevice( renderer::Renderer const & renderer )
	{
		m_device = renderer.createDevice( common::makeConnection( this, renderer ) );
		m_device->enable();
	}

	void RenderPanel::doCreateSwapChain()
	{
		wxSize size{ GetClientSize() };
		m_swapChain = m_device->createSwapChain( { size.x, size.y } );
		m_swapChain->setClearColour( { 1.0f, 0.8f, 0.4f, 0.0f } );
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
		m_updateCommandBuffer = m_device->getGraphicsCommandPool().createCommandBuffer();
	}

	void RenderPanel::doCreateTexture()
	{
		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / "Assets";
		auto image = common::loadImage( shadersFolder / "texture.png" );
		m_texture = m_device->createTexture();
		m_texture->setImage( image.format, { image.size[0], image.size[1] } );
		m_view = m_texture->createView( m_texture->getType()
			, image.format );
		m_sampler = m_device->createSampler( renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::Filter::eLinear
			, renderer::Filter::eLinear );
		m_stagingBuffer->uploadTextureData( m_swapChain->getDefaultResources().getCommandBuffer()
			, image.data
			, *m_view );
	}

	void RenderPanel::doCreateUniformBuffers()
	{
		m_matrixUbo = std::make_unique< renderer::UniformBuffer< renderer::Mat4 > >( *m_device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_objectUbo = std::make_unique< renderer::UniformBuffer< renderer::Mat4 > >( *m_device
			, 2u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_computeUbo = std::make_unique< renderer::UniformBuffer< Configuration > >( *m_device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
	}

	void RenderPanel::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique< renderer::StagingBuffer >( *m_device
			, 0u
			, 10000000u );
	}

	void RenderPanel::doCreateOffscreenDescriptorSet()
	{
		std::vector< renderer::DescriptorSetLayoutBinding > bindings
		{
			renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
			renderer::DescriptorSetLayoutBinding{ 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex },
			renderer::DescriptorSetLayoutBinding{ 2u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex },
		};
		m_offscreenDescriptorLayout = m_device->createDescriptorSetLayout( std::move( bindings ) );
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
		renderer::RenderPassAttachmentArray attaches
		{
			{
				0u,
				ColourFormat,
				renderer::SampleCountFlag::e1,
				renderer::AttachmentLoadOp::eClear,
				renderer::AttachmentStoreOp::eStore,
				renderer::AttachmentLoadOp::eDontCare,
				renderer::AttachmentStoreOp::eDontCare,
				renderer::ImageLayout::eUndefined,
				renderer::ImageLayout::eShaderReadOnlyOptimal,
			},
			{
				1u,
				DepthFormat,
				renderer::SampleCountFlag::e1,
				renderer::AttachmentLoadOp::eClear,
				renderer::AttachmentStoreOp::eStore,
				renderer::AttachmentLoadOp::eDontCare,
				renderer::AttachmentStoreOp::eDontCare,
				renderer::ImageLayout::eUndefined,
				renderer::ImageLayout::eDepthStencilAttachmentOptimal,
			}
		};
		renderer::RenderSubpassAttachmentArray subAttaches
		{
			{ 0u, renderer::ImageLayout::eColourAttachmentOptimal }
		};
		renderer::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( m_device->createRenderSubpass( renderer::PipelineBindPoint::eGraphics
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite }
			, subAttaches
			, { 1u, renderer::ImageLayout::eDepthStencilAttachmentOptimal } ) );
		m_offscreenRenderPass = m_device->createRenderPass( attaches
			, std::move( subpasses )
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite }
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eShaderRead } );
	}

	void RenderPanel::doCreateFrameBuffer()
	{
		auto size = GetClientSize();
		m_renderTargetColour = m_device->createTexture();
		m_renderTargetColour->setImage( ColourFormat
			, { size.GetWidth(), size.GetHeight() }
			, renderer::ImageUsageFlag::eColourAttachment | renderer::ImageUsageFlag::eSampled | renderer::ImageUsageFlag::eStorage );
		m_renderTargetColourView = m_renderTargetColour->createView( m_renderTargetColour->getType()
			, m_renderTargetColour->getFormat() );

		m_renderTargetDepth = m_device->createTexture();
		m_renderTargetDepth->setImage( DepthFormat
			, { size.GetWidth(), size.GetHeight() }
			, renderer::ImageUsageFlag::eDepthStencilAttachment );
		m_renderTargetDepthView = m_renderTargetDepth->createView( m_renderTargetDepth->getType()
			, m_renderTargetDepth->getFormat() );
		renderer::FrameBufferAttachmentArray attaches;
		attaches.emplace_back( *( m_offscreenRenderPass->begin() + 0u ), *m_renderTargetColourView );
		attaches.emplace_back( *( m_offscreenRenderPass->begin() + 1u ), *m_renderTargetDepthView );
		m_frameBuffer = m_offscreenRenderPass->createFrameBuffer( { size.GetWidth(), size.GetHeight() }
		, std::move( attaches ) );
	}

	void RenderPanel::doCreateOffscreenVertexBuffer()
	{
		m_offscreenVertexLayout = renderer::makeLayout< TexturedVertexData >( 0 );
		m_offscreenVertexLayout->createAttribute< renderer::Vec4 >( 0u
			, uint32_t( offsetof( TexturedVertexData, position ) ) );
		m_offscreenVertexLayout->createAttribute< renderer::Vec2 >( 1u
			, uint32_t( offsetof( TexturedVertexData, uv ) ) );

		m_offscreenVertexBuffer = renderer::makeVertexBuffer< TexturedVertexData >( *m_device
			, uint32_t( m_offscreenVertexData.size() )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_stagingBuffer->uploadVertexData( m_swapChain->getDefaultResources().getCommandBuffer()
			, m_offscreenVertexData
			, *m_offscreenVertexBuffer
			, renderer::PipelineStageFlag::eVertexInput );

		m_offscreenIndexBuffer = renderer::makeBuffer< uint16_t >( *m_device
			, uint32_t( m_offscreenIndexData.size() )

			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_stagingBuffer->uploadBufferData( m_swapChain->getDefaultResources().getCommandBuffer()
			, m_offscreenIndexData
			, *m_offscreenIndexBuffer );

		m_offscreenGeometryBuffers = m_device->createGeometryBuffers( *m_offscreenVertexBuffer
			, 0u
			, *m_offscreenVertexLayout
			, m_offscreenIndexBuffer->getBuffer()
			, 0u
			, renderer::IndexType::eUInt16 );
	}

	void RenderPanel::doCreateOffscreenPipeline()
	{
		renderer::PushConstantRange range{ renderer::ShaderStageFlag::eFragment, 0u, m_objectPcbs[0].getSize() };
		m_offscreenPipelineLayout = m_device->createPipelineLayout( renderer::DescriptorSetLayoutCRefArray{ { *m_offscreenDescriptorLayout } }
		, renderer::PushConstantRangeCRefArray{ { range } } );
		wxSize size{ GetClientSize() };
		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";
		m_offscreenProgram = m_device->createShaderProgram();

		if ( !wxFileExists( shadersFolder / "offscreen.vert" )
			|| !wxFileExists( shadersFolder / "offscreen.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		m_offscreenProgram->createModule( common::parseShaderFile( *m_device, shadersFolder / "offscreen.vert" )
			, renderer::ShaderStageFlag::eVertex );
		m_offscreenProgram->createModule( common::parseShaderFile( *m_device, shadersFolder / "offscreen.frag" )
			, renderer::ShaderStageFlag::eFragment );

		m_offscreenPipeline = m_offscreenPipelineLayout->createPipeline( renderer::GraphicsPipelineCreateInfo
		{
			*m_offscreenProgram,
			*m_offscreenRenderPass,
			renderer::VertexInputState::create( *m_offscreenVertexLayout ),
			renderer::InputAssemblyState{ renderer::PrimitiveTopology::eTriangleList },
			renderer::RasterisationState{ 1.0f, 0, false, false, renderer::PolygonMode::eFill, renderer::CullModeFlag::eNone },
			renderer::MultisampleState{},
			renderer::ColourBlendState::createDefault(),
			renderer::DepthStencilState{},
		} );
	}

	void RenderPanel::doCreateMainDescriptorSet()
	{
		std::vector< renderer::DescriptorSetLayoutBinding > bindings
		{
			renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
		};
		m_mainDescriptorLayout = m_device->createDescriptorSetLayout( std::move( bindings ) );
		m_mainDescriptorPool = m_mainDescriptorLayout->createPool( 1u );
		m_mainDescriptorSet = m_mainDescriptorPool->createDescriptorSet();
		m_mainDescriptorSet->createBinding( m_mainDescriptorLayout->getBinding( 0u )
			, *m_renderTargetColourView
			, *m_sampler );
		m_mainDescriptorSet->update();
	}

	void RenderPanel::doCreateMainRenderPass()
	{
		renderer::RenderPassAttachmentArray attaches
		{
			{
				0u,
				m_swapChain->getFormat(),
				renderer::SampleCountFlag::e1,
				renderer::AttachmentLoadOp::eClear,
				renderer::AttachmentStoreOp::eStore,
				renderer::AttachmentLoadOp::eDontCare,
				renderer::AttachmentStoreOp::eDontCare,
				renderer::ImageLayout::eUndefined,
				renderer::ImageLayout::ePresentSrc,
			}
		};
		renderer::RenderSubpassAttachmentArray subAttaches
		{
			{ 0u, renderer::ImageLayout::eColourAttachmentOptimal }
		};
		renderer::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( m_device->createRenderSubpass( renderer::PipelineBindPoint::eGraphics
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite }
			, subAttaches ) );
		m_mainRenderPass = m_device->createRenderPass( attaches
			, std::move( subpasses )
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eBottomOfPipe
				, renderer::AccessFlag::eMemoryRead }
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eBottomOfPipe
				, renderer::AccessFlag::eMemoryRead } );
	}

	void RenderPanel::doPrepareOffscreenFrame()
	{
		doUpdateProjection();
		m_offscreenQueryPool = m_device->createQueryPool( renderer::QueryType::eTimestamp
			, 2u
			, 0u );
		m_commandBuffer = m_device->getGraphicsCommandPool().createCommandBuffer();
		wxSize size{ GetClientSize() };
		auto & commandBuffer = *m_commandBuffer;
		auto & frameBuffer = *m_frameBuffer;

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
		{
			auto dimensions = m_swapChain->getDimensions();
			commandBuffer.resetQueryPool( *m_offscreenQueryPool
				, 0u
				, 2u );
			commandBuffer.beginRenderPass( *m_offscreenRenderPass
				, frameBuffer
				, { renderer::ClearValue{ m_swapChain->getClearColour() }, renderer::ClearValue{ renderer::DepthStencilClearValue{ 1.0f, 0u } } }
				, renderer::SubpassContents::eInline );
			commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eTopOfPipe
				, *m_offscreenQueryPool
				, 0u );
			commandBuffer.bindPipeline( *m_offscreenPipeline );
			commandBuffer.setViewport( { uint32_t( dimensions.x )
				, uint32_t( dimensions.y )
				, 0
				, 0 } );
			commandBuffer.setScissor( { 0
				, 0
				, uint32_t( dimensions.x )
				, uint32_t( dimensions.y ) } );
			commandBuffer.bindGeometryBuffers( *m_offscreenGeometryBuffers );
			commandBuffer.bindDescriptorSet( *m_offscreenDescriptorSets[0]
				, *m_offscreenPipelineLayout );
			commandBuffer.pushConstants( *m_offscreenPipelineLayout
				, m_objectPcbs[0] );
			commandBuffer.drawIndexed( uint32_t( m_offscreenIndexData.size() ) );
			commandBuffer.bindDescriptorSet( *m_offscreenDescriptorSets[1]
				, *m_offscreenPipelineLayout );
			commandBuffer.pushConstants( *m_offscreenPipelineLayout
				, m_objectPcbs[1] );
			commandBuffer.drawIndexed( uint32_t( m_offscreenIndexData.size() ) );
			commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eBottomOfPipe
				, *m_offscreenQueryPool
				, 1u );
			commandBuffer.endRenderPass();
			auto res = commandBuffer.end();

			if ( !res )
			{
				std::stringstream stream;
				stream << "Command buffers recording failed.";
				throw std::runtime_error{ stream.str() };
			}
		}
	}

	void RenderPanel::doCreateComputeDescriptorSet()
	{
		std::vector< renderer::DescriptorSetLayoutBinding > bindings
		{
			renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eCompute },
			renderer::DescriptorSetLayoutBinding{ 1u, renderer::DescriptorType::eStorageImage, renderer::ShaderStageFlag::eCompute },
		};
		m_computeDescriptorLayout = m_device->createDescriptorSetLayout( std::move( bindings ) );
		m_computeDescriptorPool = m_computeDescriptorLayout->createPool( 1u );
		m_computeDescriptorSet = m_computeDescriptorPool->createDescriptorSet();
		m_computeDescriptorSet->createBinding( m_computeDescriptorLayout->getBinding( 0u )
			, *m_computeUbo
			, 0u
			, 1u );
		m_computeDescriptorSet->createBinding( m_computeDescriptorLayout->getBinding( 1u )
			, *m_renderTargetColourView
			, 0u );
		m_computeDescriptorSet->update();
		m_computeUbo->getData( 0u ).textureSize = renderer::IVec2{ m_swapChain->getDimensions() };
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_computeUbo->getDatas()
			, *m_computeUbo
			, renderer::PipelineStageFlag::eComputeShader );
	}

	void RenderPanel::doCreateComputePipeline()
	{
		m_computePipelineLayout = m_device->createPipelineLayout( *m_computeDescriptorLayout );
		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";
		m_computeProgram = m_device->createShaderProgram();

		if ( !wxFileExists( shadersFolder / "shader.comp" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		m_computeProgram->createModule( common::parseShaderFile( *m_device, shadersFolder / "shader.comp" )
			, renderer::ShaderStageFlag::eCompute );

		m_computePipeline = m_computePipelineLayout->createPipeline( *m_computeProgram );
	}

	void RenderPanel::doPrepareCompute()
	{
		wxSize size{ GetClientSize() };
		m_computeQueryPool = m_device->createQueryPool( renderer::QueryType::eTimestamp
			, 2u
			, 0u );
		m_computeFence = m_device->createFence( renderer::FenceCreateFlag::eSignaled );
		m_computeCommandBuffer = m_device->getComputeCommandPool().createCommandBuffer();
		auto & commandBuffer = *m_computeCommandBuffer;

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
		{
			commandBuffer.resetQueryPool( *m_computeQueryPool
				, 0u
				, 2u );
			commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eTopOfPipe
				, *m_computeQueryPool
				, 0u );
			commandBuffer.bindPipeline( *m_computePipeline );
			commandBuffer.bindDescriptorSet( *m_computeDescriptorSet
				, *m_computePipelineLayout
				, renderer::PipelineBindPoint::eCompute );
			commandBuffer.dispatch( size.GetWidth() / 16u
				, size.GetHeight() / 16u
				, 1u );
			commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eBottomOfPipe
				, *m_computeQueryPool
				, 1u );
			commandBuffer.end();
		}
	}

	void RenderPanel::doCreateMainVertexBuffer()
	{
		m_mainVertexLayout = renderer::makeLayout< TexturedVertexData >( 0 );
		m_mainVertexLayout->createAttribute< renderer::Vec4 >( 0u
			, uint32_t( offsetof( TexturedVertexData, position ) ) );
		m_mainVertexLayout->createAttribute< renderer::Vec2 >( 1u
			, uint32_t( offsetof( TexturedVertexData, uv ) ) );

		m_mainVertexBuffer = renderer::makeVertexBuffer< TexturedVertexData >( *m_device
			, uint32_t( m_mainVertexData.size() )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_stagingBuffer->uploadVertexData( m_swapChain->getDefaultResources().getCommandBuffer()
			, m_mainVertexData
			, *m_mainVertexBuffer
			, renderer::PipelineStageFlag::eVertexInput );

		m_mainGeometryBuffers = m_device->createGeometryBuffers( *m_mainVertexBuffer
			, 0u
			, *m_mainVertexLayout );
	}

	void RenderPanel::doCreateMainPipeline()
	{
		m_mainPipelineLayout = m_device->createPipelineLayout( *m_mainDescriptorLayout );
		wxSize size{ GetClientSize() };
		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";
		m_mainProgram = m_device->createShaderProgram();

		if ( !wxFileExists( shadersFolder / "main.vert" )
			|| !wxFileExists( shadersFolder / "main.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		m_mainProgram->createModule( common::parseShaderFile( *m_device, shadersFolder / "main.vert" )
			, renderer::ShaderStageFlag::eVertex );
		m_mainProgram->createModule( common::parseShaderFile( *m_device, shadersFolder / "main.frag" )
			, renderer::ShaderStageFlag::eFragment );

		m_mainPipeline = m_mainPipelineLayout->createPipeline( renderer::GraphicsPipelineCreateInfo
		{
			*m_mainProgram,
			*m_mainRenderPass,
			renderer::VertexInputState::create( *m_mainVertexLayout ),
			renderer::InputAssemblyState{ renderer::PrimitiveTopology::eTriangleStrip },
			renderer::RasterisationState{ 1.0f }
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

			if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
			{
				auto dimensions = m_swapChain->getDimensions();
				commandBuffer.beginRenderPass( *m_mainRenderPass
					, frameBuffer
					, { renderer::ClearValue{ { 1.0, 0.0, 0.0, 1.0 } } }
				, renderer::SubpassContents::eInline );
				commandBuffer.bindPipeline( *m_mainPipeline );
				commandBuffer.setViewport( { uint32_t( dimensions.x )
					, uint32_t( dimensions.y )
					, 0
					, 0 } );
				commandBuffer.setScissor( { 0
					, 0
					, uint32_t( dimensions.x )
					, uint32_t( dimensions.y ) } );
				commandBuffer.bindGeometryBuffers( *m_mainGeometryBuffers );
				commandBuffer.bindDescriptorSet( *m_mainDescriptorSet
					, *m_mainPipelineLayout );
				commandBuffer.draw( 4u );
				commandBuffer.endRenderPass();

				auto res = commandBuffer.end();

				if ( !res )
				{
					std::stringstream stream;
					stream << "Command buffers recording failed.";
					throw std::runtime_error{ stream.str() };
				}
			}
		}
	}

	void RenderPanel::doUpdate()
	{
		static renderer::Mat4 const originalTranslate1 = []()
		{
			renderer::Mat4 result;
			result = utils::translate( result, { 2, 0, -5 } );
			return result;
		}( );
		static renderer::Mat4 const originalTranslate2 = []()
		{
			renderer::Mat4 result;
			result = utils::translate( result, { -2, 0, -5 } );
			return result;
		}( );
		static renderer::Mat4 const originalRotate = []()
		{
			renderer::Mat4 result;
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
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_objectUbo->getDatas()
			, *m_objectUbo
			, renderer::PipelineStageFlag::eVertexShader );
		m_computeUbo->getData( 0u ).roll = ( m_frame % GetClientSize().GetWidth() ) * 0.03f;
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_computeUbo->getDatas()
			, *m_computeUbo
			, renderer::PipelineStageFlag::eComputeShader );
		++m_frame;
	}

	void RenderPanel::doDraw()
	{
		auto resources = m_swapChain->getResources();

		if ( resources )
		{
			auto before = std::chrono::high_resolution_clock::now();
			auto & queue = m_device->getGraphicsQueue();
			auto res = queue.submit( *m_commandBuffer
				, nullptr );

			if ( res )
			{
				m_computeFence->wait( ~( 0u ) );
				m_computeFence->reset();
				res = m_device->getComputeQueue().submit( *m_computeCommandBuffer
					, m_computeFence.get() );
			}

			if ( res )
			{
				auto res = queue.submit( *m_commandBuffers[resources->getBackBuffer()]
					, resources->getImageAvailableSemaphore()
					, renderer::PipelineStageFlag::eColourAttachmentOutput
					, resources->getRenderingFinishedSemaphore()
					, &resources->getFence() );
				m_swapChain->present( *resources );
				renderer::UInt32Array values1{ 0u, 0u };
				renderer::UInt32Array values2{ 0u, 0u };
				m_offscreenQueryPool->getResults( 0u
					, 2u
					, 0u
					, renderer::QueryResultFlag::eWait
					, values1 );
				m_computeQueryPool->getResults( 0u
					, 2u
					, 0u
					, renderer::QueryResultFlag::eWait
					, values1 );

				// Elapsed time in nanoseconds
				auto elapsed1 = std::chrono::nanoseconds{ uint64_t( ( values1[1] - values1[0] ) / float( m_device->getTimestampPeriod() ) ) };
				auto elapsed2 = std::chrono::nanoseconds{ uint64_t( ( values2[1] - values2[0] ) / float( m_device->getTimestampPeriod() ) ) };
				auto after = std::chrono::high_resolution_clock::now();
				wxGetApp().updateFps( std::chrono::duration_cast< std::chrono::microseconds >( elapsed1 + elapsed2 )
					, std::chrono::duration_cast< std::chrono::microseconds >( after - before ) );
			}
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
		m_swapChain->reset( { size.GetWidth(), size.GetHeight() } );
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
