/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class BindDescriptorSetCommand
		: public CommandBase
	{
	public:
		BindDescriptorSetCommand( VkDevice device
			, VkDescriptorSet descriptorSet
			, VkPipelineLayout layout
			, UInt32Array const & dynamicOffsets
			, VkPipelineBindPoint bindingPoint );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		UInt32Array m_dynamicOffsets;
	};
}
