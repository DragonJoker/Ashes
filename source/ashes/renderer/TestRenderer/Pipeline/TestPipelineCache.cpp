#include "Pipeline/TestPipelineCache.hpp"

namespace ashes::test
{
	PipelineCache::PipelineCache( VkDevice device
		, VkPipelineCacheCreateInfo createInfo )
		: m_device{ device }
	{
	}

	VkResult PipelineCache::merge( VkPipelineCacheArray pipelines )
	{
		return VK_SUCCESS;
	}
}
