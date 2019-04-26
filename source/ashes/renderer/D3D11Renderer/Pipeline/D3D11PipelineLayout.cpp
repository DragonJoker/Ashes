#include "Pipeline/D3D11PipelineLayout.hpp"

namespace ashes::d3d11
{
	PipelineLayout::PipelineLayout( VkDevice device
		, VkPipelineLayoutCreateInfo createInfo )
		: m_device{ device }
		, m_setLayouts{ createInfo.pSetLayouts, createInfo.pSetLayouts + createInfo.setLayoutCount }
		, m_pushConstantRanges{ createInfo.pPushConstantRanges, createInfo.pPushConstantRanges + createInfo.pushConstantRangeCount }
		, m_createInfo{ std::move( createInfo ) }
	{
		m_createInfo.pSetLayouts = m_setLayouts.data();
		m_createInfo.pPushConstantRanges = m_pushConstantRanges.data();
	}

	PipelineLayout::~PipelineLayout()
	{
	}
}
