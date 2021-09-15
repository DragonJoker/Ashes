/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	/**
	*\brief
	*	Wrapper de TestPipelineLayout.
	*/
	class PipelineLayout
	{
	public:
		PipelineLayout( VkDevice device
			, VkPipelineLayoutCreateInfo createInfo );

		inline VkDescriptorSetLayoutArray const & getDescriptorsLayouts()const
		{
			return m_setLayouts;
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkDescriptorSetLayoutArray m_setLayouts;
		VkPushConstantRangeArray m_pushConstantRanges;
		VkPipelineLayoutCreateInfo m_createInfo;
	};
}
