/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class ResetQueryPoolCommand
		: public CommandBase
	{
	public:
		ResetQueryPoolCommand( VkDevice device
			, VkQueryPool pool
			, uint32_t firstQuery
			, uint32_t queryCount );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
