#include "RendererPrerequisites.hpp"

namespace renderer
{
	VkFormat convert( PixelFormat format )
	{
		switch ( format )
		{
		case PixelFormat::eL8:
			return VK_FORMAT_R8_UINT;

		case PixelFormat::eL8A8:
			return VK_FORMAT_R8G8_UINT;

		case PixelFormat::eR8G8B8:
			return VK_FORMAT_R8G8B8_UINT;

		case PixelFormat::eRGB565:
			return VK_FORMAT_R5G6B5_UNORM_PACK16;

		case PixelFormat::eR8G8B8A8:
			return VK_FORMAT_R8G8B8A8_UNORM;

		case PixelFormat::eRGBA5551:
			return VK_FORMAT_R5G5B5A1_UNORM_PACK16;

		case PixelFormat::eRGBA4444:
			return VK_FORMAT_R4G4B4A4_UNORM_PACK16;

		case PixelFormat::eD16:
			return VK_FORMAT_D16_UNORM;

		case PixelFormat::eS8:
			return VK_FORMAT_S8_UINT;
		}

		assert( false && "Unsupported pixel format." );
		return VK_FORMAT_UNDEFINED;
	}

	VkFilter convert( Filter filter )
	{
		switch ( filter )
		{
		case Filter::eNearest:
			return VK_FILTER_NEAREST;

		case Filter::eLinear:
			return VK_FILTER_LINEAR;

		default:
			assert( false );
			return VK_FILTER_NEAREST;
		}
	}

	VkSamplerMipmapMode convert( MipmapMode mode )
	{
		switch ( mode )
		{
		case MipmapMode::eNearest:
			return VK_SAMPLER_MIPMAP_MODE_NEAREST;

		case MipmapMode::eLinear:
			return VK_SAMPLER_MIPMAP_MODE_LINEAR;

		default:
			assert( false );
			return VK_SAMPLER_MIPMAP_MODE_NEAREST;
		}
	}

	VkSamplerAddressMode convert( WrapMode mode )
	{
		switch ( mode )
		{
		case WrapMode::eRepeat:
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;

		case WrapMode::eMirroredRepeat:
			return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;

		case WrapMode::eClampToEdge:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

		case WrapMode::eClampToBorder:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;

		case WrapMode::eMirrorClampToEdge:
			return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;

		default:
			assert( false );
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}
	}

	VkMemoryPropertyFlags convert( MemoryPropertyFlags flags )
	{
		VkMemoryPropertyFlags result{ 0 };

		if ( checkFlag( flags, MemoryPropertyFlag::eDeviceLocal ) )
		{
			result |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		}

		if ( checkFlag( flags, MemoryPropertyFlag::eHostCached ) )
		{
			result |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
		}

		if ( checkFlag( flags, MemoryPropertyFlag::eHostCoherent ) )
		{
			result |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}

		if ( checkFlag( flags, MemoryPropertyFlag::eHostVisible ) )
		{
			result |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		}

		if ( checkFlag( flags, MemoryPropertyFlag::eLazilyAllocated ) )
		{
			result |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
		}

		return result;
	}

	VkBufferUsageFlags convert( BufferTargets targets )
	{
		VkBufferUsageFlags result{ 0 };

		if ( checkFlag( targets, BufferTarget::eIndexBuffer ) )
		{
			result |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eIndirectBuffer ) )
		{
			result |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eStorageBuffer ) )
		{
			result |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eStorageTexelBuffer ) )
		{
			result |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eTransferDst ) )
		{
			result |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eTransferSrc ) )
		{
			result |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eUniformBuffer ) )
		{
			result |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eUniformTexelBuffer ) )
		{
			result |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eVertexBuffer ) )
		{
			result |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}

		return result;
	}

	VkMemoryMapFlags convert( MemoryMapFlags flags )
	{
		return VkMemoryMapFlags{ 0 };
	}

	VkPrimitiveTopology convert( PrimitiveTopology topology )
	{
		switch ( topology )
		{
		case PrimitiveTopology::ePointList:
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;

		case PrimitiveTopology::eLineList:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

		case PrimitiveTopology::eLineStrip:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;

		case PrimitiveTopology::eTriangleList:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		case PrimitiveTopology::eTriangleStrip:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

		case PrimitiveTopology::eTriangleFan:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;

		case PrimitiveTopology::eLineListWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;

		case PrimitiveTopology::eLineStripWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;

		case PrimitiveTopology::eTriangleListWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;

		case PrimitiveTopology::eTriangleStripWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;

		case PrimitiveTopology::ePatchList:
			return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

		default:
			assert( false );
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}
	}

	uint32_t pixelSize( PixelFormat format )noexcept
	{
		switch ( format )
		{
		case PixelFormat::eL8:
		case PixelFormat::eS8:
			return 1;
			break;

		case PixelFormat::eL8A8:
		case PixelFormat::eRGB565:
		case PixelFormat::eRGBA5551:
		case PixelFormat::eRGBA4444:
		case PixelFormat::eD16:
			return 2;
			break;

		case PixelFormat::eR8G8B8:
			return 3;
			break;

		case PixelFormat::eR8G8B8A8:
			return 4;
			break;

		default:
			assert( false && "Unsupported pixel format" );
			return 0;
		}
	}
}
