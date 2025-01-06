#include "Pipeline/XBoxPipelineCache.hpp"

namespace ashes::xbox
{
	PipelineCache::PipelineCache( VkDevice device
		, VkPipelineCacheCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
	}

	VkResult PipelineCache::merge()const
	{
		return VK_SUCCESS;
	}
}
