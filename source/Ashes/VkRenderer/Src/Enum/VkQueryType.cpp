#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkQueryType convert( ashes::QueryType const & value )
	{
		switch ( value )
		{
		case ashes::QueryType::eOcclusion:
			return VK_QUERY_TYPE_OCCLUSION;

		case ashes::QueryType::eTimestamp:
			return VK_QUERY_TYPE_TIMESTAMP;

		case ashes::QueryType::ePipelineStatistics:
			return VK_QUERY_TYPE_PIPELINE_STATISTICS;

		default:
			assert( false && "Unsupported query type" );
			return VK_QUERY_TYPE_TIMESTAMP;
		}
	}
}
