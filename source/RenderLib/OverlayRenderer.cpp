#include "OverlayRenderer.h"

#include <Renderer/Attribute.hpp>
#include <Renderer/GlUniform.h>
#include <Renderer/VertexBuffer.hpp>

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
		void doFillBuffers( renderer::RenderingResources const & resources
			, Overlay::QuadArray::const_iterator begin
			, uint32_t count
			, uint32_t & offset
			, renderer::VertexBuffer< Overlay::Quad > const & buffer )
		{
			resources.copyVertexData( reinterpret_cast< uint8_t const * const >( &( *begin ) )
				, uint32_t( count * sizeof( Overlay::Quad ) )
				, uint32_t( offset * sizeof( Overlay::Quad ) )
				, buffer.getVbo() );
			offset += count;
		}

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


		void doBindMaterial( OverlayNode & node
			, Material const & material )
		{
			node.m_overlayUbo.getData().colour = renderer::Vec4{ material.ambient(), material.opacity() };

			if ( material.hasDiffuseMap()
				&& node.m_mapColour->valid() )
			{
				node.m_mapColour->value( 0 );
				node.m_mapColour->bind();
				material.diffuseMap().bind( 0 );
			}

			if ( material.hasOpacityMap()
				&& node.m_mapOpacity->valid() )
			{
				node.m_mapOpacity->value( 1 );
				node.m_mapOpacity->bind();
				material.opacityMap().bind( 1 );
			}
		}

		void doUnbindMaterial( OverlayNode & node
			, Material const & material )
		{
			if ( material.hasOpacityMap()
				&& node.m_mapOpacity->valid() )
			{
				material.opacityMap().unbind( 1 );
			}

			if ( material.hasDiffuseMap()
				&& node.m_mapColour->valid() )
			{
				material.diffuseMap().unbind( 0 );
			}
		}
	}

	//*************************************************************************

	OverlayNode::OverlayNode( renderer::RenderingResources const & resources
		, bool text
		, OpacityType opacity
		, TextureFlags textures )
		: m_program{ UberShader::createShaderProgram( RenderType::eScene
			, textures
			, opacity
			, text ? ObjectType::eTextOverlay : ObjectType::ePanelOverlay ) }
		, m_overlayUbo{ resources, 1u, renderer::BufferTarget::eTransferDst, renderer::MemoryPropertyFlag::eDeviceLocal }
		, m_mapColour{ renderer::makeUniform< int >( "mapColour", *m_program ) }
		, m_mapOpacity{ renderer::makeUniform< int >( "mapOpacity", *m_program ) }
		, m_position{ m_program->createAttribute< renderer::Vec2 >( "position"
			, sizeof( TextOverlay::Vertex )
			, offsetof( TextOverlay::Vertex, coords ) ) }
		, m_texture{ m_program->createAttribute< renderer::Vec2 >( "texture"
			, sizeof( TextOverlay::Vertex )
			, offsetof( TextOverlay::Vertex, text ) ) }
	{
	}

	OverlayNodeArray doCreatePanelNodes( renderer::RenderingResources const & resources )
	{
		return
		{
			{
				OverlayNode{ resources, false, OpacityType::eOpaque, TextureFlag::eNone },
				OverlayNode{ resources, false, OpacityType::eOpaque, TextureFlag::eDiffuse },
				OverlayNode{ resources, false, OpacityType::eAlphaBlend, TextureFlag::eNone },
				OverlayNode{ resources, false, OpacityType::eAlphaBlend, TextureFlag::eDiffuse },
				OverlayNode{ resources, false, OpacityType::eAlphaBlend, TextureFlag::eOpacity },
				OverlayNode{ resources, false, OpacityType::eAlphaBlend, TextureFlag::eDiffuse | TextureFlag::eOpacity },
				OverlayNode{ resources, false, OpacityType::eAlphaTest, TextureFlag::eNone },
				OverlayNode{ resources, false, OpacityType::eAlphaTest, TextureFlag::eDiffuse },
				OverlayNode{ resources, false, OpacityType::eAlphaTest, TextureFlag::eOpacity },
				OverlayNode{ resources, false, OpacityType::eAlphaTest, TextureFlag::eDiffuse | TextureFlag::eOpacity },
			}
		};
	}

	//*************************************************************************

	OverlayRenderer::OverlayRenderer( renderer::RenderingResources const & resources
		, uint32_t maxCharsPerBuffer )
		: m_resources{ resources }
		, m_panelNodes{ doCreatePanelNodes( resources ) }
		, m_textNode{ resources, true, OpacityType::eAlphaTest, TextureFlag::eOpacity }
		, m_pipeline{ true, false, true, true }
		, m_panelBuffer{ renderer::makeVertexBuffer< Overlay::Quad >( resources
			, 0u
			, uint32_t( 1u * sizeof( Overlay::Quad ) )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_borderBuffer{ renderer::makeVertexBuffer< Overlay::Quad >( resources
			, 0u
			, uint32_t( 8u * sizeof( Overlay::Quad ) )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_maxCharsPerBuffer{ maxCharsPerBuffer }
		, m_viewport{ { 100, 100 } }
	{
		doCreateTextBuffer();
	}

	OverlayRenderer::~OverlayRenderer()
	{
	}

	void OverlayRenderer::draw( OverlayList const & overlays )
	{
		auto & size = m_viewport.size();
		auto sorted = doSortPerZIndex( overlays, size );

		for ( auto & overlay : sorted )
		{
			overlay->render( *this );
		}
	}

	void OverlayRenderer::beginRender( renderer::IVec2 const & size )
	{
		if ( m_viewport.size() != size )
		{
			m_sizeChanged = true;
			m_viewport.resize( size );
			m_viewport.ortho( 0.0f
				, float( size.x )
				, 0.0f
				, float( size.y )
				, 0.0f
				, 1000.0f );
			m_transform = m_viewport.transform();
		}

		m_pipeline.apply();
	}

	void OverlayRenderer::endRender()
	{
		m_sizeChanged = false;
	}

	void OverlayRenderer::drawPanel( PanelOverlay const & overlay )
	{
		auto & material = overlay.material();
		auto & node = m_panelNodes[size_t( UberShader::nodeType
			( material.opacityType()
			, material.textureFlags() ) )];
		m_resources.copyVertexData( overlay.panelVertex()
			, m_panelBuffer->getVbo() );
		node.m_program->bind();
		doDrawBuffer( *m_panelBuffer
			, 1u
			, overlay.transform()
			, overlay.material()
			, node );
		node.m_program->unbind();
	}

	void OverlayRenderer::drawBorderPanel( BorderPanelOverlay const & overlay )
	{
		auto & material = overlay.material();
		auto & node = m_panelNodes[size_t( UberShader::nodeType
			( material.opacityType()
			, material.textureFlags() ) )];
		m_resources.copyVertexData( overlay.panelVertex()
			, m_panelBuffer->getVbo() );
		m_resources.copyVertexData( overlay.borderVertex()
			, m_borderBuffer->getVbo() );
		node.m_program->bind();
		doDrawBuffer( *m_panelBuffer
			, 1u
			, overlay.transform()
			, overlay.material()
			, node );
		doDrawBuffer( *m_borderBuffer
			, 8u
			, overlay.transform()
			, overlay.borderMaterial()
			, node );
		node.m_program->unbind();
	}

	void OverlayRenderer::drawText( TextOverlay const & overlay )
	{
		uint32_t offset{ 0u };
		auto quads = overlay.textVertex();
		uint32_t count = uint32_t( quads.size() );
		auto it = quads.cbegin();
		uint32_t index{ 0u };
		std::vector< renderer::VertexBuffer< Overlay::Quad > const * > buffers;

		while ( count > m_maxCharsPerBuffer )
		{
			buffers.push_back( &doFillTextPart( count
				, offset
				, it
				, index ) );
			count -= m_maxCharsPerBuffer;
		}

		if ( count > 0 )
		{
			buffers.push_back( &doFillTextPart( count
				, offset
				, it
				, index ) );
		}

		count = uint32_t( quads.size() );
		m_textNode.m_program->bind();

		for ( auto & buffer : buffers )
		{
			doDrawBuffer( *buffer
				, std::min( count, m_maxCharsPerBuffer )
				, overlay.transform()
				, overlay.material()
				, overlay.fontTexture().texture()
				, m_textNode );
			count -= m_maxCharsPerBuffer;
		}

		m_textNode.m_program->unbind();
	}

	renderer::VertexBuffer< Overlay::Quad > const & OverlayRenderer::doCreateTextBuffer()
	{
		auto buffer = renderer::makeVertexBuffer< TextOverlay::Quad >( m_resources
			, 0u
			, m_maxCharsPerBuffer
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_textBuffers.push_back( std::move( buffer ) );
		return *m_textBuffers.back();
	}

	void OverlayRenderer::doDrawBuffer( renderer::VertexBuffer< Overlay::Quad > const & buffer
		, uint32_t count
		, renderer::Mat4 const & transform
		, Material const & material
		, OverlayNode const & node )
	{
		buffer.bind();
		node.m_position->bind();

		if ( material.textureFlags() )
		{
			node.m_texture->bind();
			node.m_mpUniform->value( m_transform * transform );
			doBindMaterial( node, material );
			node.m_overlayUbo.bind( 0u );
			glCheckError( glDrawArrays, GL_TRIANGLES, 0, count * 6 );
			doUnbindMaterial( node, material );
			node.m_texture->unbind();
		}
		else
		{
			node.m_mpUniform->value( m_transform * transform );
			doBindMaterial( node, material );
			node.m_overlayUbo.bind( 0u );
			glCheckError( glDrawArrays, GL_TRIANGLES, 0, count * 6 );
			doUnbindMaterial( node, material );
		}

		node.m_position->unbind();
		buffer.unbind();
	}

	void OverlayRenderer::doDrawBuffer( renderer::VertexBuffer< Overlay::Quad > const & buffer
		, uint32_t count
		, renderer::Mat4 const & transform
		, Material const & material
		, Texture const & textOpacity
		, OverlayNode const & node )
	{
		buffer.bind();
		node.m_position->bind();
		node.m_texture->bind();
		node.m_mpUniform->value( m_transform * transform );
		node.m_colour->value( { material.ambient(), material.opacity() } );
		node.m_mapOpacity->value( 0 );
		node.m_mapOpacity->bind();
		textOpacity.bind( 0 );
		node.m_overlayUbo.bind( 0u );
		glCheckError( glDrawArrays, GL_TRIANGLES, 0, count * 6 );
		textOpacity.unbind( 0 );
		node.m_texture->unbind();
		node.m_position->unbind();
		buffer.unbind();
	}

	renderer::VertexBuffer< Overlay::Quad > const & OverlayRenderer::doFillTextPart
		( uint32_t count
		, uint32_t & offset
		, TextOverlay::QuadArray::const_iterator & it
		, uint32_t & index )
	{
		renderer::VertexBuffer< Overlay::Quad > const * buffer{ nullptr };

		if ( offset + count > m_maxCharsPerBuffer )
		{
			buffer = &doCreateTextBuffer();
			offset = 0u;
			++index;
		}
		else
		{
			buffer = m_textBuffers[index].get();
		}

		count = std::min( count, m_maxCharsPerBuffer );
		doFillBuffers( m_resources, it, count, offset, *buffer );
		it += count;

		return *buffer;
	}
}
