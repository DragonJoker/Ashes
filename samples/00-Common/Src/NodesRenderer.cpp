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
			, std::string const & vertexShaderFile
			, std::string const & fragmentShaderFile )
		{
			std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / "Sample-00-Common" / "Shaders";

			if ( !wxFileExists( shadersFolder / vertexShaderFile )
				|| !wxFileExists( fragmentShaderFile ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			ashes::PipelineShaderStageCreateInfoArray result;
			result.push_back( ashes::PipelineShaderStageCreateInfo
				{
					0u,
					VK_SHADER_STAGE_VERTEX_BIT,
					device.createShaderModule( dumpShaderFile( device
						, VK_SHADER_STAGE_VERTEX_BIT
						, shadersFolder / vertexShaderFile ) ),
					"main",
					std::nullopt,
				} );
			result.push_back( ashes::PipelineShaderStageCreateInfo
				{
					0u,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					device.createShaderModule( dumpShaderFile( device
						, VK_SHADER_STAGE_FRAGMENT_BIT
						, fragmentShaderFile ) ),
					"main",
					std::nullopt,
				} );
			return result;
		}

		ashes::PipelineShaderStageCreateInfoArray doCreateObjectProgram( ashes::Device const & device
			, std::string const & fragmentShaderFile )
		{
			return doCreateObjectProgram( device, "object.vert", fragmentShaderFile );
		}

		ashes::PipelineShaderStageCreateInfoArray doCreateBillboardProgram( ashes::Device const & device
			, std::string const & fragmentShaderFile )
		{
			return doCreateObjectProgram( device, "billboard.vert", fragmentShaderFile );
		}

		ashes::RenderPassPtr doCreateRenderPass( ashes::Device const & device
			, std::vector< VkFormat > const & formats
			, bool clearViews )
		{
			uint32_t index{ 0u };
			ashes::VkAttachmentDescriptionArray attaches;
			ashes::VkAttachmentReferenceArray colorAttachments;
			ashes::Optional< VkAttachmentReference > depthStencilAttachment;

			for ( auto format : formats )
			{
				if ( ashes::isDepthOrStencilFormat( format ) )
				{
					attaches.push_back( VkAttachmentDescription
						{
							0u,
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
					depthStencilAttachment = { index, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
				}
				else
				{
					attaches.push_back(
						{
							0u,
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
					colorAttachments.emplace_back( VkAttachmentReference{ index, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } );
				}

				++index;
			}
			
			ashes::SubpassDescriptionArray subpasses;
			subpasses.emplace_back( ashes::SubpassDescription
				{
					0u,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					{},
					std::move( colorAttachments ),
					{},
					std::move( depthStencilAttachment ),
					{},
				} );
			ashes::VkSubpassDependencyArray dependencies
			{
				{
					VK_SUBPASS_EXTERNAL,
					0u,
					VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					VK_ACCESS_SHADER_READ_BIT,
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
					VK_DEPENDENCY_BY_REGION_BIT,
				},
				{
					0u,
					VK_SUBPASS_EXTERNAL,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
					VK_ACCESS_SHADER_READ_BIT,
					VK_DEPENDENCY_BY_REGION_BIT,
				}
			};
			ashes::RenderPassCreateInfo createInfo
			{
				0u,
				std::move( attaches ),
				std::move( subpasses ),
				std::move( dependencies ),
			};
			return device.createRenderPass( std::move( createInfo ) );
		}

		ashes::FrameBufferPtr doCreateFrameBuffer( ashes::RenderPass const & renderPass
			, ashes::ImageViewArray views )
		{
			assert( !views.empty() );
			assert( views.size() == renderPass.getAttachmentCount() );
			ashes::ImageViewCRefArray attaches;
			auto it = renderPass.getAttachments().begin();
			auto dimensions = views[0].image->getDimensions();

			for ( auto & view : views )
			{
				attaches.emplace_back( view );
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
				result.emplace_back( view.get().image->createView( view.get().createInfo ) );
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
		m_dummyImage = m_device.createImage( ashes::ImageCreateInfo
			{
				0u,
				VK_IMAGE_TYPE_2D,
				VK_FORMAT_R8G8B8A8_UNORM,
				{ 1, 1, 1 },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_SAMPLED_BIT,
			}
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_dummyView = m_dummyImage->createView( VK_IMAGE_VIEW_TYPE_2D
			, VK_FORMAT_R8G8B8A8_UNORM );


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
		auto dimensions = views[0].image->getDimensions();
		auto size = VkExtent2D{ dimensions.width, dimensions.height };

		if ( size != m_size )
		{
			m_size = size;
			static VkClearColorValue const colour{ 1.0f, 0.8f, 0.4f, 0.0f };
			static VkClearDepthStencilValue const depth{ 1.0, 0 };
			ashes::VkClearValueArray clearValues;

			for ( auto & view : views )
			{
				if ( !ashes::isDepthOrStencilFormat( view.createInfo.format ) )
				{
					clearValues.emplace_back( ashes::makeClearValue( colour ) );
				}
				else
				{
					clearValues.emplace_back( ashes::makeClearValue( depth ) );
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
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
				, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
				, m_dummyView.makeShaderInputResource( VK_IMAGE_LAYOUT_UNDEFINED ) );
			commandBuffer.beginRenderPass( *m_renderPass
				, *m_frameBuffer
				, clearValues
				, VK_SUBPASS_CONTENTS_INLINE );

			for ( auto & node : m_submeshRenderNodes )
			{
				commandBuffer.bindPipeline( *node.pipeline );
				commandBuffer.setViewport( { 0.0f, 0.0f, float( size.width ), float( size.height ), 0.0f, 1.0f } );
				commandBuffer.setScissor( { { 0, 0 }, { size.width, size.height } } );
				commandBuffer.bindVertexBuffer( 0u, node.instance->vbo->getBuffer(), 0u );
				commandBuffer.bindIndexBuffer( node.instance->ibo->getBuffer(), 0u, VK_INDEX_TYPE_UINT32 );
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
				commandBuffer.bindVertexBuffers( 0u
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
		if ( !billboard.list.empty()
			&& billboard.material.hasOpacity != m_opaqueNodes )
		{
			ashes::VkDescriptorSetLayoutBindingArray bindings
			{
				{ 0u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			};
			doFillBillboardDescriptorLayoutBindings( bindings );
			m_billboardDescriptorLayout = m_device.getDevice().createDescriptorSetLayout( std::move( bindings ) );
			m_billboardDescriptorPool = m_billboardDescriptorLayout->createPool( m_billboardsCount );

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
				materialNode.layout = m_device.getDevice().createDescriptorSetLayout( { { 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 6u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr } } );
				materialNode.pool = materialNode.layout->createPool( 1u );
				materialNode.descriptorSetTextures = materialNode.pool->createDescriptorSet( 1u );
				doFillTextures( *materialNode.layout
					, *materialNode.descriptorSetTextures
					, materialNode.textures );
				materialNode.descriptorSetTextures->update();

				// Initialise the pipeline
				ashes::PipelineVertexInputStateCreateInfo vertexLayout
				{
					0u,
					{
						{ 0u, sizeof( Vertex ), VK_VERTEX_INPUT_RATE_VERTEX },
						{ 1u, sizeof( BillboardInstanceData ), VK_VERTEX_INPUT_RATE_INSTANCE },
					},
					{
						{ 0u, 0u, VK_FORMAT_R32G32B32_SFLOAT, offsetof( Vertex, position ) },
						{ 1u, 0u, VK_FORMAT_R32G32B32_SFLOAT, offsetof( Vertex, normal ) },
						{ 2u, 0u, VK_FORMAT_R32G32B32_SFLOAT, offsetof( Vertex, tangent ) },
						{ 3u, 0u, VK_FORMAT_R32G32B32_SFLOAT, offsetof( Vertex, bitangent ) },
						{ 4u, 0u, VK_FORMAT_R32G32_SFLOAT, offsetof( Vertex, texture ) },
						{ 5u, 1u, VK_FORMAT_R32G32B32_SFLOAT, offsetof( BillboardInstanceData, offset ) },
						{ 6u, 1u, VK_FORMAT_R32G32_SFLOAT, offsetof( BillboardInstanceData, dimensions ) },
					},
				};

				materialNode.pipelineLayout = m_device.getDevice().createPipelineLayout( { *m_billboardDescriptorLayout, *materialNode.layout } );
				materialNode.pipeline = doCreatePipeline( *materialNode.pipelineLayout
					, doCreateBillboardProgram( m_device.getDevice(), m_fragmentShaderFile )
					, std::move( vertexLayout )
					, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP 
					, VK_CULL_MODE_NONE );
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
		if ( !object.empty() )
		{
			ashes::VkDescriptorSetLayoutBindingArray bindings
			{
				{ 0u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			};
			doFillObjectDescriptorLayoutBindings( bindings );
			m_objectDescriptorLayout = m_device.getDevice().createDescriptorSetLayout( std::move( bindings ) );
			m_objectDescriptorPool = m_objectDescriptorLayout->createPool( m_objectsCount );

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
						materialNode.layout = m_device.getDevice().createDescriptorSetLayout( { { 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 6u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr } } );
						materialNode.pool = materialNode.layout->createPool( 1u );
						materialNode.descriptorSetTextures = materialNode.pool->createDescriptorSet( 1u );
						doFillTextures( *materialNode.layout
							, *materialNode.descriptorSetTextures
							, materialNode.textures );
						materialNode.descriptorSetTextures->update();

						// Initialise the pipeline
						ashes::PipelineVertexInputStateCreateInfo vertexLayout
						{
							0u,
							{
								{ 0u, sizeof( Vertex ), VK_VERTEX_INPUT_RATE_VERTEX },
							},
							{
								{ 0u, 0u, VK_FORMAT_R32G32B32_SFLOAT, offsetof( Vertex, position ) },
								{ 1u, 0u, VK_FORMAT_R32G32B32_SFLOAT, offsetof( Vertex, normal ) },
								{ 2u, 0u, VK_FORMAT_R32G32B32_SFLOAT, offsetof( Vertex, tangent ) },
								{ 3u, 0u, VK_FORMAT_R32G32B32_SFLOAT, offsetof( Vertex, bitangent ) },
								{ 4u, 0u, VK_FORMAT_R32G32_SFLOAT, offsetof( Vertex, texture ) },
							},
						};

						materialNode.pipelineLayout = m_device.getDevice().createPipelineLayout( { *m_objectDescriptorLayout, *materialNode.layout } );
						materialNode.pipeline = doCreatePipeline( *materialNode.pipelineLayout
							, doCreateObjectProgram( m_device.getDevice(), m_fragmentShaderFile )
							, std::move( vertexLayout )
							, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
							, VkCullModeFlagBits( material.data.backFace ? VK_CULL_MODE_FRONT_BIT : VK_CULL_MODE_BACK_BIT ) );
						m_submeshRenderNodes.emplace_back( std::move( materialNode ) );
						++matIndex;
					}
				}
			}
		}
	}

	void NodesRenderer::doFillTextures( ashes::DescriptorSetLayout const & layout
		, ashes::DescriptorSet & descriptorSet
		, TextureNodePtrArray const & textures )
	{
		for ( uint32_t index = 0u; index < textures.size(); ++index )
		{
			descriptorSet.createBinding( layout.getBinding( 0u, index )
				, textures[index]->view
				, *m_sampler
				, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
				, index );
		}

		for ( uint32_t index = textures.size(); index < 6u; ++index )
		{
			descriptorSet.createBinding( layout.getBinding( 0u, 0u )
				, m_dummyView
				, *m_sampler
				, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
				, index );
		}
	}

	ashes::GraphicsPipelinePtr NodesRenderer::doCreatePipeline( ashes::PipelineLayout const & pipelineLayout
		, ashes::PipelineShaderStageCreateInfoArray shaderStages
		, ashes::PipelineVertexInputStateCreateInfo vertexLayout
		, VkPrimitiveTopology topology
		, VkCullModeFlagBits cullMode )
	{
		ashes::VkPipelineColorBlendAttachmentStateArray bsAttaches;

		for ( auto & attach : m_renderPass->getAttachments() )
		{
			if ( !ashes::isDepthOrStencilFormat( attach.format ) )
			{
				bsAttaches.push_back( VkPipelineColorBlendAttachmentState
					{
						VK_FALSE,
						VK_BLEND_FACTOR_ONE,
						VK_BLEND_FACTOR_ZERO,
						VK_BLEND_OP_ADD,
						VK_BLEND_FACTOR_ONE,
						VK_BLEND_FACTOR_ZERO,
						VK_BLEND_OP_ADD,
						VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
					} );
			}
		}

		return m_device->createPipeline( ashes::GraphicsPipelineCreateInfo
			{
				0u,
				std::move( shaderStages ),
				std::move( vertexLayout ),
				ashes::PipelineInputAssemblyStateCreateInfo{ 0u, topology },
				std::nullopt,
				ashes::PipelineViewportStateCreateInfo{},
				ashes::PipelineRasterizationStateCreateInfo{ 0u, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VkCullModeFlags( cullMode ) },
				ashes::PipelineMultisampleStateCreateInfo{},
				ashes::PipelineDepthStencilStateCreateInfo{},
				ashes::PipelineColorBlendStateCreateInfo{ 0u, VK_FALSE, VK_LOGIC_OP_COPY, std::move( bsAttaches ) },
				ashes::PipelineDynamicStateCreateInfo{ 0u, { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR } },
				pipelineLayout,
				*m_renderPass,
			} );
	}
}
