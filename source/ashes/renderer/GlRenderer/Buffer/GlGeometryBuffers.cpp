/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Buffer/GlGeometryBuffers.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"

#include "ashesgl_api.hpp"

#include <algorithm>

namespace ashes::gl
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

		bool isNormalized( VkFormat format )
		{
			return format == VK_FORMAT_R4G4_UNORM_PACK8
				|| format == VK_FORMAT_R4G4B4A4_UNORM_PACK16
				|| format == VK_FORMAT_B4G4R4A4_UNORM_PACK16
				|| format == VK_FORMAT_R5G6B5_UNORM_PACK16
				|| format == VK_FORMAT_B5G6R5_UNORM_PACK16
				|| format == VK_FORMAT_R5G5B5A1_UNORM_PACK16
				|| format == VK_FORMAT_B5G5R5A1_UNORM_PACK16
				|| format == VK_FORMAT_A1R5G5B5_UNORM_PACK16
				|| format == VK_FORMAT_R8_UNORM
				|| format == VK_FORMAT_R8_SNORM
				|| format == VK_FORMAT_R8G8_UNORM
				|| format == VK_FORMAT_R8G8_SNORM
				|| format == VK_FORMAT_R8G8B8_UNORM
				|| format == VK_FORMAT_R8G8B8_SNORM
				|| format == VK_FORMAT_B8G8R8_UNORM
				|| format == VK_FORMAT_B8G8R8_SNORM
				|| format == VK_FORMAT_R8G8B8A8_UNORM
				|| format == VK_FORMAT_R8G8B8A8_SNORM
				|| format == VK_FORMAT_B8G8R8A8_UNORM
				|| format == VK_FORMAT_B8G8R8A8_SNORM
				|| format == VK_FORMAT_A8B8G8R8_UNORM_PACK32
				|| format == VK_FORMAT_A8B8G8R8_SNORM_PACK32
				|| format == VK_FORMAT_A2R10G10B10_UNORM_PACK32
				|| format == VK_FORMAT_A2R10G10B10_SNORM_PACK32
				|| format == VK_FORMAT_A2B10G10R10_UNORM_PACK32
				|| format == VK_FORMAT_A2B10G10R10_SNORM_PACK32
				|| format == VK_FORMAT_R16_UNORM
				|| format == VK_FORMAT_R16_SNORM
				|| format == VK_FORMAT_R16G16_UNORM
				|| format == VK_FORMAT_R16G16_SNORM
				|| format == VK_FORMAT_R16G16B16_UNORM
				|| format == VK_FORMAT_R16G16B16_SNORM
				|| format == VK_FORMAT_R16G16B16A16_UNORM
				|| format == VK_FORMAT_R16G16B16A16_SNORM
				|| format == VK_FORMAT_D16_UNORM
				|| format == VK_FORMAT_X8_D24_UNORM_PACK32
				|| format == VK_FORMAT_D16_UNORM_S8_UINT
				|| format == VK_FORMAT_D24_UNORM_S8_UINT
				|| format == VK_FORMAT_BC1_RGB_UNORM_BLOCK
				|| format == VK_FORMAT_BC1_RGBA_UNORM_BLOCK
				|| format == VK_FORMAT_BC2_UNORM_BLOCK
				|| format == VK_FORMAT_BC3_UNORM_BLOCK
				|| format == VK_FORMAT_BC4_UNORM_BLOCK
				|| format == VK_FORMAT_BC4_SNORM_BLOCK
				|| format == VK_FORMAT_BC5_UNORM_BLOCK
				|| format == VK_FORMAT_BC5_SNORM_BLOCK
				|| format == VK_FORMAT_BC7_UNORM_BLOCK
				|| format == VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK
				|| format == VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK
				|| format == VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK
				|| format == VK_FORMAT_EAC_R11_UNORM_BLOCK
				|| format == VK_FORMAT_EAC_R11_SNORM_BLOCK
				|| format == VK_FORMAT_EAC_R11G11_UNORM_BLOCK
				|| format == VK_FORMAT_EAC_R11G11_SNORM_BLOCK
				|| format == VK_FORMAT_ASTC_4x4_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_5x4_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_5x5_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_6x5_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_6x6_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_8x5_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_8x6_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_8x8_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_10x5_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_10x6_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_10x8_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_10x10_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_12x10_UNORM_BLOCK
				|| format == VK_FORMAT_ASTC_12x12_UNORM_BLOCK
#ifdef VK_API_VERSION_1_1
				|| format == VK_FORMAT_G8B8G8R8_422_UNORM
				|| format == VK_FORMAT_B8G8R8G8_422_UNORM
				|| format == VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM
				|| format == VK_FORMAT_G8_B8R8_2PLANE_420_UNORM
				|| format == VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM
				|| format == VK_FORMAT_G8_B8R8_2PLANE_422_UNORM
				|| format == VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM
				|| format == VK_FORMAT_R10X6_UNORM_PACK16
				|| format == VK_FORMAT_R10X6G10X6_UNORM_2PACK16
				|| format == VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16
				|| format == VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16
				|| format == VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16
				|| format == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16
				|| format == VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16
				|| format == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16
				|| format == VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16
				|| format == VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16
				|| format == VK_FORMAT_R12X4_UNORM_PACK16
				|| format == VK_FORMAT_R12X4G12X4_UNORM_2PACK16
				|| format == VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16
				|| format == VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16
				|| format == VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16
				|| format == VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16
				|| format == VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16
				|| format == VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16
				|| format == VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16
				|| format == VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16
				|| format == VK_FORMAT_G16B16G16R16_422_UNORM
				|| format == VK_FORMAT_B16G16R16G16_422_UNORM
				|| format == VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM
				|| format == VK_FORMAT_G16_B16R16_2PLANE_420_UNORM
				|| format == VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM
				|| format == VK_FORMAT_G16_B16R16_2PLANE_422_UNORM
				|| format == VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM
				|| format == VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG
				|| format == VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG
				|| format == VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG
				|| format == VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG
#endif
			;
		}

		bool isInteger( VkFormat format )
		{
			return format != VK_FORMAT_R8G8B8A8_UNORM
				&& format != VK_FORMAT_R16_SFLOAT
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
		, InputsLayout const & inputLayout
		, VkIndexType type )
		: m_device{ device }
		, m_vbos{ createVBOs( vbos, vertexInputState, inputLayout ) }
		, m_ibo{ bool( ibo ) ? std::make_unique< IBO >( ibo.value().buffer, ibo.value().offset, type ) : nullptr }
	{
	}

	GeometryBuffers::~GeometryBuffers()noexcept
	{
		if ( m_vao != GL_INVALID_INDEX )
		{
			auto context = get( m_device )->getContext();
			glLogCall( context
				, glDeleteVertexArrays
				, 1
				, &m_vao );
		}
	}

	void GeometryBuffers::enableAttribute( ContextLock & context
		, VkVertexInputBindingDescription const & binding
		, VkVertexInputAttributeDescription const & attribute
		, VkDeviceSize offset
		, VkVertexInputAttributeDescription const * programAttribute )
	{
		auto pAttribute = programAttribute ? programAttribute : &attribute;
		glLogCall( context
			, glEnableVertexAttribArray
			, attribute.location );

		if ( isInteger( pAttribute->format ) )
		{
			glLogCall( context
				, glVertexAttribIPointer
				, attribute.location
				, ashes::getCount( attribute.format )
				, getType( getInternalFormat( attribute.format ) )
				, binding.stride
				, getBufferOffset( offset + attribute.offset ) );
		}
		else
		{
			glLogCall( context
				, glVertexAttribPointer
				, attribute.location
				, ashes::getCount( attribute.format )
				, getType( getInternalFormat( attribute.format ) )
				, isNormalized( attribute.format ) ? GL_TRUE : GL_FALSE
				, binding.stride
				, getBufferOffset( offset + attribute.offset ) );
		}
	}

	void GeometryBuffers::initialise( ContextLock & context )
	{
		glLogCall( context
			, glGenVertexArrays
			, 1
			, &m_vao );

		if ( m_vao == GL_INVALID_INDEX )
		{
			get( m_device )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT
				, uint64_t( get( m_device ) )
				, 0u
				, VK_ERROR_INCOMPATIBLE_DRIVER
				, "OpenGL"
				, "Couldn't create VAO" );
			return;
		}

		glLogCall( context
			, glBindVertexArray
			, m_vao );

		for ( auto & vbo : m_vbos )
		{
			auto findAttribute = [&vbo]( uint32_t location )
			{
				return std::find_if( vbo.programAttributes.begin()
					, vbo.programAttributes.end()
					, [location]( VkVertexInputAttributeDescription const & lookup )
					{
						return lookup.location == location;
					} );
			};

			auto bufferOffset = get( vbo.vbo )->getInternalOffset();
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_ARRAY
				, get( vbo.vbo )->getInternal() );

			if ( vbo.binding.inputRate == VK_VERTEX_INPUT_RATE_VERTEX )
			{
				for ( auto & attribute : vbo.attributes )
				{
					auto it = findAttribute( attribute.location );

					if ( it == vbo.programAttributes.end() )
					{
						enableAttribute( context, vbo.binding, attribute, bufferOffset + vbo.offset, nullptr );
					}
					else
					{
						enableAttribute( context, vbo.binding, attribute, bufferOffset + vbo.offset, &( *it ) );
					}
				}
			}
			else
			{
				for ( auto & attribute : vbo.attributes )
				{
					auto it = findAttribute( attribute.location );

					if ( it == vbo.programAttributes.end() )
					{
						enableAttribute( context, vbo.binding, attribute, bufferOffset + vbo.offset, nullptr );
					}
					else
					{
						enableAttribute( context, vbo.binding, attribute, bufferOffset + vbo.offset, &( *it ) );
					}

					glLogCall( context
						, glVertexAttribDivisor
						, attribute.location
						, 1u );
				}
			}
		}

		if ( m_ibo )
		{
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_ELEMENT_ARRAY
				, get( m_ibo->ibo )->getInternal() );
		}

		glLogCall( context
			, glBindVertexArray
			, 0u );

		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_ELEMENT_ARRAY
			, 0 );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_ARRAY
			, 0 );
	}

	std::vector< GeometryBuffers::VBO > GeometryBuffers::createVBOs( VboBindings const & vbos
		, VkPipelineVertexInputStateCreateInfo const & vertexInputState
		, InputsLayout const & inputLayout )
	{
		std::vector< GeometryBuffers::VBO > result;
		result.reserve( vbos.size() );

		for ( auto & binding : vbos )
		{
			auto it = std::find_if( vertexInputState.pVertexBindingDescriptions
				, vertexInputState.pVertexBindingDescriptions + vertexInputState.vertexBindingDescriptionCount
				, [&binding]( VkVertexInputBindingDescription const & lookup )
			{
				return lookup.binding == binding.first;
			} );

			if ( it != vertexInputState.pVertexBindingDescriptions + vertexInputState.vertexBindingDescriptionCount )
			{
				auto & vbo = binding.second;
				result.emplace_back( vbo.buffer
					, vbo.offset
					, *it
					, getAttributes( vertexInputState.pVertexAttributeDescriptions
						, vertexInputState.pVertexAttributeDescriptions + vertexInputState.vertexAttributeDescriptionCount
						, it->binding )
					, getAttributes( inputLayout.vertexAttributeDescriptions.data()
						, inputLayout.vertexAttributeDescriptions.data() + inputLayout.vertexAttributeDescriptions.size()
						, it->binding ) );
			}
		}

		return result;
	}
}
