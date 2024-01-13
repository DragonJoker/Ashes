#include "Pipeline/D3D11PipelineCache.hpp"

namespace ashes::d3d11
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
