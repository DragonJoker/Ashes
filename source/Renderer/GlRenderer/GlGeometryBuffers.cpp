/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlGeometryBuffers.hpp"

#include "GlAttribute.hpp"
#include "GlBuffer.hpp"
#include "GlVertexLayout.hpp"

#include <Renderer/VertexBuffer.hpp>

#define BufferOffset( n ) ( ( uint8_t * )nullptr + ( n ) )

namespace gl_renderer
{
	GeometryBuffers::GeometryBuffers( renderer::VertexBufferBase const & vbo
		, uint64_t offset
		, renderer::VertexLayout const & layout )
		: renderer::GeometryBuffers{ vbo, offset, layout }
	{
		doInitialise();
	}

	GeometryBuffers::GeometryBuffers( renderer::VertexBufferBase const & vbo
		, uint64_t offset
		, renderer::VertexLayout const & layout
		, renderer::BufferBase const & ibo
		, uint64_t iboOffset
		, renderer::IndexType type )
		: renderer::GeometryBuffers{ vbo, offset, layout, ibo, iboOffset, type }
	{
		doInitialise();
	}

	GeometryBuffers::GeometryBuffers( renderer::VertexBufferCRefArray const & vbos
		, std::vector< uint64_t > offsets
		, renderer::VertexLayoutCRefArray const & layouts )
		: renderer::GeometryBuffers{ vbos, offsets, layouts }
	{
		doInitialise();
	}

	GeometryBuffers::GeometryBuffers( renderer::VertexBufferCRefArray const & vbos
		, std::vector< uint64_t > offsets
		, renderer::VertexLayoutCRefArray const & layouts
		, renderer::BufferBase const & ibo
		, uint64_t iboOffset
		, renderer::IndexType type )
		: renderer::GeometryBuffers{ vbos, offsets, layouts, ibo, iboOffset, type }
	{
		doInitialise();
	}

	GeometryBuffers::~GeometryBuffers()noexcept
	{
		glDeleteVertexArrays( 1, &m_vao );
	}

	void GeometryBuffers::doInitialise()
	{
		glCreateVertexArrays( 1, &m_vao );

		if ( m_vao == GL_INVALID_INDEX )
		{
			throw std::runtime_error{ "Couldn't create VAO" };
		}

		glBindVertexArray( m_vao );

		for ( auto & vbo : getVbos() )
		{
			glBindBuffer( GL_ARRAY_BUFFER
				, static_cast< Buffer const & >( vbo.vbo.getBuffer() ).getBuffer() );

			for ( auto & attribute : static_cast< VertexLayout const & >( vbo.layout ) )
			{
				glEnableVertexAttribArray( attribute.getLocation() );
				glVertexAttribPointer( attribute.getLocation()
					, getCount( attribute.getFormat() )
					, getType( attribute.getFormat() )
					, false
					, vbo.layout.getStride()
					, BufferOffset( vbo.offset + attribute.getOffset() ) );
			}
		}

		if ( hasIbo() )
		{
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER
				, static_cast< Buffer const & >( getIbo().buffer ).getBuffer() );
		}

		glBindVertexArray( 0u );
	}
}
