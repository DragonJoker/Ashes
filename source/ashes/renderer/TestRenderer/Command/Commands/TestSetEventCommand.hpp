/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class SetEventCommand
		: public CommandBase
	{
	public:
		SetEventCommand( VkDevice device
			, VkEvent event
			, VkPipelineStageFlags stageFlags );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		VkEvent m_event;
	};
}
