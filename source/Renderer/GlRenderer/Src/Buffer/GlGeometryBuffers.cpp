/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "Buffer/GlGeometryBuffers.hpp"

#include "Buffer/GlBuffer.hpp"

#include <Buffer/VertexBuffer.hpp>
#include <Pipeline/VertexLayout.hpp>

namespace gl_renderer
{
	namespace
	{
		bool isInteger( renderer::AttributeFormat format )
		{
			return format == renderer::AttributeFormat::eInt
				|| format == renderer::AttributeFormat::eUInt
				|| format == renderer::AttributeFormat::eVec2i
				|| format == renderer::AttributeFormat::eVec2ui
				|| format == renderer::AttributeFormat::eVec3i
				|| format == renderer::AttributeFormat::eVec3ui
				|| format == renderer::AttributeFormat::eVec4i
				|| format == renderer::AttributeFormat::eVec4ui;
		}
	}

	GeometryBuffers::GeometryBuffers( renderer::VertexBufferCRefArray const & vbos
		, std::vector< uint64_t > offsets
		, renderer::VertexInputState const & vertexInputState )
		: renderer::GeometryBuffers{ vbos, offsets, vertexInputState }
	{
		doInitialise();
	}

	GeometryBuffers::GeometryBuffers( renderer::VertexBufferCRefArray const & vbos
		, std::vector< uint64_t > offsets
		, renderer::VertexInputState const & vertexInputState
		, renderer::BufferBase const & ibo
		, uint64_t iboOffset
		, renderer::IndexType type )
		: renderer::GeometryBuffers{ vbos, offsets, vertexInputState, ibo, iboOffset, type }
	{
		doInitialise();
	}

	GeometryBuffers::~GeometryBuffers()noexcept
	{
		glLogCall( gl::DeleteVertexArrays, 1, &m_vao );
	}

	void GeometryBuffers::doInitialise()
	{
		glLogCall( gl::GenVertexArrays, 1, &m_vao );

		if ( m_vao == GL_INVALID_INDEX )
		{
			throw std::runtime_error{ "Couldn't create VAO" };
		}

		glLogCall( gl::BindVertexArray, m_vao );

		for ( auto & vbo : getVbos() )
		{
			glLogCall( gl::BindBuffer
				, GL_BUFFER_TARGET_ARRAY
				, static_cast< Buffer const & >( vbo.vbo.getBuffer() ).getBuffer() );

			if ( vbo.binding.inputRate == renderer::VertexInputRate::eVertex )
			{
				for ( auto & attribute : vbo.attributes )
				{
					glLogCall( gl::EnableVertexAttribArray, attribute.location );

					if ( isInteger( attribute.format ) )
					{
						glLogCall( gl::VertexAttribIPointer
							, attribute.location
							, getCount( attribute.format )
							, getType( attribute.format )
							, vbo.binding.stride
							, BufferOffset( vbo.offset + attribute.offset ) );
					}
					else
					{
						glLogCall( gl::VertexAttribPointer
							, attribute.location
							, getCount( attribute.format )
							, getType( attribute.format )
							, false
							, vbo.binding.stride
							, BufferOffset( vbo.offset + attribute.offset ) );
					}
				}
			}
			else
			{
				for ( auto & attribute : vbo.attributes )
				{
					auto format = attribute.format;
					uint32_t offset = attribute.offset;
					uint32_t location = attribute.location;
					uint32_t divisor = 1u;

					glLogCall( gl::EnableVertexAttribArray, location );

					if ( isInteger( attribute.format ) )
					{
						glLogCall( gl::VertexAttribIPointer
							, location
							, getCount( format )
							, getType( format )
							, vbo.binding.stride
							, BufferOffset( vbo.offset + offset ) );
					}
					else
					{
						glLogCall( gl::VertexAttribPointer
							, location
							, getCount( format )
							, getType( format )
							, false
							, vbo.binding.stride
							, BufferOffset( vbo.offset + offset ) );
					}

					glLogCall( gl::VertexAttribDivisor
						, location
						, divisor );
				}
			}
		}

		if ( hasIbo() )
		{
			glLogCall( gl::BindBuffer
				, GL_BUFFER_TARGET_ELEMENT_ARRAY
				, static_cast< Buffer const & >( getIbo().buffer ).getBuffer() );
		}

		glLogCall( gl::BindVertexArray, 0u );
	}
}
