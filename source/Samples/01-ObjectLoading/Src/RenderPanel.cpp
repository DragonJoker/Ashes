#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <Buffer/GeometryBuffers.hpp>
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
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderPassState.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <Utils/Transform.hpp>

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
		static renderer::PixelFormat const DepthFormat = renderer::PixelFormat::eD32F;
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, renderer::Renderer const & renderer )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
		, m_timer{ new wxTimer{ this, int( Ids::RenderTimer ) } }
		, m_mainVertexData
		{
			{ { -1.0, -1.0, 0.0, 1.0 }, { 0.0, 0.0 } },
			{ { -1.0, +1.0, 0.0, 1.0 }, { 0.0, 1.0 } },
			{ { +1.0, -1.0, 0.0, 1.0 }, { 1.0, 0.0 } },
			{ { +1.0, +1.0, 0.0, 1.0 }, { 1.0, 1.0 } },
		}
		, m_object{ common::loadObject( common::getPath( common::getExecutableDirectory() ) / "share" / "Assets" / "Nyra", "Nyra_pose.fbx", m_images ) }
	{
		try
		{
			doCreateDevice( renderer );
			std::cout << "Logical device created." << std::endl;
			doCreateSwapChain();
			std::cout << "Swap chain created." << std::endl;
			doCreateStagingBuffer();
			std::cout << "Staging buffer created." << std::endl;
			doCreateUniformBuffer();
			std::cout << "Uniform buffer created." << std::endl;
			doCreateOffscreenDescriptorSet();
			std::cout << "Offscreen descriptor set created." << std::endl;
			doCreateOffscreenRenderPass();
			std::cout << "Offscreen render pass created." << std::endl;
			doCreateOffscreenProgram();
			std::cout << "Offscreen pipeline created." << std::endl;
			doCreateTextures();
			std::cout << "Textures created." << std::endl;
			doInitialiseObject();
			std::cout << "Object created." << std::endl;
			doCreateFrameBuffer();
			std::cout << "Frame buffer created." << std::endl;
			doPrepareOffscreenFrame();
			std::cout << "Offscreen frame prepared." << std::endl;
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
			m_opaqueObject.clear();
			m_alphaBlendedObject.clear();
			m_submeshNodes.clear();
			m_textureNodes.clear();

			m_stagingBuffer.reset();

			m_materialUbo.reset();
			m_matrixUbo.reset();
			m_objectUbo.reset();
			m_mainDescriptorSet.reset();
			m_mainDescriptorPool.reset();
			m_mainDescriptorLayout.reset();
			m_mainPipeline.reset();
			m_mainPipelineLayout.reset();
			m_mainProgram.reset();
			m_mainVertexBuffer.reset();
			m_mainGeometryBuffers.reset();
			m_mainVertexLayout.reset();
			m_mainRenderPass.reset();

			m_offscreenQueryPool.reset();
			m_offscreenDescriptorPool.reset();
			m_offscreenDescriptorLayout.reset();
			m_offscreenProgram.reset();
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
			doCreateFrameBuffer();
			doPrepareOffscreenFrame();
			doCreateMainDescriptorSet();
			doPrepareMainFrames();
		} );
		m_updateCommandBuffer = m_device->getGraphicsCommandPool().createCommandBuffer();
	}

	void RenderPanel::doCreateTextures()
	{
		for ( auto & image : m_images )
		{
			common::TextureNodePtr textureNode = std::make_shared< common::TextureNode >();
			textureNode->image = image;
			textureNode->texture = m_device->createTexture();
			textureNode->texture->setImage( image->format
				, { image->size[0], image->size[1] }
				, 4u
				, renderer::ImageUsageFlag::eTransferSrc | renderer::ImageUsageFlag::eTransferDst | renderer::ImageUsageFlag::eSampled );
			textureNode->view = textureNode->texture->createView( textureNode->texture->getType()
				, textureNode->texture->getFormat()
				, 0u
				, 4u );
			auto view = textureNode->texture->createView( textureNode->texture->getType()
				, textureNode->texture->getFormat() );
			m_stagingBuffer->uploadTextureData( *m_updateCommandBuffer
				, image->data
				, *view );
			textureNode->texture->generateMipmaps();
			m_textureNodes.emplace_back( textureNode );
		}
	}

	void RenderPanel::doInitialiseObject()
	{
		m_sampler = m_device->createSampler( renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::Filter::eLinear
			, renderer::Filter::eLinear );
		uint32_t matIndex = 0u;

		for ( auto & submesh : m_object )
		{
			m_submeshNodes.push_back( std::make_shared< common::SubmeshNode >() );
			common::SubmeshNodePtr submeshNode = m_submeshNodes.back();

			// Initialise vertex layout.
			submeshNode->vertexLayout = m_device->createVertexLayout( 0u, sizeof( common::Vertex ) );
			submeshNode->vertexLayout->createAttribute< renderer::Vec3 >( 0u, offsetof( common::Vertex, position ) );
			submeshNode->vertexLayout->createAttribute< renderer::Vec3 >( 1u, offsetof( common::Vertex, normal ) );
			submeshNode->vertexLayout->createAttribute< renderer::Vec3 >( 2u, offsetof( common::Vertex, tangent ) );
			submeshNode->vertexLayout->createAttribute< renderer::Vec3 >( 3u, offsetof( common::Vertex, bitangent ) );
			submeshNode->vertexLayout->createAttribute< renderer::Vec2 >( 4u, offsetof( common::Vertex, texture ) );

			// Initialise geometry buffers.
			submeshNode->vbo = renderer::makeVertexBuffer< common::Vertex >( *m_device
				, uint32_t( submesh.vbo.data.size() )
				, renderer::BufferTarget::eTransferDst
				, renderer::MemoryPropertyFlag::eDeviceLocal );
			m_stagingBuffer->uploadVertexData( m_swapChain->getDefaultResources().getCommandBuffer()
				, submesh.vbo.data
				, *submeshNode->vbo
				, renderer::PipelineStageFlag::eVertexInput );
			submeshNode->ibo = renderer::makeBuffer< common::Face >( *m_device
				, uint32_t( submesh.ibo.data.size() )
				, renderer::BufferTarget::eTransferDst
				, renderer::MemoryPropertyFlag::eDeviceLocal );
			m_stagingBuffer->uploadBufferData( m_swapChain->getDefaultResources().getCommandBuffer()
				, submesh.ibo.data
				, *submeshNode->ibo );
			submeshNode->geometryBuffers = m_device->createGeometryBuffers( *submeshNode->vbo
				, 0u
				, *submeshNode->vertexLayout
				, submeshNode->ibo->getBuffer()
				, 0u
				, renderer::IndexType::eUInt32 );

			for ( auto & material : submesh.materials )
			{
				bool needsBlending = material.data.opacity < 1.0f;
				common::MaterialNode materialNode{ submeshNode };

				// Initialise material textures.
				for ( uint32_t index = 0u; index < material.data.texturesCount; ++index )
				{
					auto & texture = material.textures[index];
					auto it = std::find_if( m_textureNodes.begin()
						, m_textureNodes.end()
						, [&texture]( common::TextureNodePtr const & lookup )
						{
							return lookup->image == texture;
						} );
					assert( it != m_textureNodes.end() );
					materialNode.textures.push_back( *it );
				}

				m_materialUbo->getData( matIndex ) = material.data;

				// Initialise descriptor set for UBOs
				materialNode.descriptorSetUbos = m_offscreenDescriptorPool->createDescriptorSet( 0u );
				materialNode.descriptorSetUbos->createBinding( m_offscreenDescriptorLayout->getBinding( 0u )
					, *m_matrixUbo
					, 0u
					, 1u );
				materialNode.descriptorSetUbos->createBinding( m_offscreenDescriptorLayout->getBinding( 1u )
					, *m_objectUbo
					, 0u
					, 1u );
				materialNode.descriptorSetUbos->createBinding( m_offscreenDescriptorLayout->getBinding( 2u )
					, *m_materialUbo
					, matIndex
					, 1u );
				materialNode.descriptorSetUbos->update();

				// Initialise descriptor set for textures.
				renderer::DescriptorSetLayoutBindingArray bindings;
				bindings.emplace_back( 0u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment, 6u );
				materialNode.layout = m_device->createDescriptorSetLayout( std::move( bindings ) );
				materialNode.pool = materialNode.layout->createPool( 1u );
				materialNode.descriptorSetTextures = materialNode.pool->createDescriptorSet( 1u );

				for ( uint32_t index = 0u; index < material.data.texturesCount; ++index )
				{
					materialNode.descriptorSetTextures->createBinding( materialNode.layout->getBinding( 0u, index )
						, *materialNode.textures[index]->view
						, *m_sampler
						, index );
				}

				materialNode.descriptorSetTextures->update();

				// Initialise the pipeline
				renderer::RasterisationState rasterisationState;

				if ( material.data.backFace )
				{
					rasterisationState = renderer::RasterisationState{ 0u
						, false
						, false
						, renderer::PolygonMode::eFill
						, renderer::CullModeFlag::eFront };
				}

				if ( materialNode.layout )
				{
					materialNode.pipelineLayout = m_device->createPipelineLayout( { *m_offscreenDescriptorLayout, *materialNode.layout } );
				}
				else
				{
					materialNode.pipelineLayout = m_device->createPipelineLayout( *m_offscreenDescriptorLayout );
				}

				materialNode.pipeline = materialNode.pipelineLayout->createPipeline( *m_offscreenProgram
					, { *submeshNode->vertexLayout }
					, *m_offscreenRenderPass
					, renderer::PrimitiveTopology::eTriangleList
					, rasterisationState );
				materialNode.pipeline->multisampleState( renderer::MultisampleState{} );
				materialNode.pipeline->depthStencilState( renderer::DepthStencilState{} );
				materialNode.pipeline->finish();

				if ( needsBlending )
				{
					m_alphaBlendedObject.emplace_back( std::move( materialNode ) );
				}
				else
				{
					m_opaqueObject.emplace_back( std::move( materialNode ) );
				}

				++matIndex;
			}
		}

		// Fill Materials Ubo
		m_stagingBuffer->uploadUniformData( m_swapChain->getDefaultResources().getCommandBuffer()
			, m_materialUbo->getDatas()
			, *m_materialUbo
			, renderer::PipelineStageFlag::eFragmentShader );
	}

	void RenderPanel::doCreateUniformBuffer()
	{
		m_nodesCount = 0u;

		for ( auto & submesh : m_object )
		{
			m_nodesCount += uint32_t( submesh.materials.size() );
		}

		m_matrixUbo = std::make_unique< renderer::UniformBuffer< renderer::Mat4 > >( *m_device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_objectUbo = std::make_unique< renderer::UniformBuffer< renderer::Mat4 > >( *m_device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_materialUbo = std::make_unique< renderer::UniformBuffer< common::MaterialData > >( *m_device
			, m_nodesCount
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
	}

	void RenderPanel::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique< renderer::StagingBuffer >( *m_device
			, 0u
			, 200u * 1024u * 1024u );
	}

	void RenderPanel::doCreateOffscreenDescriptorSet()
	{
		std::vector< renderer::DescriptorSetLayoutBinding > bindings
		{
			renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex },
			renderer::DescriptorSetLayoutBinding{ 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex },
			renderer::DescriptorSetLayoutBinding{ 2u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment },
		};
		m_offscreenDescriptorLayout = m_device->createDescriptorSetLayout( std::move( bindings ) );
		m_offscreenDescriptorPool = m_offscreenDescriptorLayout->createPool( m_nodesCount );
	}

	void RenderPanel::doCreateOffscreenRenderPass()
	{
		std::vector< renderer::PixelFormat > formats{ { renderer::PixelFormat::eR8G8B8A8, DepthFormat } };
		renderer::RenderPassAttachmentArray attaches
		{
			{ renderer::PixelFormat::eR8G8B8A8, true },
			{ DepthFormat, true },
		};
		renderer::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( m_device->createRenderSubpass( formats
			, { renderer::PipelineStageFlag::eColourAttachmentOutput, renderer::AccessFlag::eColourAttachmentWrite } ) );
		m_offscreenRenderPass = m_device->createRenderPass( attaches
			, subpasses
			, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite
				, { renderer::ImageLayout::eColourAttachmentOptimal, renderer::ImageLayout::eDepthStencilAttachmentOptimal } }
			, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite
				, { renderer::ImageLayout::eColourAttachmentOptimal, renderer::ImageLayout::eDepthStencilAttachmentOptimal } } );
	}

	void RenderPanel::doCreateFrameBuffer()
	{
		auto size = GetClientSize();
		m_renderTargetColour = m_device->createTexture();
		m_renderTargetColour->setImage( renderer::PixelFormat::eR8G8B8A8
			, { size.GetWidth(), size.GetHeight() }
			, renderer::ImageUsageFlag::eColourAttachment | renderer::ImageUsageFlag::eSampled );
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

	void RenderPanel::doCreateOffscreenProgram()
	{
		wxSize size{ GetClientSize() };
		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";
		m_offscreenProgram = m_device->createShaderProgram();

		if ( !wxFileExists( shadersFolder / "offscreen.vert" )
			|| !wxFileExists( shadersFolder / "offscreen.frag" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		m_offscreenProgram->createModule( common::dumpTextFile( shadersFolder / "offscreen.vert" )
			, renderer::ShaderStageFlag::eVertex );
		m_offscreenProgram->createModule( common::dumpTextFile( shadersFolder / "offscreen.frag" )
			, renderer::ShaderStageFlag::eFragment );
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
		std::vector< renderer::PixelFormat > formats{ { m_swapChain->getFormat() } };
		renderer::RenderPassAttachmentArray attaches{ { m_swapChain->getFormat(), true } };
		renderer::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( m_device->createRenderSubpass( formats
			, { renderer::PipelineStageFlag::eColourAttachmentOutput, renderer::AccessFlag::eColourAttachmentWrite } ) );
		m_mainRenderPass = m_device->createRenderPass( attaches
			, subpasses
			, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite
				, { renderer::ImageLayout::eColourAttachmentOptimal } }
			, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite
				, { renderer::ImageLayout::eColourAttachmentOptimal } } );
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
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::PipelineStageFlag::eColourAttachmentOutput
				, m_renderTargetColourView->makeColourAttachment() );
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTopOfPipe
				, renderer::PipelineStageFlag::eEarlyFragmentTests
				, m_renderTargetDepthView->makeDepthStencilAttachment() );
			commandBuffer.beginRenderPass( *m_offscreenRenderPass
				, frameBuffer
				, { renderer::ClearValue{ m_swapChain->getClearColour() }, renderer::ClearValue{ renderer::DepthStencilClearValue{ 1.0f, 0u } } }
				, renderer::SubpassContents::eInline );
			commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eTopOfPipe
				, *m_offscreenQueryPool
				, 0u );

			for ( auto & node : m_opaqueObject )
			{
				commandBuffer.bindPipeline( *node.pipeline );
				commandBuffer.setViewport( { uint32_t( dimensions.x )
					, uint32_t( dimensions.y )
					, 0
					, 0 } );
				commandBuffer.setScissor( { 0
					, 0
					, uint32_t( dimensions.x )
					, uint32_t( dimensions.y ) } );
				commandBuffer.bindGeometryBuffers( *node.submesh->geometryBuffers );
				commandBuffer.bindDescriptorSet( *node.descriptorSetUbos
					, *node.pipelineLayout );
				commandBuffer.bindDescriptorSet( *node.descriptorSetTextures
					, *node.pipelineLayout );

				commandBuffer.drawIndexed( uint32_t( node.submesh->ibo->getBuffer().getSize() / sizeof( uint32_t ) ) );
			}

			for ( auto & node : m_alphaBlendedObject )
			{
				commandBuffer.bindPipeline( *node.pipeline );
				commandBuffer.setViewport( { uint32_t( dimensions.x )
					, uint32_t( dimensions.y )
					, 0
					, 0 } );
				commandBuffer.setScissor( { 0
					, 0
					, uint32_t( dimensions.x )
					, uint32_t( dimensions.y ) } );
				commandBuffer.bindGeometryBuffers( *node.submesh->geometryBuffers );
				commandBuffer.bindDescriptorSet( *node.descriptorSetUbos
					, *node.pipelineLayout );
				commandBuffer.bindDescriptorSet( *node.descriptorSetTextures
					, *node.pipelineLayout );

				commandBuffer.drawIndexed( uint32_t( node.submesh->ibo->getBuffer().getSize() / sizeof( uint32_t ) ) );
			}

			commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eBottomOfPipe
				, *m_offscreenQueryPool
				, 1u );
			commandBuffer.endRenderPass();
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::PipelineStageFlag::eBottomOfPipe
				, m_renderTargetColourView->makeShaderInputResource() );
			auto res = commandBuffer.end();

			if ( !res )
			{
				std::stringstream stream;
				stream << "Command buffers recording failed.";
				throw std::runtime_error{ stream.str() };
			}
		}
	}

	void RenderPanel::doCreateMainVertexBuffer()
	{
		m_mainVertexLayout = renderer::makeLayout< TexturedVertexData >( *m_device, 0 );
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

		m_mainProgram->createModule( common::dumpTextFile( shadersFolder / "main.vert" )
			, renderer::ShaderStageFlag::eVertex );
		m_mainProgram->createModule( common::dumpTextFile( shadersFolder / "main.frag" )
			, renderer::ShaderStageFlag::eFragment );

		m_mainPipeline = m_mainPipelineLayout->createPipeline( *m_mainProgram
			, { *m_mainVertexLayout }
			, *m_mainRenderPass
			, renderer::PrimitiveTopology::eTriangleStrip );
		m_mainPipeline->multisampleState( renderer::MultisampleState{} );
		m_mainPipeline->finish();
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
				m_swapChain->preRenderCommands( i, commandBuffer );
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
				m_swapChain->postRenderCommands( i, commandBuffer );

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
		static renderer::Mat4 const originalTranslate = []()
		{
			renderer::Mat4 result;
			result = utils::translate( result, { 0, 0, -5 } );
			return result;
		}();
		static renderer::Clock::time_point save = renderer::Clock::now();
		auto duration = std::chrono::duration_cast< std::chrono::microseconds >( renderer::Clock::now() - save );
		m_rotate = utils::rotate( m_rotate
			, float( utils::DegreeToRadian ) * ( duration.count() / 20000.0f )
			, { 0, 1, 0 } );
		m_objectUbo->getData( 0 ) = originalTranslate * m_rotate;
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_objectUbo->getDatas()
			, *m_objectUbo
			, renderer::PipelineStageFlag::eVertexShader );
		save = renderer::Clock::now();
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
				auto res = queue.submit( *m_commandBuffers[resources->getBackBuffer()]
					, resources->getImageAvailableSemaphore()
					, renderer::PipelineStageFlag::eColourAttachmentOutput
					, resources->getRenderingFinishedSemaphore()
					, &resources->getFence() );
				m_swapChain->present( *resources );
				renderer::UInt32Array values{ 0u, 0u };
				m_offscreenQueryPool->getResults( 0u
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
			auto before = std::chrono::high_resolution_clock::now();
			doUpdate();
			doDraw();
			auto after = std::chrono::high_resolution_clock::now();
			wxGetApp().updateFps( std::chrono::duration_cast< std::chrono::microseconds >( after - before ) );
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
