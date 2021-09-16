/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class BindComputePipelineCommand
		: public CommandBase
	{
	public:
		BindComputePipelineCommand( VkDevice device
			, VkPipeline pipeline
			, VkPipelineBindPoint bindingPoint );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		VkPipeline m_pipeline;
		VkPipelineLayout m_layout;
	};
}
