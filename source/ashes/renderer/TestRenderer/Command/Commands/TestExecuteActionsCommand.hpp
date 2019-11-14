/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class ExecuteActionsCommand
		: public CommandBase
	{
	public:
		ExecuteActionsCommand( VkDevice device
			, ActionArray actions );

		void apply()const;
		CommandPtr clone()const;

	private:
		ActionArray m_actions;
	};
}
