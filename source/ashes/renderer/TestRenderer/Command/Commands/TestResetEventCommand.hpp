/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class ResetEventCommand
		: public CommandBase
	{
	public:
		ResetEventCommand( VkDevice device
			, VkEvent event
			, VkPipelineStageFlags stageFlags );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
