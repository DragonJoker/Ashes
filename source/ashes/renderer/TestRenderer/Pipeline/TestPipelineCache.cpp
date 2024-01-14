#include "Pipeline/TestPipelineCache.hpp"

namespace ashes::test
{
	PipelineCache::PipelineCache( VkDevice device
		, VkPipelineCacheCreateInfo const & )
		: m_device{ device }
	{
	}

	VkResult PipelineCache::merge( VkPipelineCacheArray const & )
	{
		return VK_SUCCESS;
	}
}
