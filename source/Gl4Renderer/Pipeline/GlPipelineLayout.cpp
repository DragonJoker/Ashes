#include "Pipeline/GlPipelineLayout.hpp"

#include "Core/GlDevice.hpp"
#include "Descriptor/GlDescriptorSetLayout.hpp"
#include "Pipeline/GlPipeline.hpp"

namespace ashes::gl4
{
	PipelineLayout::PipelineLayout( VkDevice device
		, VkPipelineLayoutCreateInfo createInfo )
		: m_device{ device }
		, m_setLayouts{ createInfo.pSetLayouts, createInfo.pSetLayouts + createInfo.setLayoutCount }
		, m_pushConstantRanges{ createInfo.pPushConstantRanges, createInfo.pPushConstantRanges + createInfo.pushConstantRangeCount }
		, m_createInfo{ createInfo }
	{
		m_createInfo.pSetLayouts = m_setLayouts.data();
		m_createInfo.pPushConstantRanges = m_pushConstantRanges.data();
	}
}
