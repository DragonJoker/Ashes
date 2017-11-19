#include "Submesh.h"

#include "Mesh.h"

#include <GlLib/GlAttribute.h>
#include <GlLib/GlBuffer.h>

namespace render
{
	namespace
	{
		template< typename T >
		void doBindAttribBuffer( gl::Buffer< T > const & buffer
			, gl::Attribute< T > const & attribute )noexcept
		{
			if ( attribute.valid() )
			{
				buffer.bind();
				attribute.bind();
			}
		}
		template< typename T >
		void doUnbindAttribBuffer( gl::Buffer< T > const & buffer
			, gl::Attribute< T > const & attribute )noexcept
		{
			if ( attribute.valid() )
			{
				attribute.unbind();
				buffer.unbind();
			}
		}
	}

	Submesh::Submesh( Mesh const & mesh, UInt16Array const & idx )
		: m_mesh{ mesh }
		, m_index{ gl::BufferTarget::eElementArrayBuffer, idx }
	{
	}

	void Submesh::draw()const noexcept
	{
		m_index.bind();
		glCheckError( glDrawElements
			, GL_TRIANGLES
			, GLsizei( m_index.count() )
			, GL_UNSIGNED_SHORT
			, nullptr );
		m_index.unbind();
	}

	void Submesh::bind( gl::Vec3Attribute const * position
		, gl::Vec3Attribute const * normal
		, gl::Vec2Attribute const * texture )const noexcept
	{
		doBindAttribBuffer( *m_mesh.m_positions, *position );

		if ( normal && m_mesh.m_normal )
		{
			doBindAttribBuffer( *m_mesh.m_normal, *normal );
		}

		if ( texture && m_mesh.m_texcoord )
		{
			doBindAttribBuffer( *m_mesh.m_texcoord, *texture );
		}
	}

	void Submesh::unbind( gl::Vec3Attribute const * position
		, gl::Vec3Attribute const * normal
		, gl::Vec2Attribute const * texture )const noexcept
	{
		if ( texture && m_mesh.m_texcoord )
		{
			doUnbindAttribBuffer( *m_mesh.m_texcoord, *texture );
		}

		if ( normal && m_mesh.m_normal )
		{
			doUnbindAttribBuffer( *m_mesh.m_normal, *normal );
		}

		doUnbindAttribBuffer( *m_mesh.m_positions, *position );
	}
}
