#include "OverlayRenderer.h"

#include "Texture.h"

#include <Renderer/Attribute.hpp>
#include <Renderer/DepthStencilState.hpp>
#include <Renderer/DescriptorSet.hpp>
#include <Renderer/DescriptorSetLayoutBinding.hpp>
#include <Renderer/DescriptorSetPool.hpp>
#include <Renderer/MultisampleState.hpp>
#include <Renderer/StagingBuffer.hpp>
#include <Renderer/VertexBuffer.hpp>
#include <Renderer/VertexLayout.hpp>

#include "BorderPanelOverlay.h"
#include "ElementsList.h"
#include "Material.h"
#include "PanelOverlay.h"
#include "TextOverlay.h"
#include "UberShader.h"
#include "Viewport.h"

namespace render
{
	//*************************************************************************

	namespace
	{
		template< typename OvType >
		struct OverlayFinder
		{
			OverlayFinder( OvType const & overlay )
				: m_overlay{ overlay }
			{
			}

			bool operator()( RenderOverlayPtr< OvType > const & lookup )
			{
				return lookup->m_overlay.get() == &m_overlay;
			}

			OvType const & m_overlay;
		};

		std::vector< OverlayPtr > doSortPerZIndex( OverlayList const & overlays
			, renderer::IVec2 const & size )
		{
			std::vector< OverlayPtr > result;

			for ( auto & overlay : overlays )
			{
				auto & ovLeftTop = overlay.second->position();
				auto ovRightBottom = ovLeftTop + overlay.second->size();

				if ( overlay.second->visible()
					&& ovLeftTop.x < size.x
					&& ovLeftTop.y < size.y
					&& ovRightBottom.x > 0
					&& ovRightBottom.y > 0 )
				{
					auto it = std::lower_bound( std::begin( result )
						, std::end( result )
						, overlay.second->index()
						, []( OverlayPtr const & res, uint32_t index )
					{
						return res->index() <= index;
					} );

					if ( it == std::end( result ) )
					{
						result.push_back( overlay.second );
					}
					else
					{
						result.insert( it, overlay.second );
					}
				}
			}

			return result;
		}

		OverlayNodeArray doCreatePanelNodes( renderer::Device const & device
			, renderer::RenderPass const & renderPass )
		{
			return
			{
				{
					OverlayNode{ device, renderPass, false, OpacityType::eOpaque, TextureFlag::eNone },
					OverlayNode{ device, renderPass, false, OpacityType::eOpaque, TextureFlag::eDiffuse },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaBlend, TextureFlag::eNone },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaBlend, TextureFlag::eDiffuse },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaBlend, TextureFlag::eOpacity },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaBlend, TextureFlag::eDiffuse | TextureFlag::eOpacity },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaTest, TextureFlag::eNone },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaTest, TextureFlag::eDiffuse },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaTest, TextureFlag::eOpacity },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaTest, TextureFlag::eDiffuse | TextureFlag::eOpacity },
				}
			};
		}

		renderer::DescriptorSetLayoutPtr doCreateDescriptorLayout( renderer::Device const & device
			, TextureFlags flags
			, bool text )
		{
			renderer::DescriptorSetLayoutBindingArray bindings;

			bindings.emplace_back( UberShader::UboOverlayBinding
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eVertex | renderer::ShaderStageFlag::eFragment );

			if ( checkFlag( flags, TextureFlag::eDiffuse ) )
			{
				// Diffuse texture.
				bindings.emplace_back( UberShader::TextureDiffuseBinding
					, renderer::DescriptorType::eCombinedImageSampler
					, renderer::ShaderStageFlag::eFragment );
			}

			if ( checkFlag( flags, TextureFlag::eOpacity )
				|| text )
			{
				// Opacity texture
				bindings.emplace_back( UberShader::TextureOpacityBinding
					, renderer::DescriptorType::eCombinedImageSampler
					, renderer::ShaderStageFlag::eFragment );
			}

			return device.createDescriptorSetLayout( std::move( bindings ) );
		}

		template< typename Data, typename OvType >
		bool doAddOverlay( uint32_t maxCharsPerBuffer
			, OverlayPtr overlay
			, OverlayNode const & node
			, OverlayVbo< Data, OvType > & vbo )
		{
			bool result = vbo.m_overlays.size() < vbo.m_data.size();

			if ( result )
			{
				if ( vbo.m_overlays.end() != std::find_if( vbo.m_overlays.begin()
					, vbo.m_overlays.end()
					, [&overlay]( RenderOverlayPtr< OvType > const & lookup )
					{
						return lookup->m_overlay == overlay;
					} ) )
				{
					assert( false && "Overlay has already been added" );
					return result;
				}

				auto & material = overlay->material();
				vbo.m_overlays.push_back( makeRenderOverlay( *node.m_descriptorPool
					, std::static_pointer_cast< OvType >( overlay )
					, uint32_t( vbo.m_overlays.size() )
					, maxCharsPerBuffer
					, *vbo.m_buffer
					, *node.m_layout
					, *node.m_overlayUbo ) );
				result = true;
			}

			return result;
		}

		template< typename Data, typename OvType >
		bool doRemoveOverlay( OverlayPtr overlay
			, OverlayVbo< Data, OvType > & vbo )
		{
			auto itr = std::find_if( vbo.m_overlays.begin()
				, vbo.m_overlays.end()
				, [overlay]( RenderOverlayPtr< OvType > & lookup )
				{
					return lookup->m_overlay == overlay;
				} );

			if ( itr == vbo.m_overlays.end() )
			{
				return false;
			}

			itr = vbo.m_overlays.erase( itr );
			auto index = uint32_t( std::distance( itr = vbo.m_overlays.begin(), itr ) );

			for ( auto it = itr; it != vbo.m_overlays.end(); ++it )
			{
				( *it )->createDescriptor( index++ );
			}

			return true;
		}

		void doCopyBuffer( PanelOverlay const & overlay
			, uint32_t index
			, std::vector< Overlay::Quad > & buffer )
		{
			buffer[index] = overlay.panelVertex();
		}

		void doCopyBuffer( BorderPanelOverlay const & overlay
			, uint32_t index
			, std::vector< Overlay::Quad > & buffer )
		{
			buffer[index] = overlay.panelVertex();
		}

		void doCopyBuffer( BorderPanelOverlay const & overlay
			, uint32_t index
			, std::vector< BorderPanelOverlay::BorderQuads > & buffer )
		{
			buffer[index] = overlay.borderVertex();
		}

		void doCopyBuffer( TextOverlay const & overlay
			, uint32_t index
			, std::vector< Overlay::Quad > & buffer )
		{
			std::copy( overlay.textVertex().begin()
				, overlay.textVertex().end()
				, buffer.begin() + index );
		}

		template< typename Data, typename OvType >
		void doUpdate( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, OverlayNode & node
			, uint32_t count
			, renderer::Mat4 const & projection
			, OverlayVbo< Data, OvType > & vbo )
		{
			if ( !vbo.m_overlays.empty() )
			{
				uint32_t index = 0u;

				for ( auto & overlay : vbo.m_overlays )
				{
					auto & data = node.m_overlayUbo->getData( index );
					data.modelProj = projection * overlay->m_overlay->transform();
					data.colour = renderer::RgbaColour{ overlay->m_overlay->material().ambient()
						, overlay->m_overlay->material().opacity() };
					doCopyBuffer( *overlay->m_overlay
						, uint32_t( index * count )
						, vbo.m_data );
					++index;
				}

				stagingBuffer.copyUniformData( commandBuffer
					, node.m_overlayUbo->getDatas()
					, *node.m_overlayUbo
					, renderer::PipelineStageFlag::eVertexShader | renderer::PipelineStageFlag::eFragmentShader );
				stagingBuffer.copyVertexData( commandBuffer
					, vbo.m_data
					, *vbo.m_buffer
					, renderer::PipelineStageFlag::eVertexInput );
			}
		}
	}

	//*************************************************************************

	OverlayNode::OverlayNode( renderer::Device const & device
		, renderer::RenderPass const & renderPass
		, bool text
		, OpacityType opacity
		, TextureFlags textures )
		: m_program{ UberShader::createShaderProgram( device
			, RenderType::eScene
			, textures
			, opacity
			, text ? ObjectType::eTextOverlay : ObjectType::ePanelOverlay ) }
		, m_layout{ renderer::makeLayout< TextOverlay::Vertex >( device, 0u ) }
		, m_overlayUbo{ std::make_unique< renderer::UniformBuffer< OverlayUbo > >( device
			, MaxObjectsCount
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_descriptorLayout{ doCreateDescriptorLayout( device, textures, text ) }
		, m_descriptorPool{ m_descriptorLayout->createPool( MaxObjectsCount ) }
	{
		m_layout->createAttribute< renderer::Vec2 >( 0u
			, offsetof( TextOverlay::Vertex, position ) );
		m_layout->createAttribute< renderer::Vec2 >( 1u
			, offsetof( TextOverlay::Vertex, texture ) );

		renderer::ColourBlendState bsState;
		bsState.addAttachment( renderer::ColourBlendStateAttachment{ true
			, renderer::BlendFactor::eSrcAlpha
			, renderer::BlendFactor::eInvSrcAlpha
			, renderer::BlendOp::eAdd
			, renderer::BlendFactor::eSrcAlpha
			, renderer::BlendFactor::eInvSrcAlpha
			, renderer::BlendOp::eAdd } );
		m_pipelineLayout = device.createPipelineLayout( *m_descriptorLayout );
		m_pipeline = device.createPipeline( *m_pipelineLayout
			, *m_program
			, { *m_layout }
			, renderPass
			, renderer::PrimitiveTopology::eTriangleList
			, renderer::RasterisationState{}
			, bsState );
		m_pipeline->multisampleState( renderer::MultisampleState{} )
			.depthStencilState( renderer::DepthStencilState{} )
			.finish();
	}

	//*************************************************************************

	template< typename OvType >
	RenderOverlay< OvType >::RenderOverlay( renderer::DescriptorSetPool const & pool
		, std::shared_ptr< OvType > overlay
		, uint32_t index
		, renderer::UniformBuffer< OverlayUbo > const & overlayUbo
		, Texture const * opacity )
		: m_pool{ pool }
		, m_overlayUbo{ overlayUbo }
		, m_opacity{ opacity }
		, m_overlay{ overlay }
	{
		createDescriptor( index );
	}

	template< typename OvType >
	void RenderOverlay< OvType >::createDescriptor( uint32_t index )
	{
		m_index = index;
		m_descriptor = m_pool.createDescriptorSet();
		m_descriptor->createBinding( m_pool.getLayout().getBinding( UberShader::UboOverlayBinding )
			, m_overlayUbo
			, index );

		auto & material = m_overlay->material();

		if ( material.hasDiffuseMap() )
		{
			m_descriptor->createBinding( m_pool.getLayout().getBinding( UberShader::TextureDiffuseBinding )
				, material.diffuseMap().texture().getView()
				, material.diffuseMap().sampler() );
		}

		if ( m_opacity )
		{
			m_descriptor->createBinding( m_pool.getLayout().getBinding( UberShader::TextureOpacityBinding )
				, m_opacity->texture().getView()
				, m_opacity->sampler() );
		}

		m_descriptor->update();
	}

	//*************************************************************************
	
	template< typename OvType, typename Data >
	RenderOverlayPtr< OvType > makeRenderOverlay( renderer::DescriptorSetPool const & pool
		, std::shared_ptr< OvType > overlay
		, uint32_t index
		, uint32_t maxCharsPerBuffer
		, renderer::VertexBuffer< Data > const & vbo
		, renderer::VertexLayout const & layout
		, renderer::UniformBuffer< OverlayUbo > const & overlayUbo )
	{
		auto result = std::make_unique< RenderOverlay< OvType > >( pool
			, overlay
			, index
			, overlayUbo
			, overlay->material().hasOpacityMap()
				? &overlay->material().opacityMap()
			: nullptr );
		result->m_vao = vbo.getDevice().createGeometryBuffers( vbo
			, index * sizeof( Data )
			, layout );
		return result;
	}
	
	RenderOverlayPtr< TextOverlay > makeRenderOverlay( renderer::DescriptorSetPool const & pool
		, TextOverlayPtr overlay
		, uint32_t index
		, uint32_t maxCharsPerBuffer
		, renderer::VertexBuffer< Overlay::Quad > const & vbo
		, renderer::VertexLayout const & layout
		, renderer::UniformBuffer< OverlayUbo > const & overlayUbo )
	{
		auto result = std::make_unique< RenderOverlay< TextOverlay > >( pool
			, overlay
			, index
			, overlayUbo
			, &overlay->fontTexture().texture() );
		result->m_vao = vbo.getDevice().createGeometryBuffers( vbo
			, index * maxCharsPerBuffer * sizeof( Overlay::Quad )
			, layout );
		return result;
	}

	//*************************************************************************

	template< typename Data, typename OvType >
	OverlayVbo< Data, OvType >::OverlayVbo( renderer::Device const & device
		, uint32_t count )
		: m_device{ device }
		, m_buffer{ renderer::makeVertexBuffer< Data >( device
			, count
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_data{ count, Data{} }
	{
	}

	//*************************************************************************

	OverlayRenderer::OverlayRenderer( renderer::Device const & device
		, renderer::RenderPass const & renderPass
		, renderer::CommandPool const & commandPool
		, renderer::IVec2 const & size
		, uint32_t maxCharsPerBuffer )
		: m_device{ device }
		, m_renderPass{ renderPass }
		, m_panelNodes{ doCreatePanelNodes( device, renderPass ) }
		, m_borderNodesPanel{ doCreatePanelNodes( device, renderPass ) }
		, m_borderNodesBorder{ doCreatePanelNodes( device, renderPass ) }
		, m_textNode{ device, renderPass, true, OpacityType::eAlphaTest, TextureFlag::eOpacity }
		, m_drawCommandBuffer{ commandPool.createCommandBuffer( false ) }
		, m_updateCommandBuffer{ commandPool.createCommandBuffer() }
		, m_stagingBuffer{ std::make_unique< renderer::StagingBuffer >( device
			, 0u
			, uint32_t( MaxObjectsCount * maxCharsPerBuffer * sizeof( BorderPanelOverlay::BorderQuads ) ) ) }
		, m_panelOverlays
		{
			PanelOverlayVbo{ device, MaxObjectsCount },
			PanelOverlayVbo{ device, MaxObjectsCount },
			PanelOverlayVbo{ device, MaxObjectsCount },
			PanelOverlayVbo{ device, MaxObjectsCount },
			PanelOverlayVbo{ device, MaxObjectsCount },
			PanelOverlayVbo{ device, MaxObjectsCount },
			PanelOverlayVbo{ device, MaxObjectsCount },
			PanelOverlayVbo{ device, MaxObjectsCount },
			PanelOverlayVbo{ device, MaxObjectsCount },
			PanelOverlayVbo{ device, MaxObjectsCount },
		}
		, m_borderOverlaysPanels
		{
			BorderOverlayPanelVbo{ device, MaxObjectsCount },
			BorderOverlayPanelVbo{ device, MaxObjectsCount },
			BorderOverlayPanelVbo{ device, MaxObjectsCount },
			BorderOverlayPanelVbo{ device, MaxObjectsCount },
			BorderOverlayPanelVbo{ device, MaxObjectsCount },
			BorderOverlayPanelVbo{ device, MaxObjectsCount },
			BorderOverlayPanelVbo{ device, MaxObjectsCount },
			BorderOverlayPanelVbo{ device, MaxObjectsCount },
			BorderOverlayPanelVbo{ device, MaxObjectsCount },
			BorderOverlayPanelVbo{ device, MaxObjectsCount },
		}
		, m_borderOverlaysBorders
		{
			BorderOverlayBorderVbo{ device, MaxObjectsCount },
			BorderOverlayBorderVbo{ device, MaxObjectsCount },
			BorderOverlayBorderVbo{ device, MaxObjectsCount },
			BorderOverlayBorderVbo{ device, MaxObjectsCount },
			BorderOverlayBorderVbo{ device, MaxObjectsCount },
			BorderOverlayBorderVbo{ device, MaxObjectsCount },
			BorderOverlayBorderVbo{ device, MaxObjectsCount },
			BorderOverlayBorderVbo{ device, MaxObjectsCount },
			BorderOverlayBorderVbo{ device, MaxObjectsCount },
			BorderOverlayBorderVbo{ device, MaxObjectsCount },
		}
		, m_maxCharsPerBuffer{ maxCharsPerBuffer }
		, m_viewport{ { 100, 100 } }
	{
		resize( size );
		m_textOverlays.emplace_back( device, MaxObjectsCount * m_maxCharsPerBuffer );
	}

	OverlayRenderer::~OverlayRenderer()
	{
	}

	void OverlayRenderer::registerOverlay( PanelOverlayPtr overlay )
	{
		assert( overlay && "Null overlay given" );
		auto & material = overlay->material();
		auto node = UberShader::nodeType( material.opacityType()
			, material.textureFlags() );
		doAddOverlay( m_maxCharsPerBuffer
			, overlay
			, m_panelNodes[size_t( node )]
			, m_panelOverlays[size_t( node )] );
		m_connections[overlay.get()] = overlay->onChanged.connect( [this]( Overlay & overlay )
		{
			m_changed = true;
		} );
		m_changed = true;
	}

	void OverlayRenderer::registerOverlay( BorderPanelOverlayPtr overlay )
	{
		assert( overlay && "Null overlay given" );
		auto & material = overlay->material();
		auto node = UberShader::nodeType( material.opacityType()
			, material.textureFlags() );
		doAddOverlay( m_maxCharsPerBuffer
			, overlay
			, m_borderNodesPanel[size_t( node )]
			, m_borderOverlaysPanels[size_t( node )] );
		doAddOverlay( m_maxCharsPerBuffer
			, overlay
			, m_borderNodesBorder[size_t( node )]
			, m_borderOverlaysBorders[size_t( node )] );
		m_connections[overlay.get()] = overlay->onChanged.connect( [this]( Overlay & overlay )
		{
			m_changed = true;
		} );
		m_changed = true;
	}

	void OverlayRenderer::registerOverlay( TextOverlayPtr overlay )
	{
		doAddOverlay( m_maxCharsPerBuffer
			, overlay
			, m_textNode
			, m_textOverlays[m_textOverlays.size() - 1] );
		m_connections[overlay.get()] = overlay->onChanged.connect( [this]( Overlay & overlay )
		{
			m_changed = true;
		} );
		m_changed = true;
	}

	void OverlayRenderer::unregisterOverlay( OverlayPtr overlay )
	{
		switch ( overlay->type() )
		{
		case Overlay::Type::ePanel:
			unregisterOverlay( std::static_pointer_cast< PanelOverlay >( overlay ) );
			break;

		case Overlay::Type::eBorderPanel:
			unregisterOverlay( std::static_pointer_cast< BorderPanelOverlay >( overlay ) );
			break;

		case Overlay::Type::eText:
			unregisterOverlay( std::static_pointer_cast< TextOverlay >( overlay ) );
			break;
		}
	}

	void OverlayRenderer::unregisterOverlay( PanelOverlayPtr overlay )
	{
		auto & material = overlay->material();
		auto node = UberShader::nodeType( material.opacityType()
			, material.textureFlags() );
		bool result = doRemoveOverlay( overlay, m_panelOverlays[size_t( node )] );
		assert( result && "Overlay not found in the list" );
		m_connections.erase( m_connections.find( overlay.get() ) );
		m_changed = true;
	}

	void OverlayRenderer::unregisterOverlay( BorderPanelOverlayPtr overlay )
	{
		auto & material = overlay->material();
		auto node = UberShader::nodeType( material.opacityType()
			, material.textureFlags() );
		bool result = doRemoveOverlay( overlay, m_borderOverlaysPanels[size_t( node )] );
		assert( result && "Overlay panel not found in the list" );
		result = doRemoveOverlay( overlay, m_borderOverlaysBorders[size_t( node )] );
		assert( result && "Overlay borders not found in the list" );
		m_connections.erase( m_connections.find( overlay.get() ) );
		m_changed = true;
	}

	void OverlayRenderer::unregisterOverlay( TextOverlayPtr overlay )
	{
		bool result = false;
		auto it = m_textOverlays.begin();

		while ( !result )
		{
			result = doRemoveOverlay( overlay, *it );
		}

		assert( result && "Overlay not found in the list" );
		m_changed = true;
		m_connections.erase( m_connections.find( overlay.get() ) );
	}

	void OverlayRenderer::update()
	{
		if ( m_changed )
		{
			uint32_t index = 0;

			for ( auto & renderOverlays : m_panelOverlays )
			{
				doUpdate( *m_stagingBuffer
					, *m_updateCommandBuffer
					, m_panelNodes[index++]
					, 1u
					, m_transform
					, renderOverlays );
			}

			index = 0;

			for ( auto & renderOverlays : m_borderOverlaysPanels )
			{
				doUpdate( *m_stagingBuffer
					, *m_updateCommandBuffer
					, m_borderNodesPanel[index++]
					, 1u
					, m_transform
					, renderOverlays );
			}

			index = 0;

			for ( auto & renderOverlays : m_borderOverlaysBorders )
			{
				doUpdate( *m_stagingBuffer
					, *m_updateCommandBuffer
					, m_borderNodesBorder[index++]
					, 1u
					, m_transform
					, renderOverlays );
			}

			for ( auto & vbos : m_textOverlays )
			{
				doUpdate( *m_stagingBuffer
					, *m_updateCommandBuffer
					, m_textNode
					, m_maxCharsPerBuffer
					, m_transform
					, vbos );
			}

			m_changed = false;
		}
	}

	void OverlayRenderer::draw( renderer::FrameBuffer const & frameBuffer
		, OverlayList const & overlays )const
	{
		if ( m_drawCommandBuffer->begin( renderer::CommandBufferUsageFlag::eRenderPassContinue
			, m_renderPass
			, 0u
			, frameBuffer ) )
		{
			draw( *m_drawCommandBuffer
				, overlays );
			m_drawCommandBuffer->end();
		}
	}

	void OverlayRenderer::draw( renderer::CommandBuffer const & commandBuffer
		, OverlayList const & overlays )const
	{
		auto & size = m_viewport.size();
		auto sorted = doSortPerZIndex( overlays, size );

		for ( auto & overlay : sorted )
		{
			overlay->render( commandBuffer
				, *this );
		}

		m_sizeChanged = false;
	}

	void OverlayRenderer::resize( renderer::IVec2 const & size )
	{
		if ( m_viewport.size() != size )
		{
			m_sizeChanged = true;
			m_viewport.resize( size );
			m_viewport.ortho( 0.0f
				, float( size.x )
				, 0.0f
				, float( size.y )
				, -1.0f
				, 1.0f );
			m_transform = m_viewport.transform();
			m_changed = true;
		}
	}

	void OverlayRenderer::drawPanel( renderer::CommandBuffer const & commandBuffer
		, PanelOverlay const & overlay )const
	{
		auto & material = overlay.material();
		auto nodeType = size_t( UberShader::nodeType( material.opacityType()
			, material.textureFlags() ) );
		auto & overlays = m_panelOverlays[nodeType];
		auto & node = m_panelNodes[nodeType];
		commandBuffer.bindPipeline( *node.m_pipeline );
		commandBuffer.setViewport( m_viewport.viewport() );
		commandBuffer.setScissor( m_viewport.scissor() );
		auto it = std::find_if( overlays.m_overlays.begin()
			, overlays.m_overlays.end()
			, OverlayFinder< PanelOverlay >{ overlay } );
		assert( it != overlays.m_overlays.end() );
		doDrawBuffer( commandBuffer
			, *( *it )->m_vao
			, 1u
			, node
			, *( *it )->m_descriptor );
	}

	void OverlayRenderer::drawBorderPanel( renderer::CommandBuffer const & commandBuffer
		, BorderPanelOverlay const & overlay )const
	{
		auto & material = overlay.material();
		auto nodeType = size_t( UberShader::nodeType( material.opacityType()
			, material.textureFlags() ) );
		auto & node = m_panelNodes[nodeType];
		commandBuffer.bindPipeline( *node.m_pipeline );
		commandBuffer.setViewport( m_viewport.viewport() );
		commandBuffer.setScissor( m_viewport.scissor() );
		auto & panelOverlays = m_borderOverlaysPanels[nodeType];
		auto itPanel = std::find_if( panelOverlays.m_overlays.begin()
			, panelOverlays.m_overlays.end()
			, OverlayFinder< BorderPanelOverlay >{ overlay } );
		assert( itPanel != panelOverlays.m_overlays.end() );
		doDrawBuffer( commandBuffer
			, *( *itPanel )->m_vao
			, 1u
			, node
			, *( *itPanel )->m_descriptor );
		auto & borderOverlays = m_borderOverlaysBorders[nodeType];
		auto itBorder = std::find_if( borderOverlays.m_overlays.begin()
			, borderOverlays.m_overlays.end()
			, OverlayFinder< BorderPanelOverlay >{ overlay } );
		assert( itBorder != borderOverlays.m_overlays.end() );
		doDrawBuffer( commandBuffer
			, *( *itBorder )->m_vao
			, 8u
			, node
			, *( *itBorder )->m_descriptor );
	}

	void OverlayRenderer::drawText( renderer::CommandBuffer const & commandBuffer
		, TextOverlay const & overlay )const
	{
		uint32_t offset{ 0u };
		auto & quads = overlay.textVertex();
		auto count = uint32_t( quads.size() );
		commandBuffer.bindPipeline( *m_textNode.m_pipeline );
		commandBuffer.setViewport( m_viewport.viewport() );
		commandBuffer.setScissor( m_viewport.scissor() );
		auto it = std::find_if( m_textOverlays.begin()
			, m_textOverlays.end()
			, [&overlay, &commandBuffer, count, this]( TextOverlayVbo const & vboLookup )
			{
				auto itText = std::find_if( vboLookup.m_overlays.begin()
					, vboLookup.m_overlays.end()
					, OverlayFinder< TextOverlay >{ overlay } );

				if ( itText != vboLookup.m_overlays.end() )
				{
					doDrawBuffer( commandBuffer
						//, *vboLookup.m_buffer
						//, ( *itText )->m_index * m_maxCharsPerBuffer
						, *( *itText )->m_vao
						, std::min( count, m_maxCharsPerBuffer )
						, m_textNode
						, *( *itText )->m_descriptor );
				}

				return itText != vboLookup.m_overlays.end();
			} );
		assert( it != m_textOverlays.end() );
	}

	void OverlayRenderer::doDrawBuffer( renderer::CommandBuffer const & commandBuffer
		, renderer::GeometryBuffers const & buffer
		, uint32_t count
		, OverlayNode const & node
		, renderer::DescriptorSet const & descriptor )const
	{
		commandBuffer.bindGeometryBuffers( buffer );
		commandBuffer.bindDescriptorSet( descriptor
			, *node.m_pipelineLayout );
		commandBuffer.draw( count * 6u
			, 1u
			, 0u
			, 0u );
	}

	//void OverlayRenderer::doDrawBuffer( renderer::CommandBuffer const & commandBuffer
	//	, renderer::GeometryBuffers const & buffer
	//	, uint32_t count
	//	, OverlayNode const & node
	//	, renderer::DescriptorSet const & descriptor )const
	//{
	//	commandBuffer.bindGeometryBuffers( buffer );
	//	commandBuffer.bindDescriptorSet( descriptor
	//		, *node.m_pipelineLayout );
	//	commandBuffer.draw( count * 6u * 8u
	//		, 1u
	//		, 0u
	//		, 0u );
	//}

	void OverlayRenderer::onOverlayChanged( Overlay & overlay )
	{
		m_changed = true;
	}
}
