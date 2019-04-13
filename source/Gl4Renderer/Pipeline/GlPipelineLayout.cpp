#include "Pipeline/GlPipelineLayout.hpp"

#include "Core/GlDevice.hpp"
#include "Descriptor/GlDescriptorSetLayout.hpp"
#include "Pipeline/GlPipeline.hpp"

namespace ashes::gl4
{
	PipelineLayout::PipelineLayout( VkDevice device
		, VkPipelineLayoutCreateInfo createInfo )
		: m_device{ device }
	{
	}
}
