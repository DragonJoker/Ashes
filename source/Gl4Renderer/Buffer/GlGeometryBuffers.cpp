/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Buffer/GlGeometryBuffers.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"

#include "ashesgl4_api.hpp"

#include <algorithm>

namespace ashes::gl4
{
	namespace
	{
		VkVertexInputAttributeDescriptionArray getAttributes( VkVertexInputAttributeDescription const * begin
			, VkVertexInputAttributeDescription const * end
			, uint32_t bindingSlot )
		{
			VkVertexInputAttributeDescriptionArray result;

			for ( auto it = begin; it != end; ++it )
			{
				if ( it->binding == bindingSlot )
				{
					result.push_back( *it );
				}
			}

			return result;
		}

		bool isInteger( VkFormat format )
		{
			return format != VK_FORMAT_R16_SFLOAT
				&& format != VK_FORMAT_R16G16_SFLOAT
				&& format != VK_FORMAT_R16G16B16_SFLOAT
				&& format != VK_FORMAT_R16G16B16A16_SFLOAT
				&& format != VK_FORMAT_R32_SFLOAT
				&& format != VK_FORMAT_R32G32_SFLOAT
				&& format != VK_FORMAT_R32G32B32_SFLOAT
				&& format != VK_FORMAT_R32G32B32A32_SFLOAT
				&& format != VK_FORMAT_R64_SFLOAT
				&& format != VK_FORMAT_R64G64_SFLOAT
				&& format != VK_FORMAT_R64G64B64_SFLOAT
				&& format != VK_FORMAT_R64G64B64A64_SFLOAT
				&& format != VK_FORMAT_B10G11R11_UFLOAT_PACK32
				&& format != VK_FORMAT_E5B9G9R9_UFLOAT_PACK32
				&& format != VK_FORMAT_D32_SFLOAT
				&& format != VK_FORMAT_D32_SFLOAT_S8_UINT
				&& format != VK_FORMAT_BC6H_UFLOAT_BLOCK
				&& format != VK_FORMAT_BC6H_SFLOAT_BLOCK;
		}
	}

	GeometryBuffers::GeometryBuffers( VkDevice device
		, VboBindings const & vbos
		, IboBinding const & ibo
		, VkPipelineVertexInputStateCreateInfo const & vertexInputState
		, VkIndexType type )
		: m_device{ device }
		, m_vbos{ createVBOs( vbos, vertexInputState ) }
		, m_ibo{ bool( ibo ) ? std::make_unique< IBO >( ibo.value().bo, ibo.value().offset, type ) : nullptr }
	{
	}

	GeometryBuffers::~GeometryBuffers()noexcept
	{
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glDeleteVertexArrays
			, 1
			, &m_vao );
	}

	void GeometryBuffers::initialise()
	{
		auto context = get( m_device )->getContext();
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

			if ( vbo.binding.inputRate == VK_VERTEX_INPUT_RATE_VERTEX )
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
							, ashes::getCount( attribute.format )
							, getType( getInternalFormat( attribute.format ) )
							, vbo.binding.stride
							, BufferOffset( vbo.offset + attribute.offset ) );
					}
					else
					{
						glLogCall( context
							, glVertexAttribPointer
							, attribute.location
							, ashes::getCount( attribute.format )
							, getType( getInternalFormat( attribute.format ) )
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
							, ashes::getCount( format )
							, getType( getInternalFormat( attribute.format ) )
							, vbo.binding.stride
							, BufferOffset( vbo.offset + offset ) );
					}
					else
					{
						glLogCall( context
							, glVertexAttribPointer
							, location
							, ashes::getCount( format )
							, getType( getInternalFormat( attribute.format ) )
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
		, VkPipelineVertexInputStateCreateInfo const & vertexInputState )
	{
		std::vector< GeometryBuffers::VBO > result;
		assert( vbos.size() == vertexInputState.vertexBindingDescriptionCount );
		result.reserve( vbos.size() );

		for ( auto & binding : vbos )
		{
			auto it = std::find_if( vertexInputState.pVertexBindingDescriptions
				, vertexInputState.pVertexBindingDescriptions + vertexInputState.vertexBindingDescriptionCount
				, [&binding]( VkVertexInputBindingDescription const & lookup )
			{
				return lookup.binding == binding.first;
			} );
			assert( it != vertexInputState.pVertexBindingDescriptions + vertexInputState.vertexBindingDescriptionCount );
			auto & vbo = binding.second;
			result.emplace_back( vbo.bo
				, vbo.offset
				, *it
				, getAttributes( vertexInputState.pVertexAttributeDescriptions
					, vertexInputState.pVertexAttributeDescriptions + vertexInputState.vertexAttributeDescriptionCount
					, it->binding ) );
		}

		return result;
	}
}
