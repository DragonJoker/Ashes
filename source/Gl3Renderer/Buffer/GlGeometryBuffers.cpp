/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "Buffer/GlGeometryBuffers.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"

#include <Ashes/Buffer/VertexBuffer.hpp>

#include <algorithm>

namespace gl_renderer
{
	namespace
	{
		ashes::VertexInputAttributeDescriptionArray getAttributes( ashes::VertexInputAttributeDescriptionArray const & attributes
			, uint32_t bindingSlot )
		{
			ashes::VertexInputAttributeDescriptionArray result;

			for ( auto & attribute : attributes )
			{
				if ( attribute.binding == bindingSlot )
				{
					result.push_back( attribute );
				}
			}

			return result;
		}

		bool isInteger( ashes::Format format )
		{
			return format != ashes::Format::eR16_SFLOAT
				&& format != ashes::Format::eR16G16_SFLOAT
				&& format != ashes::Format::eR16G16B16_SFLOAT
				&& format != ashes::Format::eR16G16B16A16_SFLOAT
				&& format != ashes::Format::eR32_SFLOAT
				&& format != ashes::Format::eR32G32_SFLOAT
				&& format != ashes::Format::eR32G32B32_SFLOAT
				&& format != ashes::Format::eR32G32B32A32_SFLOAT
				&& format != ashes::Format::eR64_SFLOAT
				&& format != ashes::Format::eR64G64_SFLOAT
				&& format != ashes::Format::eR64G64B64_SFLOAT
				&& format != ashes::Format::eR64G64B64A64_SFLOAT
				&& format != ashes::Format::eB10G11R11_UFLOAT_PACK32
				&& format != ashes::Format::eE5B9G9R9_UFLOAT_PACK32
				&& format != ashes::Format::eD32_SFLOAT
				&& format != ashes::Format::eD32_SFLOAT_S8_UINT
				&& format != ashes::Format::eBC6H_UFLOAT_BLOCK
				&& format != ashes::Format::eBC6H_SFLOAT_BLOCK;
		}
	}

	GeometryBuffers::GeometryBuffers( Device const & device
		, VboBindings const & vbos
		, IboBinding const & ibo
		, ashes::VertexInputState const & vertexInputState
		, ashes::IndexType type )
		: m_device{ device }
		, m_vbos{ createVBOs( vbos, vertexInputState ) }
		, m_ibo{ bool( ibo ) ? std::make_unique< IBO >( ibo.value().bo, ibo.value().offset, type ) : nullptr }
	{
	}

	GeometryBuffers::~GeometryBuffers()noexcept
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glDeleteVertexArrays
			, 1
			, &m_vao );
	}

	void GeometryBuffers::initialise()
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glGenVertexArrays
			, 1
			, &m_vao );

		if ( m_vao == GL_INVALID_INDEX )
		{
			throw std::runtime_error{ "Couldn't create VAO" };
		}

		glLogCall( context
			, glBindVertexArray
			, m_vao );

		for ( auto & vbo : m_vbos )
		{
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_ARRAY
				, vbo.vbo );

			if ( vbo.binding.inputRate == ashes::VertexInputRate::eVertex )
			{
				for ( auto & attribute : vbo.attributes )
				{
					glLogCall( context
						, glEnableVertexAttribArray
						, attribute.location );

					if ( isInteger( attribute.format ) )
					{
						glLogCall( context
							, glVertexAttribIPointer
							, attribute.location
							, getCount( attribute.format )
							, getType( getInternal( attribute.format ) )
							, vbo.binding.stride
							, BufferOffset( vbo.offset + attribute.offset ) );
					}
					else
					{
						glLogCall( context
							, glVertexAttribPointer
							, attribute.location
							, getCount( attribute.format )
							, getType( getInternal( attribute.format ) )
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

					glLogCall( context
						, glEnableVertexAttribArray
						, location );

					if ( isInteger( attribute.format ) )
					{
						glLogCall( context
							, glVertexAttribIPointer
							, location
							, getCount( format )
							, getType( getInternal( attribute.format ) )
							, vbo.binding.stride
							, BufferOffset( vbo.offset + offset ) );
					}
					else
					{
						glLogCall( context
							, glVertexAttribPointer
							, location
							, getCount( format )
							, getType( getInternal( attribute.format ) )
							, false
							, vbo.binding.stride
							, BufferOffset( vbo.offset + offset ) );
					}

					glLogCall( context
						, glVertexAttribDivisor
						, location
						, divisor );
				}
			}
		}

		if ( m_ibo )
		{
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_ELEMENT_ARRAY
				, m_ibo->ibo );
		}

		glLogCall( context
			, glBindVertexArray
			, 0u );
	}

	std::vector< GeometryBuffers::VBO > GeometryBuffers::createVBOs( VboBindings const & vbos
		, ashes::VertexInputState const & vertexInputState )
	{
		std::vector< GeometryBuffers::VBO > result;
		assert( vbos.size() == vertexInputState.vertexBindingDescriptions.size() );
		result.reserve( vbos.size() );

		for ( auto & binding : vbos )
		{
			auto it = std::find_if( vertexInputState.vertexBindingDescriptions.begin()
				, vertexInputState.vertexBindingDescriptions.end()
				, [&binding]( ashes::VertexInputBindingDescription const & lookup )
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
