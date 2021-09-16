/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class BeginQueryCommand
		: public CommandBase
	{
	public:
		BeginQueryCommand( VkDevice device
			, VkQueryPool pool
			, uint32_t query
			, VkQueryControlFlags flags );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
