#include "NodesRenderer.hpp"

#include "FileUtils.hpp"
#include "RenderTarget.hpp"
#include "Scene.hpp"

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
		renderer::ShaderProgramPtr doCreateObjectProgram( renderer::Device const & device
			, std::string const & fragmentShaderFile )
		{
			renderer::ShaderProgramPtr result = device.createShaderProgram();
			std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / "Sample-00-Common" / "Shaders";

			if ( !wxFileExists( shadersFolder / "object.vert" )
				|| !wxFileExists( fragmentShaderFile ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			result->createModule( common::dumpTextFile( shadersFolder / "object.vert" )
				, renderer::ShaderStageFlag::eVertex );
			result->createModule( common::dumpTextFile( fragmentShaderFile )
				, renderer::ShaderStageFlag::eFragment );

			return result;
		}

		renderer::ShaderProgramPtr doCreateBillboardProgram( renderer::Device const & device
			, std::string const & fragmentShaderFile )
		{
			renderer::ShaderProgramPtr result = device.createShaderProgram();
			std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / "Sample-00-Common" / "Shaders";

			if ( !wxFileExists( shadersFolder / "billboard.vert" )
				|| !wxFileExists( fragmentShaderFile ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			result->createModule( common::dumpTextFile( shadersFolder / "billboard.vert" )
				, renderer::ShaderStageFlag::eVertex );
			result->createModule( common::dumpTextFile( fragmentShaderFile )
				, renderer::ShaderStageFlag::eFragment );

			return result;
		}

		renderer::RenderPassPtr doCreateRenderPass( renderer::Device const & device
			, std::vector< renderer::PixelFormat > const & formats
			, bool clearViews )
		{
			renderer::RenderPassAttachmentArray attaches;
			renderer::ImageLayoutArray initialLayouts;
			renderer::ImageLayoutArray finalLayouts;

			for ( auto format : formats )
			{
				attaches.push_back( { format, clearViews } );

				if ( renderer::isDepthFormat( format )
					|| renderer::isStencilFormat( format )
					|| renderer::isDepthStencilFormat( format ) )
				{
					initialLayouts.push_back( renderer::ImageLayout::eDepthStencilAttachmentOptimal );
					finalLayouts.push_back( renderer::ImageLayout::eDepthStencilAttachmentOptimal );
				}
				else
				{
					initialLayouts.push_back( clearViews
						? renderer::ImageLayout::eShaderReadOnlyOptimal
						: renderer::ImageLayout::eColourAttachmentOptimal );
					finalLayouts.push_back( clearViews
						? renderer::ImageLayout::eColourAttachmentOptimal
						: renderer::ImageLayout::eShaderReadOnlyOptimal );
				}
			}

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
			, renderer::TextureViewCRefArray const & views )
		{
			assert( !views.empty() );
			assert( views.size() == renderPass.getSize() );
			renderer::FrameBufferAttachmentArray attaches;
			auto it = renderPass.begin();

			for ( auto view : views )
			{
				attaches.emplace_back( *it, view.get() );
				++it;
			}

			auto dimensions = views[0].get().getTexture().getDimensions();
			return renderPass.createFrameBuffer( renderer::UIVec2{ dimensions[0], dimensions[1] }
			, std::move( attaches ) );
		}

		renderer::UniformBufferPtr< common::MaterialData > doCreateMaterialsUbo( renderer::Device const & device
			, Scene const & scene
			, bool m_opaqueNodes
			, uint32_t & objectsCount
			, uint32_t & billboardsCount )
		{
			objectsCount = 0u;
			billboardsCount = 0u;

			for ( auto & submesh : scene.object )
			{
				objectsCount += std::count_if( submesh.materials.begin()
					, submesh.materials.end()
					, [&m_opaqueNodes]( common::Material const & lookup )
				{
					return lookup.hasOpacity == !m_opaqueNodes;
				} );
			}

			if ( !scene.billboard.list.empty()
				&& scene.billboard.material.hasOpacity != m_opaqueNodes )
			{
				++billboardsCount;
			}

			renderer::UniformBufferPtr< common::MaterialData > result;

			if ( objectsCount + billboardsCount )
			{
				result = std::make_unique< renderer::UniformBuffer< common::MaterialData > >( device
					, objectsCount + billboardsCount
					, renderer::BufferTarget::eTransferDst
					, renderer::MemoryPropertyFlag::eDeviceLocal );
			}

			return result;
		}
	}

	NodesRenderer::NodesRenderer( renderer::Device const & device
		, std::string const & fragmentShaderFile
		, std::vector< renderer::PixelFormat > const & formats
		, bool clearViews
		, bool opaqueNodes )
		: m_device{ device }
		, m_opaqueNodes{ opaqueNodes }
		, m_sampler{ m_device.createSampler( renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::Filter::eLinear
			, renderer::Filter::eLinear ) }
		, m_updateCommandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_commandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_objectProgram{ doCreateObjectProgram( m_device, fragmentShaderFile ) }
		, m_billboardProgram{ doCreateBillboardProgram( m_device, fragmentShaderFile ) }
		, m_renderPass{ doCreateRenderPass( m_device, formats, clearViews ) }
		, m_queryPool{ m_device.createQueryPool( renderer::QueryType::eTimestamp, 2u, 0u ) }
	{
	}

	void NodesRenderer::update( RenderTarget const & target )
	{
		doUpdate( { target.getDepthView(), target.getColourView() } );
	}

	bool NodesRenderer::draw( std::chrono::nanoseconds & gpu )const
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

	void NodesRenderer::initialise( Scene const & scene
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureViewCRefArray const & views
		, common::TextureNodePtrArray const & textureNodes )
	{
		m_materialsUbo = doCreateMaterialsUbo( m_device
			, scene
			, m_opaqueNodes
			, m_objectsCount
			, m_billboardsCount );

		uint32_t matIndex = 0u;
		doInitialiseObject( scene.object
			, stagingBuffer
			, textureNodes
			, matIndex );
		doInitialiseBillboard( scene.billboard
			, stagingBuffer
			, textureNodes
			, matIndex );

		if ( m_objectsCount || m_billboardsCount )
		{
			stagingBuffer.uploadUniformData( *m_updateCommandBuffer
				, m_materialsUbo->getDatas()
				, *m_materialsUbo
				, renderer::PipelineStageFlag::eFragmentShader );
		}

		doUpdate( views );
	}
	void NodesRenderer::doUpdate( renderer::TextureViewCRefArray const & views )
	{
		assert( !views.empty() );
		auto dimensions = views[0].get().getTexture().getDimensions();
		auto size = renderer::UIVec2{ dimensions[0], dimensions[1] };

		if ( size != m_size )
		{
			m_size = size;
			m_views.clear();
			static renderer::RgbaColour const colour{ 1.0f, 0.8f, 0.4f, 0.0f };
			static renderer::DepthStencilClearValue const depth{ 1.0, 0 };
			renderer::ClearValueArray clearValues;

			for ( auto & view : views )
			{
				m_views.push_back( &view.get() );

				if ( !renderer::isDepthFormat( view.get().getFormat() )
					&& !renderer::isStencilFormat( view.get().getFormat() )
					&& !renderer::isDepthStencilFormat( view.get().getFormat() ) )
				{
					clearValues.emplace_back( colour );
				}
				else
				{
					clearValues.emplace_back( depth );
				}
			}

			m_frameBuffer = doCreateFrameBuffer( *m_renderPass, views );
			m_commandBuffer->reset();
			auto & commandBuffer = *m_commandBuffer;

			if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
			{
				commandBuffer.resetQueryPool( *m_queryPool, 0u, 2u );
				commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eTopOfPipe
					, *m_queryPool
					, 0u );
				commandBuffer.beginRenderPass( *m_renderPass
					, *m_frameBuffer
					, clearValues
					, renderer::SubpassContents::eInline );

				for ( auto & node : m_submeshRenderNodes )
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
					commandBuffer.bindGeometryBuffers( *node.instance->geometryBuffers );
					commandBuffer.bindDescriptorSet( *node.descriptorSetUbos
						, *node.pipelineLayout );
					commandBuffer.bindDescriptorSet( *node.descriptorSetTextures
						, *node.pipelineLayout );
					commandBuffer.drawIndexed( node.instance->ibo->getCount() * 3u );
				}

				for ( auto & node : m_billboardRenderNodes )
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
					commandBuffer.bindGeometryBuffers( *node.instance->geometryBuffers );
					commandBuffer.bindDescriptorSet( *node.descriptorSetUbos
						, *node.pipelineLayout );
					commandBuffer.bindDescriptorSet( *node.descriptorSetTextures
						, *node.pipelineLayout );
					commandBuffer.draw( 4u, node.instance->instance->getCount() );
				}

				commandBuffer.endRenderPass();
				commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eBottomOfPipe
					, *m_queryPool
					, 1u );
				commandBuffer.end();
			}
		}
	}

	void NodesRenderer::doInitialiseBillboard( Billboard const & billboard
		, renderer::StagingBuffer & stagingBuffer
		, TextureNodePtrArray const & textureNodes
		, uint32_t & matIndex )
	{
		if ( !billboard.list.empty() )
		{
			std::vector< renderer::DescriptorSetLayoutBinding > bindings
			{
				renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment },
			};
			doFillBillboardDescriptorLayoutBindings( bindings );
			m_billboardDescriptorLayout = m_device.createDescriptorSetLayout( std::move( bindings ) );
			m_billboardDescriptorPool = m_billboardDescriptorLayout->createPool( m_billboardsCount );

			// Initialise vertex layout.
			m_billboardVertexLayout = renderer::makeLayout< Vertex >( m_device, 0u, renderer::VertexInputRate::eVertex );
			m_billboardVertexLayout->createAttribute< decltype( Vertex::position ) >( 0u, offsetof( Vertex, position ) );
			m_billboardVertexLayout->createAttribute< decltype( Vertex::normal ) >( 1u, offsetof( Vertex, normal ) );
			m_billboardVertexLayout->createAttribute< decltype( Vertex::tangent ) >( 2u, offsetof( Vertex, tangent ) );
			m_billboardVertexLayout->createAttribute< decltype( Vertex::bitangent ) >( 3u, offsetof( Vertex, bitangent ) );
			m_billboardVertexLayout->createAttribute< decltype( Vertex::texture ) >( 4u, offsetof( Vertex, texture ) );
			// Initialise instance layout.
			m_billboardInstanceLayout = renderer::makeLayout< BillboardInstanceData >( m_device, 1u, renderer::VertexInputRate::eInstance );
			m_billboardInstanceLayout->createAttribute< decltype( BillboardInstanceData::offset ) >( 5u, offsetof( BillboardInstanceData, offset ) );
			m_billboardInstanceLayout->createAttribute< decltype( BillboardInstanceData::dimensions ) >( 6u, offsetof( BillboardInstanceData, dimensions ) );

			if ( billboard.material.hasOpacity == !m_opaqueNodes )
			{
				std::vector< Vertex > vertexData
				{
					{ { 0.0, 0.0, 0.0 }, { 0.0, 0.0, -1.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { -0.5, -0.5 } },
					{ { 0.0, 0.0, 0.0 }, { 0.0, 0.0, -1.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { -0.5, +0.5 } },
					{ { 0.0, 0.0, 0.0 }, { 0.0, 0.0, -1.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { +0.5, -0.5 } },
					{ { 0.0, 0.0, 0.0 }, { 0.0, 0.0, -1.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { +0.5, +0.5 } },
				};
				m_billboardNodes.push_back( std::make_shared< BillboardNode >() );
				BillboardNodePtr billboardNode = m_billboardNodes.back();

				// Initialise geometry buffers.
				billboardNode->vbo = renderer::makeVertexBuffer< Vertex >( m_device
					, uint32_t( vertexData.size() )
					, renderer::BufferTarget::eTransferDst
					, renderer::MemoryPropertyFlag::eDeviceLocal );
				stagingBuffer.uploadVertexData( *m_updateCommandBuffer
					, vertexData
					, *billboardNode->vbo
					, renderer::PipelineStageFlag::eVertexInput );
				billboardNode->instance = renderer::makeVertexBuffer< BillboardInstanceData >( m_device
					, uint32_t( billboard.list.size() )
					, renderer::BufferTarget::eTransferDst
					, renderer::MemoryPropertyFlag::eDeviceLocal );
				stagingBuffer.uploadVertexData( *m_updateCommandBuffer
					, billboard.list
					, *billboardNode->instance
					, renderer::PipelineStageFlag::eVertexInput );
				billboardNode->geometryBuffers = m_device.createGeometryBuffers( { *billboardNode->vbo, *billboardNode->instance }
					, { 0u, 0u }
					, { *m_billboardVertexLayout, *m_billboardInstanceLayout } );

				auto & material = billboard.material;
				BillboardMaterialNode materialNode{ billboardNode };

				// Initialise material textures.
				for ( uint32_t index = 0u; index < material.data.texturesCount; ++index )
				{
					auto & texture = material.textures[index];
					auto it = std::find_if( textureNodes.begin()
						, textureNodes.end()
						, [&texture]( TextureNodePtr const & lookup )
					{
						return lookup->image == texture;
					} );
					assert( it != textureNodes.end() );
					materialNode.textures.push_back( *it );
				}

				m_materialsUbo->getData( matIndex ) = material.data;

				// Initialise descriptor set for UBOs
				materialNode.descriptorSetUbos = m_billboardDescriptorPool->createDescriptorSet( 0u );
				materialNode.descriptorSetUbos->createBinding( m_billboardDescriptorLayout->getBinding( 0u )
					, *m_materialsUbo
					, matIndex
					, 1u );
				doFillBillboardDescriptorSet( *m_billboardDescriptorLayout, *materialNode.descriptorSetUbos );
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
				renderer::RasterisationState rasterisationState{ 0u, false, false, renderer::PolygonMode::eFill, renderer::CullModeFlag::eNone };

				// Initialise the pipeline
				if ( materialNode.layout )
				{
					materialNode.pipelineLayout = m_device.createPipelineLayout( { *m_billboardDescriptorLayout, *materialNode.layout } );
				}
				else
				{
					materialNode.pipelineLayout = m_device.createPipelineLayout( *m_billboardDescriptorLayout );
				}

				renderer::ColourBlendState blendState;

				for ( auto & attach : *m_renderPass )
				{
					if ( !renderer::isDepthFormat( attach.pixelFormat )
						&& !renderer::isStencilFormat( attach.pixelFormat )
						&& !renderer::isDepthStencilFormat( attach.pixelFormat ) )
					{
						blendState.addAttachment( renderer::ColourBlendStateAttachment{} );
					}
				}

				materialNode.pipeline = materialNode.pipelineLayout->createPipeline( *m_billboardProgram
					, { *m_billboardVertexLayout, *m_billboardInstanceLayout }
					, *m_renderPass
					, { renderer::PrimitiveTopology::eTriangleStrip }
					, rasterisationState
					, blendState );
				materialNode.pipeline->multisampleState( renderer::MultisampleState{} );
				materialNode.pipeline->depthStencilState( renderer::DepthStencilState{} );
				materialNode.pipeline->finish();
				m_billboardRenderNodes.emplace_back( std::move( materialNode ) );
				++matIndex;
			}
		}
	}

	void NodesRenderer::doInitialiseObject( Object const & object
		, renderer::StagingBuffer & stagingBuffer
		, common::TextureNodePtrArray const & textureNodes
		, uint32_t & matIndex )
	{
		std::vector< renderer::DescriptorSetLayoutBinding > bindings
		{
			renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment },
		};
		doFillObjectDescriptorLayoutBindings( bindings );
		m_objectDescriptorLayout = m_device.createDescriptorSetLayout( std::move( bindings ) );
		m_objectDescriptorPool = m_objectDescriptorLayout->createPool( m_objectsCount );

		// Initialise vertex layout.
		m_objectVertexLayout = renderer::makeLayout< Vertex >( m_device, 0u );
		m_objectVertexLayout->createAttribute< renderer::Vec3 >( 0u, offsetof( common::Vertex, position ) );
		m_objectVertexLayout->createAttribute< renderer::Vec3 >( 1u, offsetof( common::Vertex, normal ) );
		m_objectVertexLayout->createAttribute< renderer::Vec3 >( 2u, offsetof( common::Vertex, tangent ) );
		m_objectVertexLayout->createAttribute< renderer::Vec3 >( 3u, offsetof( common::Vertex, bitangent ) );
		m_objectVertexLayout->createAttribute< renderer::Vec2 >( 4u, offsetof( common::Vertex, texture ) );

		for ( auto & submesh : object )
		{
			std::vector< Material > compatibleMaterials;

			for ( auto & material : submesh.materials )
			{
				if ( material.hasOpacity == !m_opaqueNodes )
				{
					compatibleMaterials.push_back( material );
				}
			}

			if ( !compatibleMaterials.empty() )
			{
				m_submeshNodes.push_back( std::make_shared< common::SubmeshNode >() );
				common::SubmeshNodePtr submeshNode = m_submeshNodes.back();

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
					, *m_objectVertexLayout
					, submeshNode->ibo->getBuffer()
					, 0u
					, renderer::IndexType::eUInt32 );

				for ( auto & material : compatibleMaterials )
				{
					common::SubmeshMaterialNode materialNode{ submeshNode };

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
					materialNode.descriptorSetUbos = m_objectDescriptorPool->createDescriptorSet( 0u );
					materialNode.descriptorSetUbos->createBinding( m_objectDescriptorLayout->getBinding( 0u )
						, *m_materialsUbo
						, matIndex
						, 1u );
					doFillObjectDescriptorSet( *m_objectDescriptorLayout, *materialNode.descriptorSetUbos );
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
						materialNode.pipelineLayout = m_device.createPipelineLayout( { *m_objectDescriptorLayout, *materialNode.layout } );
					}
					else
					{
						materialNode.pipelineLayout = m_device.createPipelineLayout( *m_objectDescriptorLayout );
					}

					renderer::ColourBlendState blendState;

					for ( auto & attach : *m_renderPass )
					{
						if ( !renderer::isDepthFormat( attach.pixelFormat )
							&& !renderer::isStencilFormat( attach.pixelFormat )
							&& !renderer::isDepthStencilFormat( attach.pixelFormat ) )
						{
							blendState.addAttachment( renderer::ColourBlendStateAttachment{} );
						}
					}

					materialNode.pipeline = materialNode.pipelineLayout->createPipeline( *m_objectProgram
						, { *m_objectVertexLayout }
						, *m_renderPass
						, { renderer::PrimitiveTopology::eTriangleList }
						, rasterisationState
						, blendState );
					materialNode.pipeline->multisampleState( renderer::MultisampleState{} );
					materialNode.pipeline->depthStencilState( renderer::DepthStencilState{} );
					materialNode.pipeline->finish();
					m_submeshRenderNodes.emplace_back( std::move( materialNode ) );
					++matIndex;
				}
			}
		}
	}
}
