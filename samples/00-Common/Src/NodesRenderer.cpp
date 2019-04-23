#include "NodesRenderer.hpp"

#include "FileUtils.hpp"
#include "RenderTarget.hpp"
#include "Scene.hpp"

#include <ashespp/Buffer/Buffer.hpp>
#include <ashespp/Buffer/StagingBuffer.hpp>
#include <ashespp/Buffer/VertexBuffer.hpp>
#include <ashespp/Command/CommandBuffer.hpp>
#include <ashespp/Command/CommandPool.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Descriptor/DescriptorSet.hpp>
#include <ashespp/Descriptor/DescriptorSetLayout.hpp>
#include <ashespp/Descriptor/DescriptorSetPool.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/Miscellaneous/QueryPool.hpp>
#include <ashespp/Pipeline/GraphicsPipeline.hpp>
#include <ashespp/Pipeline/PipelineLayout.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>
#include <ashespp/RenderPass/RenderPassCreateInfo.hpp>

#include <util/GlslToSpv.hpp>

#include <algorithm>

namespace common
{
	namespace
	{
		ashes::PipelineShaderStageCreateInfoArray doCreateObjectProgram( ashes::Device const & device
			, std::string const & fragmentShaderFile )
		{
			std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / "Sample-00-Common" / "Shaders";

			if ( !wxFileExists( shadersFolder / "object.vert" )
				|| !wxFileExists( fragmentShaderFile ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			ashes::PipelineShaderStageCreateInfoArray result;
			result.push_back( { device.createShaderModule( dumpShaderFile( device, VK_SHADER_STAGE_VERTEX_BIT, shadersFolder / "object.vert" ) ) } );
			result.push_back( { device.createShaderModule( dumpShaderFile( device, VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShaderFile ) ) } );
			return result;
		}

		ashes::PipelineShaderStageCreateInfoArray doCreateBillboardProgram( ashes::Device const & device
			, std::string const & fragmentShaderFile )
		{
			std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / "Sample-00-Common" / "Shaders";

			if ( !wxFileExists( shadersFolder / "billboard.vert" )
				|| !wxFileExists( fragmentShaderFile ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			ashes::PipelineShaderStageCreateInfoArray result;
			result.push_back( { device.createShaderModule( dumpShaderFile( device, VK_SHADER_STAGE_VERTEX_BIT, shadersFolder / "billboard.vert" ) ) } );
			result.push_back( { device.createShaderModule( dumpShaderFile( device, VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShaderFile ) ) } );
			return result;
		}

		ashes::RenderPassPtr doCreateRenderPass( ashes::Device const & device
			, std::vector< VkFormat > const & formats
			, bool clearViews )
		{
			uint32_t index{ 0u };
			ashes::RenderPassCreateInfo renderPass;
			renderPass.subpasses.resize( 1u );

			for ( auto format : formats )
			{
				if ( ashes::isDepthOrStencilFormat( format ) )
				{
					renderPass.attachments.push_back(
					{
						format,
						VK_SAMPLE_COUNT_1_BIT,
						( clearViews
							? VK_ATTACHMENT_LOAD_OP_CLEAR
							: VK_ATTACHMENT_LOAD_OP_DONT_CARE ),
						VK_ATTACHMENT_STORE_OP_STORE,
						( ( clearViews && ashes::isStencilFormat( format ) )
							? VK_ATTACHMENT_LOAD_OP_CLEAR
							: VK_ATTACHMENT_LOAD_OP_DONT_CARE ),
						VK_ATTACHMENT_STORE_OP_DONT_CARE,
						VK_IMAGE_LAYOUT_UNDEFINED,
						VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
					} );
					renderPass.subpasses[0].depthStencilAttachment = { index, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
				}
				else
				{
					renderPass.attachments.push_back(
					{
						format,
						VK_SAMPLE_COUNT_1_BIT,
						( clearViews
							? VK_ATTACHMENT_LOAD_OP_CLEAR
							: VK_ATTACHMENT_LOAD_OP_DONT_CARE ),
						VK_ATTACHMENT_STORE_OP_STORE,
						VK_ATTACHMENT_LOAD_OP_DONT_CARE,
						VK_ATTACHMENT_STORE_OP_DONT_CARE,
						VK_IMAGE_LAYOUT_UNDEFINED,
						( clearViews
							? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
							: VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )
					} );
					renderPass.subpasses[0].colorAttachments.emplace_back( VkAttachmentReference{ index, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } );
				}

				++index;
			}

			renderPass.dependencies.resize( 2u );
			renderPass.dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
			renderPass.dependencies[0].dstSubpass = 0u;
			renderPass.dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			renderPass.dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			renderPass.dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			renderPass.dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			renderPass.dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			renderPass.dependencies[1].srcSubpass = 0u;
			renderPass.dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
			renderPass.dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			renderPass.dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			renderPass.dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			renderPass.dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			renderPass.dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			return device.createRenderPass( renderPass );
		}

		ashes::FrameBufferPtr doCreateFrameBuffer( ashes::RenderPass const & renderPass
			, ashes::ImageViewArray views )
		{
			assert( !views.empty() );
			assert( views.size() == renderPass.getAttachmentCount() );
			ashes::ImageViewArray attaches;
			auto it = renderPass.getAttachments().begin();
			auto dimensions = views[0]->getImage().getDimensions();

			for ( auto & view : views )
			{
				attaches.emplace_back( *it, std::move( view ) );
				++it;
			}

			return renderPass.createFrameBuffer( VkExtent2D{ dimensions.width, dimensions.height }
			, std::move( attaches ) );
		}

		ashes::UniformBufferPtr< common::MaterialData > doCreateMaterialsUbo( utils::Device const & device
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

			ashes::UniformBufferPtr< common::MaterialData > result;

			if ( objectsCount + billboardsCount )
			{
				result = utils::makeUniformBuffer< common::MaterialData >( device
					, objectsCount + billboardsCount
					, VK_BUFFER_USAGE_TRANSFER_DST_BIT
					, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
			}

			return result;
		}

		ashes::ImageViewArray doCloneViews( ashes::ImageViewCRefArray const & views )
		{
			ashes::ImageViewArray result;

			for ( auto & view : views )
			{
				result.emplace_back( view.get().image->createView( *view.get() ) );
			}

			return result;
		}
	}

	NodesRenderer::NodesRenderer( utils::Device const & device
		, ashes::CommandPool const & commandPool
		, ashes::Queue const & transferQueue
		, std::string const & fragmentShaderFile
		, std::vector< VkFormat > const & formats
		, bool clearViews
		, bool opaqueNodes )
		: m_device{ device }
		, m_commandPool{ commandPool }
		, m_transferQueue{ transferQueue }
		, m_opaqueNodes{ opaqueNodes }
		, m_fragmentShaderFile{ fragmentShaderFile }
		, m_sampler{ m_device.getDevice().createSampler( VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_FILTER_LINEAR
			, VK_FILTER_LINEAR ) }
		, m_commandBuffer{ commandPool.createCommandBuffer() }
		, m_renderPass{ doCreateRenderPass( m_device.getDevice(), formats, clearViews ) }
		, m_queryPool{ m_device.getDevice().createQueryPool( VK_QUERY_TYPE_TIMESTAMP, 2u, 0u ) }
	{
	}

	void NodesRenderer::update( RenderTarget const & target )
	{
		doUpdate( { target.getDepthView(), target.getColourView() } );
	}

	void NodesRenderer::draw( ashes::Queue const & queue
		, std::chrono::nanoseconds & gpu )const
	{
		queue.submit( *m_commandBuffer, nullptr );
		ashes::UInt64Array values{ 0u, 0u };
		m_queryPool->getResults( 0u
			, 2u
			, 0u
			, VK_QUERY_RESULT_WAIT_BIT
			, values );
		gpu = std::chrono::nanoseconds{ uint64_t( ( values[1] - values[0] ) / float( m_device.getDevice().getTimestampPeriod() ) ) };
	}

	void NodesRenderer::initialise( Scene const & scene
		, ashes::StagingBuffer & stagingBuffer
		, ashes::ImageViewArray views
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
			stagingBuffer.uploadUniformData( m_transferQueue
				, m_commandPool
				, m_materialsUbo->getDatas()
				, *m_materialsUbo
				, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT );
		}

		doUpdate( std::move( views ) );
	}

	void NodesRenderer::doUpdate( ashes::ImageViewArray views )
	{
		assert( !views.empty() );
		auto dimensions = views[0]->getImage().getDimensions();
		auto size = VkExtent2D{ dimensions.width, dimensions.height };

		if ( size != m_size )
		{
			m_size = size;
			static VkClearColorValue const colour{ 1.0f, 0.8f, 0.4f, 0.0f };
			static VkClearDepthStencilValue const depth{ 1.0, 0 };
			ashes::VkClearValueArray clearValues;

			for ( auto & view : views )
			{
				if ( !ashes::isDepthOrStencilFormat( view->getFormat() ) )
				{
					clearValues.emplace_back( colour );
				}
				else
				{
					clearValues.emplace_back( depth );
				}
			}

			m_frameBuffer = doCreateFrameBuffer( *m_renderPass, std::move( views ) );
			m_commandBuffer->reset();
			auto & commandBuffer = *m_commandBuffer;

			commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT );
			commandBuffer.resetQueryPool( *m_queryPool, 0u, 2u );
			commandBuffer.writeTimestamp( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, *m_queryPool
				, 0u );
			commandBuffer.beginRenderPass( *m_renderPass
				, *m_frameBuffer
				, clearValues
				, VK_SUBPASS_CONTENTS_INLINE );

			for ( auto & node : m_submeshRenderNodes )
			{
				commandBuffer.bindPipeline( *node.pipeline );
				commandBuffer.setViewport( { 0.0f, 0.0f, float( size.width ), float( size.height ), 0.0f, 1.0f } );
				commandBuffer.setScissor( { { 0, 0 }, { size.width, size.height } } );
				m_commandBuffer->bindVertexBuffer( 0u, node.instance->vbo->getBuffer(), 0u );
				m_commandBuffer->bindIndexBuffer( node.instance->ibo->getBuffer(), 0u, VK_INDEX_TYPE_UINT32 );
				commandBuffer.bindDescriptorSet( *node.descriptorSetUbos
					, *node.pipelineLayout );
				commandBuffer.bindDescriptorSet( *node.descriptorSetTextures
					, *node.pipelineLayout );
				commandBuffer.drawIndexed( node.instance->ibo->getCount() * 3u );
			}

			for ( BillboardMaterialNode & node : m_billboardRenderNodes )
			{
				commandBuffer.bindPipeline( *node.pipeline );
				commandBuffer.setViewport( { 0.0f, 0.0f, float( size.width ), float( size.height ), 0.0f, 1.0f } );
				commandBuffer.setScissor( { { 0, 0 }, { size.width, size.height } } );
				m_commandBuffer->bindVertexBuffers( 0u
					, { node.instance->vbo->getBuffer(), node.instance->instance->getBuffer() }
					, { 0u, 0u } );
				commandBuffer.bindDescriptorSet( *node.descriptorSetUbos
					, *node.pipelineLayout );
				commandBuffer.bindDescriptorSet( *node.descriptorSetTextures
					, *node.pipelineLayout );
				commandBuffer.draw( 4u, node.instance->instance->getCount() );
			}

			commandBuffer.endRenderPass();
			commandBuffer.writeTimestamp( VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
				, *m_queryPool
				, 1u );
			commandBuffer.end();
		}
	}

	void NodesRenderer::doInitialiseBillboard( Billboard const & billboard
		, ashes::StagingBuffer & stagingBuffer
		, TextureNodePtrArray const & textureNodes
		, uint32_t & matIndex )
	{
		if ( !billboard.list.empty() )
		{
			ashes::VkDescriptorSetLayoutBindingArray bindings
			{
				VkDescriptorSetLayoutBinding{ 0u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT },
			};
			doFillBillboardDescriptorLayoutBindings( bindings );
			m_billboardDescriptorLayout = m_device.getDevice().createDescriptorSetLayout( std::move( bindings ) );
			m_billboardDescriptorPool = m_billboardDescriptorLayout->createPool( m_billboardsCount );

			// Initialise vertex layout.
			m_billboardVertexLayout = ashes::makeLayout< Vertex >( 0u, VK_VERTEX_INPUT_RATE_VERTEX );
			m_billboardVertexLayout->createAttribute( 0u
				, VK_FORMAT_R32G32B32_SFLOAT
				, offsetof( Vertex, position ) );
			m_billboardVertexLayout->createAttribute( 1u
				, VK_FORMAT_R32G32B32_SFLOAT
				, offsetof( Vertex, normal ) );
			m_billboardVertexLayout->createAttribute( 2u
				, VK_FORMAT_R32G32B32_SFLOAT
				, offsetof( Vertex, tangent ) );
			m_billboardVertexLayout->createAttribute( 3u
				, VK_FORMAT_R32G32B32_SFLOAT
				, offsetof( Vertex, bitangent ) );
			m_billboardVertexLayout->createAttribute( 4u
				, VK_FORMAT_R32G32_SFLOAT
				, offsetof( Vertex, texture ) );
			// Initialise instance layout.
			m_billboardInstanceLayout = ashes::makeLayout< BillboardInstanceData >( 1u, VK_VERTEX_INPUT_RATE_INSTANCE );
			m_billboardInstanceLayout->createAttribute( 5u
				, VK_FORMAT_R32G32B32_SFLOAT
				, offsetof( BillboardInstanceData, offset ) );
			m_billboardInstanceLayout->createAttribute( 6u
				, VK_FORMAT_R32G32_SFLOAT
				, offsetof( BillboardInstanceData, dimensions ) );

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
				billboardNode->vbo = utils::makeVertexBuffer< Vertex >( m_device
					, uint32_t( vertexData.size() )
					, VK_BUFFER_USAGE_TRANSFER_DST_BIT
					, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
				stagingBuffer.uploadVertexData( m_transferQueue
					, m_commandPool
					, vertexData
					, *billboardNode->vbo );
				billboardNode->instance = utils::makeVertexBuffer< BillboardInstanceData >( m_device
					, uint32_t( billboard.list.size() )
					, VK_BUFFER_USAGE_TRANSFER_DST_BIT
					, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
				stagingBuffer.uploadVertexData( m_transferQueue
					, m_commandPool
					, billboard.list
					, *billboardNode->instance );

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
				ashes::VkDescriptorSetLayoutBindingArray bindings;
				bindings.emplace_back( 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 6u );
				materialNode.layout = m_device.getDevice().createDescriptorSetLayout( std::move( bindings ) );
				materialNode.pool = materialNode.layout->createPool( 1u );
				materialNode.descriptorSetTextures = materialNode.pool->createDescriptorSet( 1u );

				for ( uint32_t index = 0u; index < material.data.texturesCount; ++index )
				{
					materialNode.descriptorSetTextures->createBinding( materialNode.layout->getBinding( 0u, index )
						, *materialNode.textures[index]->view
						, *m_sampler
						, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
						, index );
				}

				materialNode.descriptorSetTextures->update();
				VkPipelineRasterizationStateCreateInfo rasterisationState;
				rasterisationState.cullMode = VK_CULL_MODE_NONE;

				// Initialise the pipeline
				if ( materialNode.layout )
				{
					materialNode.pipelineLayout = m_device.getDevice().createPipelineLayout( { *m_billboardDescriptorLayout, *materialNode.layout } );
				}
				else
				{
					materialNode.pipelineLayout = m_device.getDevice().createPipelineLayout( *m_billboardDescriptorLayout );
				}

				VkPipelineColorBlendStateCreateInfo blendState;

				for ( auto & attach : m_renderPass->getAttachments() )
				{
					if ( !ashes::isDepthOrStencilFormat( attach.format ) )
					{
						blendState.attachs.push_back( ashes::ColourBlendStateAttachment{} );
					}
				}

				std::vector< VkDynamicState > dynamicStateEnables
				{
					VK_DYNAMIC_STATE_VIEWPORT,
					VK_DYNAMIC_STATE_SCISSOR
				};

				materialNode.pipeline = materialNode.pipelineLayout->createPipeline( 
				{
					doCreateBillboardProgram( m_device.getDevice(), m_fragmentShaderFile ),
					*m_renderPass,
					ashes::VertexInputState::create( { *m_billboardVertexLayout, *m_billboardInstanceLayout } ),
					{ VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP },
					rasterisationState,
					VkPipelineMultisampleStateCreateInfo{},
					blendState,
					dynamicStateEnables,
					VkPipelineDepthStencilStateCreateInfo{}
				} );
				m_billboardRenderNodes.emplace_back( std::move( materialNode ) );
				++matIndex;
			}
		}
	}

	void NodesRenderer::doInitialiseObject( Object const & object
		, ashes::StagingBuffer & stagingBuffer
		, common::TextureNodePtrArray const & textureNodes
		, uint32_t & matIndex )
	{
		ashes::VkDescriptorSetLayoutBindingArray bindings
		{
			VkDescriptorSetLayoutBinding{ 0u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT },
		};
		doFillObjectDescriptorLayoutBindings( bindings );
		m_objectDescriptorLayout = m_device.getDevice().createDescriptorSetLayout( std::move( bindings ) );
		m_objectDescriptorPool = m_objectDescriptorLayout->createPool( m_objectsCount );

		// Initialise vertex layout.
		m_objectVertexLayout = ashes::makeLayout< Vertex >( 0u );
		m_objectVertexLayout->createAttribute( 0u
			, VK_FORMAT_R32G32B32_SFLOAT
			, offsetof( common::Vertex, position ) );
		m_objectVertexLayout->createAttribute( 1u
			, VK_FORMAT_R32G32B32_SFLOAT
			, offsetof( common::Vertex, normal ) );
		m_objectVertexLayout->createAttribute( 2u
			, VK_FORMAT_R32G32B32_SFLOAT
			, offsetof( common::Vertex, tangent ) );
		m_objectVertexLayout->createAttribute( 3u
			, VK_FORMAT_R32G32B32_SFLOAT
			, offsetof( common::Vertex, bitangent ) );
		m_objectVertexLayout->createAttribute( 4u
			, VK_FORMAT_R32G32_SFLOAT
			, offsetof( common::Vertex, texture ) );

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
				submeshNode->vbo = utils::makeVertexBuffer< common::Vertex >( m_device
					, uint32_t( submesh.vbo.data.size() )
					, VK_BUFFER_USAGE_TRANSFER_DST_BIT
					, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
				stagingBuffer.uploadVertexData( m_transferQueue
					, m_commandPool
					, submesh.vbo.data
					, *submeshNode->vbo );
				submeshNode->ibo = utils::makeBuffer< common::Face >( m_device
					, uint32_t( submesh.ibo.data.size() )
					, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT
					, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
				stagingBuffer.uploadBufferData( m_transferQueue
					, m_commandPool
					, submesh.ibo.data
					, *submeshNode->ibo );

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
					ashes::VkDescriptorSetLayoutBindingArray bindings;
					bindings.emplace_back( 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 6u );
					materialNode.layout = m_device.getDevice().createDescriptorSetLayout( std::move( bindings ) );
					materialNode.pool = materialNode.layout->createPool( 1u );
					materialNode.descriptorSetTextures = materialNode.pool->createDescriptorSet( 1u );

					for ( uint32_t index = 0u; index < material.data.texturesCount; ++index )
					{
						materialNode.descriptorSetTextures->createBinding( materialNode.layout->getBinding( 0u, index )
							, *materialNode.textures[index]->view
							, *m_sampler
							, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
							, index );
					}

					materialNode.descriptorSetTextures->update();
					VkPipelineRasterizationStateCreateInfo rasterisationState;

					if ( material.data.backFace )
					{
						rasterisationState.cullMode = VK_CULL_MODE_FRONT_BIT;
					}

					// Initialise the pipeline
					if ( materialNode.layout )
					{
						materialNode.pipelineLayout = m_device.getDevice().createPipelineLayout( { *m_objectDescriptorLayout, *materialNode.layout } );
					}
					else
					{
						materialNode.pipelineLayout = m_device.getDevice().createPipelineLayout( *m_objectDescriptorLayout );
					}

					VkPipelineColorBlendStateCreateInfo blendState;

					for ( auto & attach : m_renderPass->getAttachments() )
					{
						if ( !ashes::isDepthOrStencilFormat( attach.format ) )
						{
							blendState.attachs.push_back( ashes::ColourBlendStateAttachment{} );
						}
					}

					std::vector< VkDynamicState > dynamicStateEnables
					{
						VK_DYNAMIC_STATE_VIEWPORT,
						VK_DYNAMIC_STATE_SCISSOR
					};

					materialNode.pipeline = materialNode.pipelineLayout->createPipeline(
					{
						doCreateObjectProgram( m_device.getDevice(), m_fragmentShaderFile ),
						*m_renderPass,
						ashes::VertexInputState::create( *m_objectVertexLayout ),
						{ VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST },
						rasterisationState,
						VkPipelineMultisampleStateCreateInfo{},
						blendState,
						dynamicStateEnables,
						VkPipelineDepthStencilStateCreateInfo{}
					} );
					m_submeshRenderNodes.emplace_back( std::move( materialNode ) );
					++matIndex;
				}
			}
		}
	}
}
