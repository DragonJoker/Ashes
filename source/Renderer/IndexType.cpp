#include "IndexType.hpp"

namespace renderer
{
	VkIndexType convert( IndexType const & type )
	{
		switch ( type )
		{
		case IndexType::eUInt16:
			return VK_INDEX_TYPE_UINT16;

		case IndexType::eUInt32:
			return VK_INDEX_TYPE_UINT32;

		default:
			assert( false && "Unsupported index type" );
			return VK_INDEX_TYPE_RANGE_SIZE;
		}
	}
}
