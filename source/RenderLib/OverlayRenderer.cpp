#include "OverlayRenderer.h"

#include <GlLib/GlAttribute.h>
#include <GlLib/GlBuffer.h>
#include <GlLib/GlUniform.h>

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
		void doFillBuffers( Overlay::QuadArray::const_iterator begin
			, uint32_t count
			, uint32_t & offset
			, gl::Buffer< Overlay::Quad > const & buffer )
		{
			buffer.bind();
			buffer.upload( offset, count, &( *begin ) );
			buffer.unbind();
			offset += count;
		}

		std::vector< OverlayPtr > doSortPerZIndex( OverlayList const & overlays
			, gl::IVec2 const & size )
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


		void doBindMaterial( OverlayNode const & node
			, Material const & material )
		{
			node.m_colour->value( { material.ambient(), material.opacity() } );

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

		void doUnbindMaterial( OverlayNode const & node
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

	OverlayNode::OverlayNode( bool text
		, OpacityType opacity
		, TextureFlags textures )
		: m_program{ UberShader::createShaderProgram( RenderType::eScene
			, textures
			, opacity
			, text ? ObjectType::eTextOverlay : ObjectType::ePanelOverlay ) }
		, m_overlayUbo{ "Overlay", 0u, *m_program }
		, m_mpUniform{ &m_overlayUbo.createUniform< gl::Mat4 >( "mtxMP" ) }
		, m_colour{ &m_overlayUbo.createUniform< gl::RgbaColour >( "colour" ) }
		, m_mapColour{ gl::makeUniform< int >( "mapColour", *m_program ) }
		, m_mapOpacity{ gl::makeUniform< int >( "mapOpacity", *m_program ) }
		, m_position{ m_program->createAttribute< gl::Vec2 >( "position"
			, sizeof( TextOverlay::Vertex )
			, offsetof( TextOverlay::Vertex, coords ) ) }
		, m_texture{ m_program->createAttribute< gl::Vec2 >( "texture"
			, sizeof( TextOverlay::Vertex )
			, offsetof( TextOverlay::Vertex, text ) ) }
	{
		m_overlayUbo.initialise();
	}

	OverlayNodeArray doCreatePanelNodes()
	{
		return
		{
			{
				OverlayNode{ false, OpacityType::eOpaque, TextureFlag::eNone },
				OverlayNode{ false, OpacityType::eOpaque, TextureFlag::eDiffuse },
				OverlayNode{ false, OpacityType::eAlphaBlend, TextureFlag::eNone },
				OverlayNode{ false, OpacityType::eAlphaBlend, TextureFlag::eDiffuse },
				OverlayNode{ false, OpacityType::eAlphaBlend, TextureFlag::eOpacity },
				OverlayNode{ false, OpacityType::eAlphaBlend, TextureFlag::eDiffuse | TextureFlag::eOpacity },
				OverlayNode{ false, OpacityType::eAlphaTest, TextureFlag::eNone },
				OverlayNode{ false, OpacityType::eAlphaTest, TextureFlag::eDiffuse },
				OverlayNode{ false, OpacityType::eAlphaTest, TextureFlag::eOpacity },
				OverlayNode{ false, OpacityType::eAlphaTest, TextureFlag::eDiffuse | TextureFlag::eOpacity },
			}
		};
	}

	//*************************************************************************

	OverlayRenderer::OverlayRenderer( uint32_t maxCharsPerBuffer )
		: m_panelNodes{ doCreatePanelNodes() }
		, m_textNode{ true, OpacityType::eAlphaTest, TextureFlag::eOpacity }
		, m_pipeline{ true, false, true, true }
		, m_panelBuffer{ gl::makeBuffer( gl::BufferTarget::eArrayBuffer
				, std::vector< TextOverlay::Quad >( 1u ) ) }
		, m_borderBuffer{ gl::makeBuffer( gl::BufferTarget::eArrayBuffer
				, std::vector< TextOverlay::Quad >( 8u ) ) }
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

	void OverlayRenderer::beginRender( gl::IVec2 const & size )
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
		node.m_program->bind();
		m_panelBuffer->bind();
		m_panelBuffer->upload( 0u, 1u, overlay.panelVertex().data() );
		m_panelBuffer->unbind();
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
		node.m_program->bind();
		m_panelBuffer->bind();
		m_panelBuffer->upload( 0u, 1u, overlay.panelVertex().data() );
		m_panelBuffer->unbind();
		doDrawBuffer( *m_panelBuffer
			, 1u
			, overlay.transform()
			, overlay.material()
			, node );
		m_borderBuffer->bind();
		m_borderBuffer->upload( 0u, 8u, overlay.borderVertex().data() );
		m_borderBuffer->unbind();
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
		std::vector< gl::Buffer< Overlay::Quad > const * > buffers;

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

	gl::Buffer< Overlay::Quad > const & OverlayRenderer::doCreateTextBuffer()
	{
		auto buffer = gl::makeBuffer( gl::BufferTarget::eArrayBuffer
			, std::vector< TextOverlay::Quad >( m_maxCharsPerBuffer ) );
		m_textBuffers.push_back( std::move( buffer ) );
		return *m_textBuffers.back();
	}

	void OverlayRenderer::doDrawBuffer( gl::Buffer< Overlay::Quad > const & buffer
		, uint32_t count
		, gl::Mat4 const & transform
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

	void OverlayRenderer::doDrawBuffer( gl::Buffer< Overlay::Quad > const & buffer
		, uint32_t count
		, gl::Mat4 const & transform
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

	gl::Buffer< Overlay::Quad > const & OverlayRenderer::doFillTextPart
		( uint32_t count
		, uint32_t & offset
		, TextOverlay::QuadArray::const_iterator & it
		, uint32_t & index )
	{
		gl::Buffer< Overlay::Quad > const * buffer{ nullptr };

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
		doFillBuffers( it, count, offset, *buffer );
		it += count;

		return *buffer;
	}
}
