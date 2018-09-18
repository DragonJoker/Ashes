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
#include <Core/Renderer.hpp>
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
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <Shader/GlslToSpv.hpp>
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
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, ashes::Renderer const & renderer )
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
			doCreateDevice( renderer );
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
			m_renderTargetColourView.reset();
			m_renderTargetColour.reset();

			m_swapChain.reset();
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
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_matrixUbo->getDatas()
			, *m_matrixUbo
			, ashes::PipelineStageFlag::eVertexShader );
	}

	void RenderPanel::doCreateDevice( ashes::Renderer const & renderer )
	{
		m_device = renderer.createDevice( common::makeConnection( this, renderer ) );
	}

	void RenderPanel::doCreateSwapChain()
	{
		wxSize size{ GetClientSize() };
		m_swapChain = m_device->createSwapChain( { uint32_t( size.x ), uint32_t( size.y ) } );
		m_swapChain->setClearColour( { 1.0f, 0.8f, 0.4f, 0.0f } );
		m_swapChainReset = m_swapChain->onReset.connect( [this]()
		{
			doCreateFrameBuffer();
			doPrepareOffscreenFrame();
			doCreateMainDescriptorSet();
			doPrepareMainFrames();
		} );
		m_updateCommandBuffer = m_device->getGraphicsCommandPool().createCommandBuffer();
	}

	void RenderPanel::doCreateTexture()
	{
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
		auto stagingTexture = m_device->createStagingTexture( ashes::Format::eR8G8B8A8_UNORM
			, { 512u, 512u } );
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
			stagingTexture->uploadTextureData( m_swapChain->getDefaultResources().getCommandBuffer()
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
		m_stagingBuffer->uploadVertexData( m_swapChain->getDefaultResources().getCommandBuffer()
			, m_offscreenVertexData
			, *m_offscreenVertexBuffer );

		m_offscreenIndexBuffer = ashes::makeBuffer< uint16_t >( *m_device
			, uint32_t( m_offscreenIndexData.size() )
			, ashes::BufferTarget::eIndexBuffer | ashes::BufferTarget::eTransferDst
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_stagingBuffer->uploadBufferData( m_swapChain->getDefaultResources().getCommandBuffer()
			, m_offscreenIndexData
			, *m_offscreenIndexBuffer );
	}

	void RenderPanel::doCreateOffscreenPipeline()
	{
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
			{ ashes::DynamicState::eViewport, ashes::DynamicState::eScissor }
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
			, *m_renderTargetColourView
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
		m_commandBuffer = m_device->getGraphicsCommandPool().createCommandBuffer();
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
		m_stagingBuffer->uploadVertexData( m_swapChain->getDefaultResources().getCommandBuffer()
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
			{ ashes::DynamicState::eViewport, ashes::DynamicState::eScissor }
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
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
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
			auto & queue = m_device->getGraphicsQueue();
			queue.submit( *m_commandBuffer
				, nullptr );
			queue.waitIdle();

			queue.submit( *m_commandBuffers[resources->getBackBuffer()]
				, resources->getImageAvailableSemaphore()
				, ashes::PipelineStageFlag::eColourAttachmentOutput
				, resources->getRenderingFinishedSemaphore()
				, &resources->getFence() );
			m_swapChain->present( *resources );
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
