#include "OpaqueRendering.hpp"

#include "FileUtils.hpp"

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
#include <Miscellaneous/QueryPool.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
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
#include <RenderPass/FrameBufferAttachment.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <algorithm>

namespace common
{
	namespace
	{
		renderer::ShaderProgramPtr doCreateProgram( renderer::Device const & device
			, std::string const & appName
			, std::string const & shader )
		{
			renderer::ShaderProgramPtr result = device.createShaderProgram();
			std::string shadersFolder = getPath( getExecutableDirectory() ) / "share" / appName / "Shaders";

			if ( !wxFileExists( shadersFolder / ( shader + ".vert" ) )
				|| !wxFileExists( shadersFolder / ( shader + ".frag" ) ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			result->createModule( common::dumpTextFile( shadersFolder / ( shader + ".vert" ) )
				, renderer::ShaderStageFlag::eVertex );
			result->createModule( common::dumpTextFile( shadersFolder / ( shader + ".frag" ) )
				, renderer::ShaderStageFlag::eFragment );

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

		renderer::RenderPassAttachmentArray doGetAttaches( std::vector< renderer::PixelFormat > const & formats )
		{
			renderer::RenderPassAttachmentArray result;

			for ( auto format : formats )
			{
				result.push_back( { format, true } );
			}

			return result;
		}

		renderer::RenderPassPtr doCreateRenderPass( renderer::Device const & device
			, renderer::PixelFormat const & colourFormat
			, renderer::PixelFormat const & depthFormat )
		{
			std::vector< renderer::PixelFormat > formats
			{
				colourFormat,
				depthFormat
			};
			auto attaches = doGetAttaches( formats );
			renderer::ImageLayoutArray const initialLayouts
			{
				renderer::ImageLayout::eShaderReadOnlyOptimal,
				renderer::ImageLayout::eDepthStencilAttachmentOptimal,
			};
			renderer::ImageLayoutArray const finalLayouts
			{
				renderer::ImageLayout::eColourAttachmentOptimal,
				renderer::ImageLayout::eDepthStencilAttachmentOptimal,
			};
			renderer::RenderSubpassPtrArray subpasses;
			subpasses.emplace_back( device.createRenderSubpass( formats
				, { renderer::PipelineStageFlag::eColourAttachmentOutput, renderer::AccessFlag::eColourAttachmentWrite } ) );
			return device.createRenderPass( attaches
				, subpasses
				, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite
					, initialLayouts }
				, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite
					, finalLayouts } );
		}

		renderer::FrameBufferPtr doCreateFrameBuffer( renderer::RenderPass const & renderPass
			, renderer::TextureView const & colourView
			, renderer::TextureView const & depthView )
		{
			auto formats = doGetFormats( colourView, depthView );
			renderer::FrameBufferAttachmentArray attaches;
			attaches.emplace_back( *( renderPass.begin() + 0u ), colourView );
			attaches.emplace_back( *( renderPass.begin() + 1u ), depthView );
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
					return !lookup.hasOpacity;
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
	}

	OpaqueRendering::OpaqueRendering( renderer::Device const & device
		, std::string const & appName
		, std::string const & shader
		, renderer::PixelFormat const & colourFormat
		, renderer::PixelFormat const & depthFormat )
		: m_device{ device }
		, m_sampler{ m_device.createSampler( renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::Filter::eLinear
			, renderer::Filter::eLinear ) }
		, m_updateCommandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_commandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_program{ doCreateProgram( m_device, appName, shader ) }
		, m_renderPass{ doCreateRenderPass( m_device, colourFormat, depthFormat ) }
		, m_queryPool{ m_device.createQueryPool( renderer::QueryType::eTimestamp, 2u, 0u ) }
	{
	}

	void OpaqueRendering::update( renderer::TextureView const & colourView
		, renderer::TextureView const & depthView )
	{
		m_colourView = &colourView;
		m_depthView = &depthView;
		m_frameBuffer = doCreateFrameBuffer( *m_renderPass, colourView, depthView );
		m_commandBuffer->reset();
		auto & commandBuffer = *m_commandBuffer;
		static renderer::RgbaColour const colour{ 1.0f, 0.8f, 0.4f, 0.0f };
		static renderer::DepthStencilClearValue const depth{ 1.0, 0 };
		auto size = colourView.getTexture().getDimensions();

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
		{
			commandBuffer.resetQueryPool( *m_queryPool, 0u, 2u );
			commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eTopOfPipe
				, *m_queryPool
				, 0u );
			commandBuffer.beginRenderPass( *m_renderPass
				, *m_frameBuffer
				, { colour, depth }
				, renderer::SubpassContents::eInline );

			if ( m_nodesCount )
			{

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
					commandBuffer.drawIndexed( node.submesh->ibo->getCount() * 3u );
				}
			}

			commandBuffer.endRenderPass();
			commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eBottomOfPipe
				, *m_queryPool
				, 1u );
			commandBuffer.end();
		}
	}

	bool OpaqueRendering::draw( std::chrono::nanoseconds & gpu )const
	{
		bool result = m_device.getGraphicsQueue().submit( *m_commandBuffer, nullptr );

		if ( result )
		{
			renderer::UInt32Array values{ 0u, 0u };
			m_queryPool->getResults( 0u
				, 2u
				, 0u
				, renderer::QueryResultFlag::eWait
				, values );
			gpu = std::chrono::nanoseconds{ uint64_t( ( values[1] - values[0] ) / float( m_device.getTimestampPeriod() ) ) };
		}

		return result;
	}

	void OpaqueRendering::doInitialise( Object const & submeshes
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureView const & colourView
		, renderer::TextureView const & depthView
		, common::TextureNodePtrArray const & textureNodes )
	{
		m_materialsUbo = doCreateMaterialsUbo( m_device, submeshes, m_nodesCount );
		std::vector< renderer::DescriptorSetLayoutBinding > bindings
		{
			renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment },
		};
		doFillDescriptorLayoutBindings( bindings );
		m_descriptorLayout = m_device.createDescriptorSetLayout( std::move( bindings ) );
		m_descriptorPool = m_descriptorLayout->createPool( m_nodesCount );

		if ( m_nodesCount )
		{
			uint32_t matIndex = 0u;

			for ( auto & submesh : submeshes )
			{
				std::vector< Material > compatibleMaterials;

				for ( auto & material : submesh.materials )
				{
					if ( !material.hasOpacity )
					{
						compatibleMaterials.push_back( material );
					}
				}

				if ( !compatibleMaterials.empty() )
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

					for ( auto & material : compatibleMaterials )
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
							, *m_materialsUbo
							, matIndex
							, 1u );
						doFillDescriptorSet( *m_descriptorLayout, *materialNode.descriptorSetUbos );
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
						renderer::RasterisationState rasterisationState;

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
							, { renderer::PrimitiveTopology::eTriangleList }
							, rasterisationState );
						materialNode.pipeline->multisampleState( renderer::MultisampleState{} );
						materialNode.pipeline->depthStencilState( renderer::DepthStencilState{} );
						materialNode.pipeline->finish();
						m_renderNodes.emplace_back( std::move( materialNode ) );
						++matIndex;
					}
				}
			}

			if ( !m_submeshNodes.empty() )
			{
				stagingBuffer.uploadUniformData( *m_updateCommandBuffer
					, m_materialsUbo->getDatas()
					, *m_materialsUbo
					, renderer::PipelineStageFlag::eFragmentShader );
			}

			update( colourView, depthView );
		}
	}
}
