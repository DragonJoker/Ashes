#include "VkAttributeFormat.hpp"

namespace vk_renderer
{
	VkFormat convert( renderer::AttributeFormat format )
	{
		switch ( format )
		{
		case renderer::AttributeFormat::eFloat:
			return VK_FORMAT_R32_SFLOAT;

		case renderer::AttributeFormat::eVec2f:
			return VK_FORMAT_R32G32_SFLOAT;

		case renderer::AttributeFormat::eVec3f:
			return VK_FORMAT_R32G32B32_SFLOAT;

		case renderer::AttributeFormat::eVec4f:
			return VK_FORMAT_R32G32B32A32_SFLOAT;

		case renderer::AttributeFormat::eInt:
			return VK_FORMAT_R32_SINT;

		case renderer::AttributeFormat::eVec2i:
			return VK_FORMAT_R32G32_SINT;

		case renderer::AttributeFormat::eVec3i:
			return VK_FORMAT_R32G32B32_SINT;

		case renderer::AttributeFormat::eVec4i:
			return VK_FORMAT_R32G32B32A32_SINT;

		default:
			assert( false && "Unsupported attribute format." );
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		}
	}

	uint32_t getSize( renderer::AttributeFormat format )
	{
		switch ( format )
		{
		case renderer::AttributeFormat::eFloat:
		case renderer::AttributeFormat::eInt:
			return 4u;

		case renderer::AttributeFormat::eVec2f:
		case renderer::AttributeFormat::eVec2i:
			return 4u * 2u;

		case renderer::AttributeFormat::eVec3f:
		case renderer::AttributeFormat::eVec3i:
			return 4u * 3u;

		case renderer::AttributeFormat::eVec4f:
		case renderer::AttributeFormat::eVec4i:
			return 4u * 4u;

		default:
			assert( false && "Unsupported attribute format." );
			return 4u * 4u;
		}
	}
}
