/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "Buffer/GlGeometryBuffers.hpp"

#include "Buffer/GlBuffer.hpp"

#include <Buffer/VertexBuffer.hpp>

namespace gl_renderer
{
	namespace
	{
		renderer::VertexInputAttributeDescriptionArray getAttributes( renderer::VertexInputAttributeDescriptionArray const & attributes
			, uint32_t bindingSlot )
		{
			renderer::VertexInputAttributeDescriptionArray result;

			for ( auto & attribute : attributes )
			{
				if ( attribute.binding == bindingSlot )
				{
					result.push_back( attribute );
				}
			}

			return result;
		}

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

	GeometryBuffers::GeometryBuffers( VboBindings const & vbos
		, IboBinding const & ibo
		, renderer::VertexInputState const & vertexInputState
		, renderer::IndexType type )
		: m_vbos{ createVBOs( vbos, vertexInputState ) }
		, m_ibo{ bool( ibo ) ? std::make_unique< IBO >( ibo.value().bo, ibo.value().offset, type ) : nullptr }
	{
	}

	GeometryBuffers::~GeometryBuffers()noexcept
	{
		glLogCall( gl::DeleteVertexArrays, 1, &m_vao );
	}

	void GeometryBuffers::initialise()
	{
		glLogCall( gl::GenVertexArrays, 1, &m_vao );

		if ( m_vao == GL_INVALID_INDEX )
		{
			throw std::runtime_error{ "Couldn't create VAO" };
		}

		glLogCall( gl::BindVertexArray, m_vao );

		for ( auto & vbo : m_vbos )
		{
			glLogCall( gl::BindBuffer
				, GL_BUFFER_TARGET_ARRAY
				, vbo.vbo );

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

		if ( m_ibo )
		{
			glLogCall( gl::BindBuffer
				, GL_BUFFER_TARGET_ELEMENT_ARRAY
				, m_ibo->ibo );
		}

		glLogCall( gl::BindVertexArray, 0u );
	}

	std::vector< GeometryBuffers::VBO > GeometryBuffers::createVBOs( VboBindings const & vbos
		, renderer::VertexInputState const & vertexInputState )
	{
		std::vector< GeometryBuffers::VBO > result;
		assert( vbos.size() == vertexInputState.vertexBindingDescriptions.size() );
		result.reserve( vbos.size() );

		for ( auto & binding : vbos )
		{
			auto it = std::find_if( vertexInputState.vertexBindingDescriptions.begin()
				, vertexInputState.vertexBindingDescriptions.end()
				, [&binding]( renderer::VertexInputBindingDescription const & lookup )
			{
				return lookup.binding == binding.first;
			} );
			assert( it != vertexInputState.vertexBindingDescriptions.end() );
			auto & vbo = binding.second;
			result.emplace_back( vbo.bo
				, vbo.offset
				, *it
				, getAttributes( vertexInputState.vertexAttributeDescriptions
					, it->binding ) );
		}

		return result;
	}
}
