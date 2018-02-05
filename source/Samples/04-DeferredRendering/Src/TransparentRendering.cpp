#include "TransparentRendering.hpp"

#include <Buffer/Buffer.hpp>
#include <Buffer/GeometryBuffers.hpp>
#include <Buffer/StagingBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <Command/CommandBuffer.hpp>
#include <Command/CommandPool.hpp>
#include <Core/Device.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/Texture.hpp>
#include <Image/TextureView.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderPassState.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <RenderPass/TextureAttachment.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <FileUtils.hpp>

namespace vkapp
{
	namespace
	{
		renderer::ShaderProgramPtr doCreateProgram( renderer::Device const & device )
		{
			renderer::ShaderProgramPtr result = device.createShaderProgram();
			std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

			if ( false && result->isSPIRVSupported() )
			{
				if ( !wxFileExists( shadersFolder / "transparent_vert.spv" )
					|| !wxFileExists( shadersFolder / "transparent_frag.spv" ) )
				{
					throw std::runtime_error{ "Shader files are missing" };
				}

				result->createModule( common::dumpBinaryFile( shadersFolder / "transparent_vert.spv" )
					, renderer::ShaderStageFlag::eVertex );
				result->createModule( common::dumpBinaryFile( shadersFolder / "transparent_frag.spv" )
					, renderer::ShaderStageFlag::eFragment );
				result->link();
			}
			else
			{
				if ( !wxFileExists( shadersFolder / "transparent.vert" )
					|| !wxFileExists( shadersFolder / "transparent.frag" ) )
				{
					throw std::runtime_error{ "Shader files are missing" };
				}

				result->createModule( common::dumpTextFile( shadersFolder / "transparent.vert" )
					, renderer::ShaderStageFlag::eVertex );
				result->createModule( common::dumpTextFile( shadersFolder / "transparent.frag" )
					, renderer::ShaderStageFlag::eFragment );
				result->link();
			}

			return result;
		}

		std::vector< renderer::PixelFormat > doGetFormats( renderer::TextureView const & colourView
			, renderer::TextureView const & depthView )
		{
			return
			{
				colourView.getFormat(),
				depthView.getFormat()
			};
		}
		renderer::RenderPassPtr doCreateRenderPass( renderer::Device const & device
			, renderer::TextureView const & colourView
			, renderer::TextureView const & depthView )
		{
			auto formats = doGetFormats( colourView, depthView );
			renderer::RenderSubpassPtrArray subpasses;
			subpasses.emplace_back( device.createRenderSubpass( formats
				, { renderer::PipelineStageFlag::eColourAttachmentOutput, renderer::AccessFlag::eColourAttachmentWrite } ) );
			return device.createRenderPass( formats
				, subpasses
				, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite
					, { renderer::ImageLayout::eColourAttachmentOptimal, renderer::ImageLayout::eDepthStencilAttachmentOptimal } }
				, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite
					, { renderer::ImageLayout::eColourAttachmentOptimal, renderer::ImageLayout::eDepthStencilAttachmentOptimal } }
				, false );
		}

		renderer::FrameBufferPtr doCreateFrameBuffer( renderer::RenderPass const & renderPass
			, renderer::TextureView const & colourView
			, renderer::TextureView const & depthView )
		{
			auto formats = doGetFormats( colourView, depthView );
			renderer::TextureAttachmentPtrArray attaches;
			attaches.emplace_back( std::make_unique< renderer::TextureAttachment >( colourView ) );
			attaches.emplace_back( std::make_unique< renderer::TextureAttachment >( depthView ) );
			auto dimensions = colourView.getTexture().getDimensions();
			return renderPass.createFrameBuffer( renderer::UIVec2{ dimensions[0], dimensions[1] }
				, std::move( attaches ) );
		}

		renderer::UniformBufferPtr< common::MaterialData > doCreateMaterialsUbo( renderer::Device const & device
			, common::Object const & submeshes
			, uint32_t & count )
		{
			count = 0u;

			for ( auto & submesh : submeshes )
			{
				count += std::count_if( submesh.materials.begin()
					, submesh.materials.end()
					, []( common::Material const & lookup )
					{
						return lookup.hasOpacity;
					} );
			}

			renderer::UniformBufferPtr< common::MaterialData > result;

			if ( count )
			{
				result = std::make_unique< renderer::UniformBuffer< common::MaterialData > >( device
					, count
					, renderer::BufferTarget::eTransferDst
					, renderer::MemoryPropertyFlag::eDeviceLocal );
			}

			return result;
		}

		renderer::DescriptorSetLayoutPtr doCreateDescriptorLayout( renderer::Device const & device )
		{
			std::vector< renderer::DescriptorSetLayoutBinding > bindings
			{
				renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex },
				renderer::DescriptorSetLayoutBinding{ 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex },
				renderer::DescriptorSetLayoutBinding{ 2u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment },
				renderer::DescriptorSetLayoutBinding{ 3u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment },
			};
			return device.createDescriptorSetLayout( std::move( bindings ) );
		}
	}

	TransparentRendering::TransparentRendering( renderer::Device const & device
		, common::Object const & submeshes
		, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
		, renderer::UniformBuffer< renderer::Mat4 > const & objectUbo
		, renderer::UniformBuffer< common::LightsData > const & lightsUbo
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureView const & colourView
		, renderer::TextureView const & depthView
		, common::TextureNodePtrArray const & textureNodes )
		: m_device{ device }
		, m_matrixUbo{ matrixUbo }
		, m_objectUbo{ objectUbo }
		, m_lightsUbo{ lightsUbo }
		, m_sampler{ m_device.createSampler( renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::Filter::eLinear
			, renderer::Filter::eLinear ) }
		, m_updateCommandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_commandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_materialsUbo{ doCreateMaterialsUbo( device, submeshes, m_nodesCount ) }
		, m_descriptorLayout{ doCreateDescriptorLayout( device ) }
		, m_descriptorPool{ m_descriptorLayout->createPool( m_nodesCount ) }
		, m_program{ doCreateProgram( m_device ) }
		, m_renderPass{ doCreateRenderPass( m_device, colourView, depthView ) }
	{
		if ( m_nodesCount )
		{
			uint32_t matIndex = 0u;

			for ( auto & submesh : submeshes )
			{
				m_submeshNodes.push_back( std::make_shared< common::SubmeshNode >() );
				common::SubmeshNodePtr submeshNode = m_submeshNodes.back();

				// Initialise vertex layout.
				submeshNode->vertexLayout = m_device.createVertexLayout( 0u, sizeof( common::Vertex ) );
				submeshNode->vertexLayout->createAttribute< renderer::Vec3 >( 0u, offsetof( common::Vertex, position ) );
				submeshNode->vertexLayout->createAttribute< renderer::Vec3 >( 1u, offsetof( common::Vertex, normal ) );
				submeshNode->vertexLayout->createAttribute< renderer::Vec3 >( 2u, offsetof( common::Vertex, tangent ) );
				submeshNode->vertexLayout->createAttribute< renderer::Vec3 >( 3u, offsetof( common::Vertex, bitangent ) );
				submeshNode->vertexLayout->createAttribute< renderer::Vec2 >( 4u, offsetof( common::Vertex, texture ) );

				// Initialise geometry buffers.
				submeshNode->vbo = renderer::makeVertexBuffer< common::Vertex >( m_device
					, uint32_t( submesh.vbo.data.size() )
					, renderer::BufferTarget::eTransferDst
					, renderer::MemoryPropertyFlag::eDeviceLocal );
				stagingBuffer.uploadVertexData( *m_updateCommandBuffer
					, submesh.vbo.data
					, *submeshNode->vbo
					, renderer::PipelineStageFlag::eVertexInput );
				submeshNode->ibo = renderer::makeBuffer< common::Face >( m_device
					, uint32_t( submesh.ibo.data.size() )
					, renderer::BufferTarget::eTransferDst
					, renderer::MemoryPropertyFlag::eDeviceLocal );
				stagingBuffer.uploadBufferData( *m_updateCommandBuffer
					, submesh.ibo.data
					, *submeshNode->ibo );
				submeshNode->geometryBuffers = m_device.createGeometryBuffers( *submeshNode->vbo
					, 0u
					, *submeshNode->vertexLayout
					, submeshNode->ibo->getBuffer()
					, 0u
					, renderer::IndexType::eUInt32 );

				for ( auto & material : submesh.materials )
				{
					if ( material.hasOpacity )
					{
						common::MaterialNode materialNode{ submeshNode };

						// Initialise material textures.
						for ( uint32_t index = 0u; index < material.data.texturesCount; ++index )
						{
							auto & texture = material.textures[index];
							auto it = std::find_if( textureNodes.begin()
								, textureNodes.end()
								, [&texture]( common::TextureNodePtr const & lookup )
								{
									return lookup->image == texture;
								} );
							assert( it != textureNodes.end() );
							materialNode.textures.push_back( *it );
						}

						m_materialsUbo->getData( matIndex ) = material.data;

						// Initialise descriptor set for UBOs
						materialNode.descriptorSetUbos = m_descriptorPool->createDescriptorSet( 0u );
						materialNode.descriptorSetUbos->createBinding( m_descriptorLayout->getBinding( 0u )
							, m_matrixUbo
							, 0u
							, 1u );
						materialNode.descriptorSetUbos->createBinding( m_descriptorLayout->getBinding( 1u )
							, m_objectUbo
							, 0u
							, 1u );
						materialNode.descriptorSetUbos->createBinding( m_descriptorLayout->getBinding( 2u )
							, *m_materialsUbo
							, matIndex
							, 1u );
						materialNode.descriptorSetUbos->createBinding( m_descriptorLayout->getBinding( 3u )
							, m_lightsUbo
							, 0u
							, 1u );
						materialNode.descriptorSetUbos->update();

						// Initialise descriptor set for textures.
						renderer::DescriptorSetLayoutBindingArray bindings;
						bindings.emplace_back( 0u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment, 6u );
						materialNode.layout = m_device.createDescriptorSetLayout( std::move( bindings ) );
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
						//renderer::ColourBlendState colourBlendState;
						//colourBlendState.addAttachment( renderer::ColourBlendStateAttachment
						//{
						//	true,
						//	renderer::BlendFactor::eSrcAlpha,
						//	renderer::BlendFactor::eInvSrcAlpha,
						//	renderer::BlendOp::eAdd,
						//	renderer::BlendFactor::eSrcAlpha,
						//	renderer::BlendFactor::eInvSrcAlpha,
						//	renderer::BlendOp::eAdd
						//} );
						renderer::RasterisationState rasterisationState;
						//renderer::DepthStencilState depthStencilState
						//{
						//	0u,
						//	true,
						//	false,
						//};

						if ( material.data.backFace )
						{
							rasterisationState = renderer::RasterisationState{ 0u
								, false
								, false
								, renderer::PolygonMode::eFill
								, renderer::CullModeFlag::eFront };
						}

						// Initialise the pipeline
						if ( materialNode.layout )
						{
							materialNode.pipelineLayout = m_device.createPipelineLayout( { *m_descriptorLayout, *materialNode.layout } );
						}
						else
						{
							materialNode.pipelineLayout = m_device.createPipelineLayout( *m_descriptorLayout );
						}

						materialNode.pipeline = materialNode.pipelineLayout->createPipeline( *m_program
							, { *submeshNode->vertexLayout }
							, *m_renderPass
							, renderer::PrimitiveTopology::eTriangleList
							, rasterisationState );
						materialNode.pipeline->multisampleState( renderer::MultisampleState{} );
						materialNode.pipeline->depthStencilState( renderer::DepthStencilState{} );
						materialNode.pipeline->finish();
						m_renderNodes.emplace_back( std::move( materialNode ) );
						++matIndex;
					}
				}
			}

			// Fill Materials Ubo
			stagingBuffer.uploadUniformData( *m_updateCommandBuffer
				, m_materialsUbo->getDatas()
				, *m_materialsUbo
				, renderer::PipelineStageFlag::eFragmentShader );

			update( colourView, depthView );
		}
	}

	void TransparentRendering::update( renderer::TextureView const & colourView
		, renderer::TextureView const & depthView )
	{
		if ( m_colourView != &colourView
			|| m_depthView != &depthView )
		{
			m_colourView = &colourView;
			m_depthView = &depthView;
			m_frameBuffer = doCreateFrameBuffer( *m_renderPass, colourView, depthView );
			auto & commandBuffer = *m_commandBuffer;
			static renderer::RgbaColour const colour{ 1.0, 1.0, 1.0, 1.0 };
			static renderer::DepthStencilClearValue const depth{ 1.0, 0 };
			auto size = colourView.getTexture().getDimensions();

			if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
			{
				if ( m_nodesCount )
				{
					commandBuffer.beginRenderPass( *m_renderPass
						, *m_frameBuffer
						, { colour, depth }
						, renderer::SubpassContents::eInline );

					for ( auto & node : m_renderNodes )
					{
						commandBuffer.bindPipeline( *node.pipeline );
						commandBuffer.setViewport( { size[0]
							, size[1]
							, 0
							, 0 } );
						commandBuffer.setScissor( { 0
							, 0
							, size[0]
							, size[1] } );
						commandBuffer.bindGeometryBuffers( *node.submesh->geometryBuffers );
						commandBuffer.bindDescriptorSet( *node.descriptorSetUbos
							, *node.pipelineLayout );
						commandBuffer.bindDescriptorSet( *node.descriptorSetTextures
							, *node.pipelineLayout );

						commandBuffer.drawIndexed( uint32_t( node.submesh->ibo->getBuffer().getSize() / sizeof( uint32_t ) )
							, 1u
							, 0u
							, 0u
							, 0u );
					}

					commandBuffer.endRenderPass();
				}

				commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::PipelineStageFlag::eBottomOfPipe
					, m_colourView->makeShaderInputResource() );
				commandBuffer.end();
			}
		}
	}

	void TransparentRendering::draw()const
	{
		if ( m_nodesCount )
		{
			m_device.getGraphicsQueue().submit( *m_commandBuffer, nullptr );
		}
	}
}
