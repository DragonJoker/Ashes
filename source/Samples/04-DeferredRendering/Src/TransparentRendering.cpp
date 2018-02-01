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
#include <Shader/ShaderProgram.hpp>

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
	}

	TransparentRendering::TransparentRendering( renderer::Device const & device
		, common::Object const & submeshes
		, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
		, renderer::UniformBuffer< renderer::Mat4 > const & objectUbo
		, renderer::UniformBuffer< common::LightsData > const & lightsUbo
		, renderer::RenderPass const & renderPass
		, renderer::UIVec2 const & size )
		: m_device{ device }
		, m_matrixUbo{ matrixUbo }
		, m_objectUbo{ objectUbo }
		, m_lightsUbo{ lightsUbo }
		, m_sampler{ m_device.createSampler( renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::Filter::eLinear
			, renderer::Filter::eLinear ) }
		, m_stagingBuffer{ std::make_unique< renderer::StagingBuffer >( m_device
			, 0u
			, 200u * 1024u * 1024u ) }
		, m_commandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer( false ) }
		, m_program{ doCreateProgram( m_device ) }
	{
		m_nodesCount = 0u;

		for ( auto & submesh : submeshes )
		{
			m_nodesCount += uint32_t( submesh.materials.size() );
		}

		m_materialsUbo = std::make_unique< renderer::UniformBuffer< common::MaterialData > >( m_device
			, m_nodesCount
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );

		std::vector< renderer::DescriptorSetLayoutBinding > bindings
		{
			renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex },
			renderer::DescriptorSetLayoutBinding{ 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex },
			renderer::DescriptorSetLayoutBinding{ 2u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment },
			renderer::DescriptorSetLayoutBinding{ 3u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment },
		};
		m_descriptorLayout = m_device.createDescriptorSetLayout( std::move( bindings ) );
		m_descriptorPool = m_descriptorLayout->createPool( m_nodesCount );

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
			m_stagingBuffer->uploadVertexData( *m_commandBuffer
				, submesh.vbo.data
				, *submeshNode->vbo
				, renderer::PipelineStageFlag::eVertexInput );
			submeshNode->ibo = renderer::makeBuffer< common::Face >( m_device
				, uint32_t( submesh.ibo.data.size() )
				, renderer::BufferTarget::eTransferDst
				, renderer::MemoryPropertyFlag::eDeviceLocal );
			m_stagingBuffer->uploadBufferData( *m_commandBuffer
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
				bool needsBlending = material.data.opacity < 1.0f;
				common::MaterialNode materialNode{ submeshNode };

				// Initialise material textures.
				for ( uint32_t index = 0u; index < material.data.texturesCount; ++index )
				{
					common::TextureNode textureNode;
					auto & texture = material.textures[index];
					textureNode.texture = m_device.createTexture();
					textureNode.texture->setImage( texture.format
						, { texture.size[0], texture.size[1] } );
					textureNode.view = textureNode.texture->createView( textureNode.texture->getType()
						, textureNode.texture->getFormat()
						, 0u
						, 1u
						, 0u
						, 1u );
					m_stagingBuffer->uploadTextureData( *m_commandBuffer
						, texture.data
						, *textureNode.view );
					needsBlending |= material.data.textureOperators[index].opacity != 0;
					materialNode.textures.emplace_back( std::move( textureNode ) );
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
						, *materialNode.textures[index].view
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
					, renderPass
					, renderer::PrimitiveTopology::eTriangleList
					, rasterisationState );
				materialNode.pipeline->multisampleState( renderer::MultisampleState{} );
				materialNode.pipeline->depthStencilState( renderer::DepthStencilState{} );
				materialNode.pipeline->finish();
				m_renderNodes.emplace_back( std::move( materialNode ) );
				++matIndex;
			}
		}

		// Fill Materials Ubo
		m_stagingBuffer->uploadUniformData( *m_commandBuffer
			, m_materialsUbo->getDatas()
			, *m_materialsUbo
			, renderer::PipelineStageFlag::eFragmentShader );

		update( size );
	}

	void TransparentRendering::update( renderer::UIVec2 const & size )
	{
		if ( m_size != size )
		{
			m_size = size;
			auto & commandBuffer = *m_commandBuffer;

			if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eRenderPassContinue ) )
			{
				for ( auto & node : m_renderNodes )
				{
					commandBuffer.bindPipeline( *node.pipeline );
					commandBuffer.setViewport( { m_size[0]
						, m_size[1]
						, 0
						, 0 } );
					commandBuffer.setScissor( { 0
						, 0
						, m_size[0]
						, m_size[1] } );
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

				commandBuffer.end();
			}
		}
	}
}
