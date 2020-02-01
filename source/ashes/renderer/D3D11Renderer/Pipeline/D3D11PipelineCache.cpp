#include "Pipeline/D3D11PipelineCache.hpp"

namespace ashes::d3d11
{
	PipelineCache::PipelineCache( VkDevice device
		, VkPipelineCacheCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ createInfo }
	{
	}

	VkResult PipelineCache::merge( ArrayView< VkPipelineCache const > pipelines )
	{
		return VK_SUCCESS;
	}
}
