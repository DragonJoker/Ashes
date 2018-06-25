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
#include <Shader/ShaderProgram.hpp>
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
		, renderer::Renderer const & renderer )
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

			for ( auto & sampler : m_blurSamplers )
			{
				sampler.reset();
			}

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
			m_device->disable();
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
		m_swapChain = m_device->createSwapChain( { uint32_t( size.x ), uint32_t( size.y ) } );
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
		m_updateCommandBuffer = m_device->getGraphicsCommandPool().createCommandBuffer();
	}

	void RenderPanel::doCreateTexture()
	{
		m_texture = m_device->createTexture(
			{
				renderer::ImageCreateFlag::eCubeCompatible,
				renderer::TextureType::e2D,
				renderer::Format::eR8G8B8A8_UNORM,
				{ 512u, 512u, 1u },
				1u,
				6u,
				renderer::SampleCountFlag::e1,
				renderer::ImageTiling::eOptimal,
				renderer::ImageUsageFlag::eTransferDst | renderer::ImageUsageFlag::eSampled
			}
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_view = m_texture->createView( renderer::TextureViewType::eCube
			, renderer::Format::eR8G8B8A8_UNORM
			, 0u
			, 1u
			, 0u
			, 6u );
		m_sampler = m_device->createSampler( renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::Filter::eLinear
			, renderer::Filter::eLinear );

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
			m_stagingBuffer->uploadTextureData( m_swapChain->getDefaultResources().getCommandBuffer()
				, {
					m_view->getSubResourceRange().aspectMask,
					m_view->getSubResourceRange().baseMipLevel,
					uint32_t( i ),
					1u,
				}
				, { 0, 0, 0 }
				, { image.size.width, image.size.height, 1u }
				, image.data
				, *m_view );
		}
	}

	void RenderPanel::doCreateUniformBuffer()
	{
		m_matrixUbo = std::make_unique< renderer::UniformBuffer< utils::Mat4 > >( *m_device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_objectUbo = std::make_unique< renderer::UniformBuffer< utils::Mat4 > >( *m_device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
	}

	void RenderPanel::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique< renderer::StagingBuffer >( *m_device
			, 0u
			, 20000000u );
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
		renderer::AttachmentDescriptionArray attaches
		{
			{
				renderer::Format::eR8G8B8A8_UNORM,
				renderer::SampleCountFlag::e1,
				renderer::AttachmentLoadOp::eClear,
				renderer::AttachmentStoreOp::eStore,
				renderer::AttachmentLoadOp::eDontCare,
				renderer::AttachmentStoreOp::eDontCare,
				renderer::ImageLayout::eUndefined,
				renderer::ImageLayout::eShaderReadOnlyOptimal,
			}
		};
		renderer::AttachmentReferenceArray subAttaches
		{
			{ 0u, renderer::ImageLayout::eColourAttachmentOptimal }
		};
		renderer::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( std::make_unique< renderer::RenderSubpass >( renderer::PipelineBindPoint::eGraphics
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite }
			, subAttaches ) );
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
		m_renderTargetColour = m_device->createTexture(
			{
				0u,
				renderer::TextureType::e2D,
				renderer::Format::eR8G8B8A8_UNORM,
				{ uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ), 1u },
				1u,
				1u,
				renderer::SampleCountFlag::e1,
				renderer::ImageTiling::eOptimal,
				renderer::ImageUsageFlag::eColourAttachment | renderer::ImageUsageFlag::eSampled
			}
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_renderTargetColourView = m_renderTargetColour->createView( renderer::TextureViewType::e2D
			, m_renderTargetColour->getFormat() );
		renderer::FrameBufferAttachmentArray attaches;
		attaches.emplace_back( *( m_offscreenRenderPass->getAttachments().begin() + 0u ), *m_renderTargetColourView );
		m_frameBuffer = m_offscreenRenderPass->createFrameBuffer( { uint32_t( size.GetWidth() ), uint32_t( size.GetHeight() ) }
			, std::move( attaches ) );
	}

	void RenderPanel::doCreateOffscreenVertexBuffer()
	{
		m_offscreenVertexLayout = renderer::makeLayout< NonTexturedVertexData >( 0 );
		m_offscreenVertexLayout->createAttribute( 0u
			, renderer::Format::eR32G32B32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, position ) ) );

		m_offscreenVertexBuffer = renderer::makeVertexBuffer< NonTexturedVertexData >( *m_device
			, uint32_t( m_offscreenVertexData.size() )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_stagingBuffer->uploadVertexData( m_swapChain->getDefaultResources().getCommandBuffer()
			, m_offscreenVertexData
			, *m_offscreenVertexBuffer );

		m_offscreenIndexBuffer = renderer::makeBuffer< uint16_t >( *m_device
			, uint32_t( m_offscreenIndexData.size() )
			, renderer::BufferTarget::eIndexBuffer | renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
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

		std::vector< renderer::ShaderStageState > shaderStages;
		shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "offscreen.vert" ) );
		shaderStages[1].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "offscreen.frag" ) );
		renderer::RasterisationState rasterisationState;
		rasterisationState.cullMode = renderer::CullModeFlag::eNone;

		m_offscreenPipeline = m_offscreenPipelineLayout->createPipeline( renderer::GraphicsPipelineCreateInfo
		{
			std::move( shaderStages ),
			*m_offscreenRenderPass,
			renderer::VertexInputState::create( *m_offscreenVertexLayout ),
			renderer::InputAssemblyState{ renderer::PrimitiveTopology::eTriangleList },
			rasterisationState,
			renderer::MultisampleState{},
			renderer::ColourBlendState::createDefault(),
			{ renderer::DynamicState::eViewport, renderer::DynamicState::eScissor }
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
			, *m_passes.combine.views[0]
			, *m_sampler );
		m_mainDescriptorSet->update();
	}

	void RenderPanel::doCreateMainRenderPass()
	{
		renderer::AttachmentDescriptionArray attaches
		{
			{
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
		renderer::AttachmentReferenceArray subAttaches
		{
			{ 0u, renderer::ImageLayout::eColourAttachmentOptimal }
		};
		renderer::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( std::make_unique< renderer::RenderSubpass >( renderer::PipelineBindPoint::eGraphics
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
		m_queryPool = m_device->createQueryPool( renderer::QueryType::eTimestamp
			, 2u
			, 0u );
		m_commandBuffer = m_device->getGraphicsCommandPool().createCommandBuffer();
		wxSize size{ GetClientSize() };
		auto & commandBuffer = *m_commandBuffer;
		auto & frameBuffer = *m_frameBuffer;

		commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse );
		auto dimensions = m_swapChain->getDimensions();
		commandBuffer.resetQueryPool( *m_queryPool
			, 0u
			, 2u );
		commandBuffer.beginRenderPass( *m_offscreenRenderPass
			, frameBuffer
			, { renderer::ClearValue{ m_swapChain->getClearColour() } }
			, renderer::SubpassContents::eInline );
		commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eTopOfPipe
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
		commandBuffer.bindIndexBuffer( m_offscreenIndexBuffer->getBuffer(), 0u, renderer::IndexType::eUInt16 );
		commandBuffer.bindDescriptorSet( *m_offscreenDescriptorSet
			, *m_offscreenPipelineLayout );
		commandBuffer.drawIndexed( uint32_t( m_offscreenIndexData.size() ) );
		commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eBottomOfPipe
			, *m_queryPool
			, 1u );
		commandBuffer.endRenderPass();
		commandBuffer.end();
	}

	void RenderPanel::doPrepareHiPass()
	{
		auto dimensions = m_swapChain->getDimensions();
		std::vector< renderer::DescriptorSetLayoutBinding > bindings
		{
			renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
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

		renderer::ImageCreateInfo image{};
		image.arrayLayers = 1u;
		image.extent = renderer::Extent3D{ dimensions.width, dimensions.height, 1u };
		image.flags = 0u;
		image.format = m_renderTargetColourView->getFormat();
		image.imageType = renderer::TextureType::e2D;
		image.initialLayout = renderer::ImageLayout::eUndefined;
		image.mipLevels = uint32_t( m_passes.blurX.size() );
		image.samples = renderer::SampleCountFlag::e1;
		image.tiling = renderer::ImageTiling::eOptimal;
		image.usage = renderer::ImageUsageFlag::eColourAttachment
			| renderer::ImageUsageFlag::eSampled
			| renderer::ImageUsageFlag::eTransferDst
			| renderer::ImageUsageFlag::eTransferSrc;
		m_passes.hi.image = m_device->createTexture( image, renderer::MemoryPropertyFlag::eDeviceLocal );

		renderer::ImageViewCreateInfo view{};
		view.format = image.format;
		view.viewType = renderer::TextureViewType::e2D;
		view.subresourceRange.aspectMask = renderer::ImageAspectFlag::eColour;
		view.subresourceRange.baseArrayLayer = 0u;
		view.subresourceRange.layerCount = 1u;
		view.subresourceRange.levelCount = 1u;

		for ( auto i = 0u; i < m_passes.blurX.size(); ++i )
		{
			view.subresourceRange.baseMipLevel = uint32_t( i );
			m_passes.hi.views.emplace_back( m_passes.hi.image->createView( view ) );
		}

		renderer::RenderPassCreateInfo renderPass{};
		renderPass.attachments.resize( 1u );
		renderPass.attachments[0].format = image.format;
		renderPass.attachments[0].loadOp = renderer::AttachmentLoadOp::eClear;
		renderPass.attachments[0].storeOp = renderer::AttachmentStoreOp::eStore;
		renderPass.attachments[0].stencilLoadOp = renderer::AttachmentLoadOp::eDontCare;
		renderPass.attachments[0].stencilStoreOp = renderer::AttachmentStoreOp::eDontCare;
		renderPass.attachments[0].samples = renderer::SampleCountFlag::e1;
		renderPass.attachments[0].initialLayout = renderer::ImageLayout::eUndefined;
		renderPass.attachments[0].finalLayout = renderer::ImageLayout::eColourAttachmentOptimal;

		renderPass.subpasses.resize( 1u );
		renderPass.subpasses[0].colorAttachments = { { 0u, renderer::ImageLayout::eColourAttachmentOptimal } };

		renderPass.dependencies.resize( 2u );
		renderPass.dependencies[0].srcSubpass = renderer::ExternalSubpass;
		renderPass.dependencies[0].dstSubpass = 0u;
		renderPass.dependencies[0].dependencyFlags = renderer::DependencyFlag::eByRegion;
		renderPass.dependencies[0].srcAccessMask = renderer::AccessFlag::eMemoryRead;
		renderPass.dependencies[0].dstAccessMask = renderer::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[0].srcStageMask = renderer::PipelineStageFlag::eHost;
		renderPass.dependencies[0].dstStageMask = renderer::PipelineStageFlag::eColourAttachmentOutput;

		renderPass.dependencies[1].srcSubpass = 0u;
		renderPass.dependencies[1].dstSubpass = renderer::ExternalSubpass;
		renderPass.dependencies[1].dependencyFlags = renderer::DependencyFlag::eByRegion;
		renderPass.dependencies[1].srcAccessMask = renderer::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[1].dstAccessMask = renderer::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[1].srcStageMask = renderer::PipelineStageFlag::eColourAttachmentOutput;
		renderPass.dependencies[1].dstStageMask = renderer::PipelineStageFlag::eColourAttachmentOutput;

		m_passes.hi.renderPass = m_device->createRenderPass( renderPass );

		renderer::FrameBufferAttachmentArray attaches
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

		renderer::ShaderStageStateArray shaderStages;
		shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "hipass.vert" ) );
		shaderStages[1].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "hipass.frag" ) );

		renderer::GraphicsPipelineCreateInfo pipeline
		{
			shaderStages,
			*m_passes.hi.renderPass,
			renderer::VertexInputState::create( *m_mainVertexLayout ),
			renderer::InputAssemblyState{ renderer::PrimitiveTopology::eTriangleStrip },
			renderer::RasterisationState{},
			renderer::MultisampleState{},
			renderer::ColourBlendState::createDefault(),
			{},
			std::nullopt,
			std::nullopt,
			renderer::Viewport{ { 0, 0 }, dimensions },
			renderer::Scissor{ { 0, 0 }, dimensions }
		};
		m_passes.hi.pipeline = m_passes.hi.pipelineLayout->createPipeline( pipeline );

		m_passes.hi.commandBuffer = m_device->getGraphicsCommandPool().createCommandBuffer( true );
		auto & cmd = *m_passes.hi.commandBuffer;

		cmd.begin();
		cmd.beginRenderPass( *m_passes.hi.renderPass
			, *m_passes.hi.frameBuffer
			, { renderer::ClearColorValue{ 0.0, 0.0, 0.0, 0.0 } }
			, renderer::SubpassContents::eInline );
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
		m_blurConfiguration = renderer::makeUniformBuffer< Configuration >( *m_device
			, uint32_t( m_passes.blurX.size() )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eHostVisible );
		auto coefficientsCount = 5u;
		auto kernel = doCreateKernel( coefficientsCount );

		for ( auto i = 0u; i < uint32_t( m_passes.blurX.size() ); ++i )
		{
			auto & data = m_blurConfiguration->getData( i );
			data.textureSize = { dimensions.width >> i, dimensions.height >> i };
			data.coefficientsCount = coefficientsCount;
			data.coefficients = kernel;

			m_blurSamplers[i] = m_device->createSampler( renderer::WrapMode::eClampToBorder
				, renderer::WrapMode::eClampToBorder
				, renderer::WrapMode::eClampToBorder
				, renderer::Filter::eNearest
				, renderer::Filter::eNearest
				, renderer::MipmapMode::eNearest
				, float( i )
				, float( i + 1u ) );
		}

		m_blurConfiguration->upload( 0u, uint32_t( m_passes.blurX.size() ) );

		renderer::ImageCreateInfo image{};
		image.arrayLayers = 1u;
		image.extent = renderer::Extent3D{ dimensions.width, dimensions.height, 1u };
		image.flags = 0u;
		image.format = m_renderTargetColourView->getFormat();
		image.imageType = renderer::TextureType::e2D;
		image.initialLayout = renderer::ImageLayout::eUndefined;
		image.mipLevels = uint32_t( m_passes.blurX.size() );
		image.samples = renderer::SampleCountFlag::e1;
		image.tiling = renderer::ImageTiling::eOptimal;
		image.usage = renderer::ImageUsageFlag::eColourAttachment
			| renderer::ImageUsageFlag::eSampled
			| renderer::ImageUsageFlag::eTransferDst
			| renderer::ImageUsageFlag::eTransferSrc;
		m_passes.blurX[0].image = m_device->createTexture( image, renderer::MemoryPropertyFlag::eDeviceLocal );

		renderer::RenderPassCreateInfo renderPass{};
		renderPass.attachments.resize( 1u );
		renderPass.attachments[0].format = image.format;
		renderPass.attachments[0].loadOp = renderer::AttachmentLoadOp::eClear;
		renderPass.attachments[0].storeOp = renderer::AttachmentStoreOp::eStore;
		renderPass.attachments[0].stencilLoadOp = renderer::AttachmentLoadOp::eDontCare;
		renderPass.attachments[0].stencilStoreOp = renderer::AttachmentStoreOp::eDontCare;
		renderPass.attachments[0].samples = renderer::SampleCountFlag::e1;
		renderPass.attachments[0].initialLayout = renderer::ImageLayout::eUndefined;
		renderPass.attachments[0].finalLayout = renderer::ImageLayout::eShaderReadOnlyOptimal;

		renderPass.subpasses.resize( 1u );
		renderPass.subpasses[0].colorAttachments = { { 0u, renderer::ImageLayout::eColourAttachmentOptimal } };

		renderPass.dependencies.resize( 2u );
		renderPass.dependencies[0].srcSubpass = renderer::ExternalSubpass;
		renderPass.dependencies[0].dstSubpass = 0u;
		renderPass.dependencies[0].dependencyFlags = renderer::DependencyFlag::eByRegion;
		renderPass.dependencies[0].srcAccessMask = renderer::AccessFlag::eHostWrite;
		renderPass.dependencies[0].dstAccessMask = renderer::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[0].srcStageMask = renderer::PipelineStageFlag::eHost;
		renderPass.dependencies[0].dstStageMask = renderer::PipelineStageFlag::eColourAttachmentOutput;

		renderPass.dependencies[1].srcSubpass = 0u;
		renderPass.dependencies[1].dstSubpass = renderer::ExternalSubpass;
		renderPass.dependencies[1].dependencyFlags = renderer::DependencyFlag::eByRegion;
		renderPass.dependencies[1].srcAccessMask = renderer::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[1].dstAccessMask = renderer::AccessFlag::eShaderRead;
		renderPass.dependencies[1].srcStageMask = renderer::PipelineStageFlag::eColourAttachmentOutput;
		renderPass.dependencies[1].dstStageMask = renderer::PipelineStageFlag::eFragmentShader;

		m_passes.blurX[0].renderPass = m_device->createRenderPass( renderPass );

		for ( auto i = 0u; i < m_passes.blurX.size(); ++i )
		{
			auto & blur = m_passes.blurX[i];
			std::vector< renderer::DescriptorSetLayoutBinding > bindings
			{
				renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
				renderer::DescriptorSetLayoutBinding{ 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment },
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
			blur.descriptorSet->update();
			blur.semaphore = m_device->createSemaphore();
			blur.pipelineLayout = m_device->createPipelineLayout( *blur.descriptorLayout );

			renderer::ImageViewCreateInfo view{};
			view.format = image.format;
			view.viewType = renderer::TextureViewType::e2D;
			view.subresourceRange.aspectMask = renderer::ImageAspectFlag::eColour;
			view.subresourceRange.baseArrayLayer = 0u;
			view.subresourceRange.layerCount = 1u;
			view.subresourceRange.levelCount = 1u;
			view.subresourceRange.baseMipLevel = uint32_t( i );
			blur.views.emplace_back( m_passes.blurX[0].image->createView( view ) );

			renderer::FrameBufferAttachmentArray attaches
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

			renderer::ShaderStageStateArray shaderStages;
			shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eVertex ) } );
			shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eFragment )
				, renderer::makeSpecialisationInfo( { { 0u, 0u, renderer::ConstantFormat::eInt } }, int( 0 ) ) } );
			shaderStages[0].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "blur.vert" ) );
			shaderStages[1].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "blur.frag" ) );

			renderer::GraphicsPipelineCreateInfo pipeline
			{
				shaderStages,
				*m_passes.blurX[0].renderPass,
				renderer::VertexInputState::create( *m_mainVertexLayout ),
				renderer::InputAssemblyState{ renderer::PrimitiveTopology::eTriangleStrip },
				renderer::RasterisationState{},
				renderer::MultisampleState{},
				renderer::ColourBlendState::createDefault(),
				{},
				std::nullopt,
				std::nullopt,
				renderer::Viewport{ { 0, 0 }, dimensions },
				renderer::Scissor{ { 0, 0 }, dimensions }
			};
			blur.pipeline = blur.pipelineLayout->createPipeline( pipeline );

			blur.commandBuffer = m_device->getGraphicsCommandPool().createCommandBuffer( true );
			auto & cmd = *blur.commandBuffer;

			cmd.begin();
			cmd.beginRenderPass( *m_passes.blurX[0].renderPass
				, *blur.frameBuffer
				, { renderer::ClearColorValue{ 0.0, 0.0, 0.0, 0.0 } }
			, renderer::SubpassContents::eInline );
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
		renderer::RenderPassCreateInfo renderPass{};
		renderPass.attachments.resize( 1u );
		renderPass.attachments[0].format = m_passes.hi.views[0]->getFormat();
		renderPass.attachments[0].loadOp = renderer::AttachmentLoadOp::eClear;
		renderPass.attachments[0].storeOp = renderer::AttachmentStoreOp::eStore;
		renderPass.attachments[0].stencilLoadOp = renderer::AttachmentLoadOp::eDontCare;
		renderPass.attachments[0].stencilStoreOp = renderer::AttachmentStoreOp::eDontCare;
		renderPass.attachments[0].samples = renderer::SampleCountFlag::e1;
		renderPass.attachments[0].initialLayout = renderer::ImageLayout::eUndefined;
		renderPass.attachments[0].finalLayout = renderer::ImageLayout::eShaderReadOnlyOptimal;

		renderPass.subpasses.resize( 1u );
		renderPass.subpasses[0].colorAttachments = { { 0u, renderer::ImageLayout::eColourAttachmentOptimal } };

		renderPass.dependencies.resize( 2u );
		renderPass.dependencies[0].srcSubpass = renderer::ExternalSubpass;
		renderPass.dependencies[0].dstSubpass = 0u;
		renderPass.dependencies[0].dependencyFlags = renderer::DependencyFlag::eByRegion;
		renderPass.dependencies[0].srcAccessMask = renderer::AccessFlag::eHostWrite;
		renderPass.dependencies[0].dstAccessMask = renderer::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[0].srcStageMask = renderer::PipelineStageFlag::eHost;
		renderPass.dependencies[0].dstStageMask = renderer::PipelineStageFlag::eColourAttachmentOutput;

		renderPass.dependencies[1].srcSubpass = 0u;
		renderPass.dependencies[1].dstSubpass = renderer::ExternalSubpass;
		renderPass.dependencies[1].dependencyFlags = renderer::DependencyFlag::eByRegion;
		renderPass.dependencies[1].srcAccessMask = renderer::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[1].dstAccessMask = renderer::AccessFlag::eShaderRead;
		renderPass.dependencies[1].srcStageMask = renderer::PipelineStageFlag::eColourAttachmentOutput;
		renderPass.dependencies[1].dstStageMask = renderer::PipelineStageFlag::eFragmentShader;

		m_passes.blurY[0].renderPass = m_device->createRenderPass( renderPass );

		for ( auto i = 0u; i < m_passes.blurY.size(); ++i )
		{
			auto & blur = m_passes.blurY[i];
			std::vector< renderer::DescriptorSetLayoutBinding > bindings
			{
				renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
				renderer::DescriptorSetLayoutBinding{ 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment },
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
			blur.descriptorSet->update();
			blur.semaphore = m_device->createSemaphore();
			blur.pipelineLayout = m_device->createPipelineLayout( *blur.descriptorLayout );

			renderer::FrameBufferAttachmentArray attaches
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

			renderer::ShaderStageStateArray shaderStages;
			shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eVertex ) } );
			shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eFragment )
				, renderer::makeSpecialisationInfo( { { 0u, 0u, renderer::ConstantFormat::eInt } }, int( 1 ) ) } );
			shaderStages[0].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "blur.vert" ) );
			shaderStages[1].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "blur.frag" ) );

			renderer::GraphicsPipelineCreateInfo pipeline
			{
				shaderStages,
				*m_passes.blurY[0].renderPass,
				renderer::VertexInputState::create( *m_mainVertexLayout ),
				renderer::InputAssemblyState{ renderer::PrimitiveTopology::eTriangleStrip },
				renderer::RasterisationState{},
				renderer::MultisampleState{},
				renderer::ColourBlendState::createDefault(),
				{},
				std::nullopt,
				std::nullopt,
				renderer::Viewport{ { 0, 0 }, dimensions },
				renderer::Scissor{ { 0, 0 }, dimensions }
			};
			blur.pipeline = blur.pipelineLayout->createPipeline( pipeline );

			blur.commandBuffer = m_device->getGraphicsCommandPool().createCommandBuffer( true );
			auto & cmd = *blur.commandBuffer;

			cmd.begin();
			cmd.beginRenderPass( *m_passes.blurY[0].renderPass
				, *blur.frameBuffer
				, { renderer::ClearColorValue{ 0.0, 0.0, 0.0, 0.0 } }
			, renderer::SubpassContents::eInline );
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
		renderer::ImageViewCreateInfo view{};
		view.format = m_renderTargetColourView->getFormat();
		view.viewType = renderer::TextureViewType::e2D;
		view.subresourceRange.aspectMask = renderer::ImageAspectFlag::eColour;
		view.subresourceRange.baseArrayLayer = 0u;
		view.subresourceRange.layerCount = 1u;
		view.subresourceRange.levelCount = uint32_t( m_passes.blurY.size() );
		view.subresourceRange.baseMipLevel = 0u;
		m_blurMipView = m_passes.hi.image->createView( view );

		m_mipSampler = m_device->createSampler( renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::Filter::eLinear
			, renderer::Filter::eLinear
			, renderer::MipmapMode::eNearest
			, 0.0f
			, float( m_passes.blurY.size() ) );

		auto dimensions = m_swapChain->getDimensions();
		std::vector< renderer::DescriptorSetLayoutBinding > bindings
		{
			renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
			renderer::DescriptorSetLayoutBinding{ 1u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
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

		renderer::ImageCreateInfo image{};
		image.arrayLayers = 1u;
		image.extent = renderer::Extent3D{ dimensions.width, dimensions.height, 1u };
		image.flags = 0u;
		image.format = m_renderTargetColourView->getFormat();
		image.imageType = renderer::TextureType::e2D;
		image.initialLayout = renderer::ImageLayout::eUndefined;
		image.mipLevels = uint32_t( m_passes.blurX.size() );
		image.samples = renderer::SampleCountFlag::e1;
		image.tiling = renderer::ImageTiling::eOptimal;
		image.usage = renderer::ImageUsageFlag::eColourAttachment
			| renderer::ImageUsageFlag::eSampled
			| renderer::ImageUsageFlag::eTransferDst
			| renderer::ImageUsageFlag::eTransferSrc;
		m_passes.combine.image = m_device->createTexture( image, renderer::MemoryPropertyFlag::eDeviceLocal );

		view = renderer::ImageViewCreateInfo{};
		view.format = image.format;
		view.viewType = renderer::TextureViewType::e2D;
		view.subresourceRange.aspectMask = renderer::ImageAspectFlag::eColour;
		view.subresourceRange.baseArrayLayer = 0u;
		view.subresourceRange.layerCount = 1u;
		view.subresourceRange.levelCount = 1u;
		view.subresourceRange.baseMipLevel = 0u;
		m_passes.combine.views.emplace_back( m_passes.combine.image->createView( view ) );

		renderer::RenderPassCreateInfo renderPass{};
		renderPass.attachments.resize( 1u );
		renderPass.attachments[0].format = image.format;
		renderPass.attachments[0].loadOp = renderer::AttachmentLoadOp::eClear;
		renderPass.attachments[0].storeOp = renderer::AttachmentStoreOp::eStore;
		renderPass.attachments[0].stencilLoadOp = renderer::AttachmentLoadOp::eDontCare;
		renderPass.attachments[0].stencilStoreOp = renderer::AttachmentStoreOp::eDontCare;
		renderPass.attachments[0].samples = renderer::SampleCountFlag::e1;
		renderPass.attachments[0].initialLayout = renderer::ImageLayout::eUndefined;
		renderPass.attachments[0].finalLayout = renderer::ImageLayout::eColourAttachmentOptimal;

		renderPass.subpasses.resize( 1u );
		renderPass.subpasses[0].colorAttachments = { { 0u, renderer::ImageLayout::eColourAttachmentOptimal } };

		renderPass.dependencies.resize( 2u );
		renderPass.dependencies[0].srcSubpass = renderer::ExternalSubpass;
		renderPass.dependencies[0].dstSubpass = 0u;
		renderPass.dependencies[0].dependencyFlags = renderer::DependencyFlag::eByRegion;
		renderPass.dependencies[0].srcAccessMask = renderer::AccessFlag::eHostWrite;
		renderPass.dependencies[0].dstAccessMask = renderer::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[0].srcStageMask = renderer::PipelineStageFlag::eHost;
		renderPass.dependencies[0].dstStageMask = renderer::PipelineStageFlag::eColourAttachmentOutput;

		renderPass.dependencies[1].srcSubpass = 0u;
		renderPass.dependencies[1].dstSubpass = renderer::ExternalSubpass;
		renderPass.dependencies[1].dependencyFlags = renderer::DependencyFlag::eByRegion;
		renderPass.dependencies[1].srcAccessMask = renderer::AccessFlag::eColourAttachmentWrite;
		renderPass.dependencies[1].dstAccessMask = renderer::AccessFlag::eShaderRead;
		renderPass.dependencies[1].srcStageMask = renderer::PipelineStageFlag::eColourAttachmentOutput;
		renderPass.dependencies[1].dstStageMask = renderer::PipelineStageFlag::eFragmentShader;

		m_passes.combine.renderPass = m_device->createRenderPass( renderPass );

		renderer::FrameBufferAttachmentArray attaches
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

		renderer::ShaderStageStateArray shaderStages;
		shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "combine.vert" ) );
		shaderStages[1].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "combine.frag" ) );

		renderer::GraphicsPipelineCreateInfo pipeline
		{
			shaderStages,
			*m_passes.combine.renderPass,
			renderer::VertexInputState::create( *m_mainVertexLayout ),
			renderer::InputAssemblyState{ renderer::PrimitiveTopology::eTriangleStrip },
			renderer::RasterisationState{},
			renderer::MultisampleState{},
			renderer::ColourBlendState::createDefault(),
			{},
			std::nullopt,
			std::nullopt,
			renderer::Viewport{ { 0, 0 }, dimensions },
			renderer::Scissor{ { 0, 0 }, dimensions }
		};
		m_passes.combine.pipeline = m_passes.combine.pipelineLayout->createPipeline( pipeline );

		m_passes.combine.commandBuffer = m_device->getGraphicsCommandPool().createCommandBuffer( true );
		auto & cmd = *m_passes.combine.commandBuffer;

		cmd.begin();
		cmd.beginRenderPass( *m_passes.combine.renderPass
			, *m_passes.combine.frameBuffer
			, { renderer::ClearColorValue{ 0.0, 0.0, 0.0, 0.0 } }
			, renderer::SubpassContents::eInline );
		cmd.bindPipeline( *m_passes.combine.pipeline );
		cmd.bindDescriptorSet( *m_passes.combine.descriptorSet, *m_passes.combine.pipelineLayout );
		cmd.bindVertexBuffer( 0u, m_mainVertexBuffer->getBuffer(), 0u );
		cmd.draw( m_mainVertexBuffer->getCount() );
		cmd.endRenderPass();
		cmd.end();
	}

	void RenderPanel::doCreateMainVertexBuffer()
	{
		m_mainVertexLayout = renderer::makeLayout< TexturedVertexData >( 0 );
		m_mainVertexLayout->createAttribute( 0u
			, renderer::Format::eR32G32B32A32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, position ) ) );
		m_mainVertexLayout->createAttribute( 1u
			, renderer::Format::eR32G32_SFLOAT
			, uint32_t( offsetof( TexturedVertexData, uv ) ) );

		m_mainVertexBuffer = renderer::makeVertexBuffer< TexturedVertexData >( *m_device
			, uint32_t( m_mainVertexData.size() )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
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

		std::vector< renderer::ShaderStageState > shaderStages;
		shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eVertex ) } );
		shaderStages.push_back( { m_device->createShaderModule( renderer::ShaderStageFlag::eFragment ) } );
		shaderStages[0].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "main.vert" ) );
		shaderStages[1].module->loadShader( common::parseShaderFile( *m_device, shadersFolder / "main.frag" ) );

		m_mainPipeline = m_mainPipelineLayout->createPipeline( renderer::GraphicsPipelineCreateInfo
		{
			std::move( shaderStages ),
			*m_mainRenderPass,
			renderer::VertexInputState::create( *m_mainVertexLayout ),
			renderer::InputAssemblyState{ renderer::PrimitiveTopology::eTriangleStrip },
			renderer::RasterisationState{},
			renderer::MultisampleState{},
			renderer::ColourBlendState::createDefault(),
			{ renderer::DynamicState::eViewport, renderer::DynamicState::eScissor }
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

			commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse );
			auto dimensions = m_swapChain->getDimensions();
			commandBuffer.beginRenderPass( *m_mainRenderPass
				, frameBuffer
				, { renderer::ClearValue{ { 1.0, 0.0, 0.0, 1.0 } } }
				, renderer::SubpassContents::eInline );
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
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_objectUbo->getDatas()
			, *m_objectUbo
			, renderer::PipelineStageFlag::eVertexShader );
		save = std::chrono::high_resolution_clock::now();
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

			queue.submit( *m_passes.hi.commandBuffer
				, nullptr );
			queue.waitIdle();

			for ( auto & blur : m_passes.blurX )
			{
				queue.submit( *blur.commandBuffer
					, nullptr );
				queue.waitIdle();
			}

			for ( auto & blur : m_passes.blurY )
			{
				queue.submit( *blur.commandBuffer
					, nullptr );
				queue.waitIdle();
			}

			queue.submit( *m_passes.combine.commandBuffer
				, nullptr );
			queue.waitIdle();

			queue.submit( *m_commandBuffers[resources->getBackBuffer()]
				, resources->getImageAvailableSemaphore()
				, renderer::PipelineStageFlag::eColourAttachmentOutput
				, resources->getRenderingFinishedSemaphore()
				, &resources->getFence() );
			m_swapChain->present( *resources );
			renderer::UInt32Array values{ 0u, 0u };
			m_queryPool->getResults( 0u
				, 2u
				, 0u
				, renderer::QueryResultFlag::eWait
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
