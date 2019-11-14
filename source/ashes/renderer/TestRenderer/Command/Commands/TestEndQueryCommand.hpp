/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class EndQueryCommand
		: public CommandBase
	{
	public:
		EndQueryCommand( VkDevice device
			, VkQueryPool pool
			, uint32_t query );

		void apply()const;
		CommandPtr clone()const;
	};
}
