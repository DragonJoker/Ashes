#include "QueryControlFlag.hpp"

namespace renderer
{
	VkQueryControlFlags convert( QueryControlFlags const & flags )
	{
		VkQueryControlFlags result{ 0 };

		if ( checkFlag( flags, QueryControlFlag::ePrecise ) )
		{
			result |= VK_QUERY_CONTROL_PRECISE_BIT;
		}

		return result;
	}
}
