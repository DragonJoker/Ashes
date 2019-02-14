#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkIndexType convert( ashes::IndexType const & type )
	{
		switch ( type )
		{
		case ashes::IndexType::eUInt16:
			return VK_INDEX_TYPE_UINT16;

		case ashes::IndexType::eUInt32:
			return VK_INDEX_TYPE_UINT32;

		default:
			assert( false && "Unsupported index type" );
			return VK_INDEX_TYPE_RANGE_SIZE;
		}
	}
}
