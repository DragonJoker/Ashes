#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkQueryResultFlags convert( ashes::QueryResultFlags const & flags )
	{
		VkQueryResultFlags result{ 0 };

		if ( checkFlag( flags, ashes::QueryResultFlag::e64 ) )
		{
			result |= VK_QUERY_RESULT_64_BIT;
		}

		if ( checkFlag( flags, ashes::QueryResultFlag::eWait ) )
		{
			result |= VK_QUERY_RESULT_WAIT_BIT;
		}

		if ( checkFlag( flags, ashes::QueryResultFlag::eWithAvailability ) )
		{
			result |= VK_QUERY_RESULT_WITH_AVAILABILITY_BIT;
		}

		if ( checkFlag( flags, ashes::QueryResultFlag::ePartial ) )
		{
			result |= VK_QUERY_RESULT_PARTIAL_BIT;
		}

		return result;
	}
}
