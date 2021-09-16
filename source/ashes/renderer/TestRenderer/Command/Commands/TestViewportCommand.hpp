/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class ViewportCommand
		: public CommandBase
	{
	public:
		ViewportCommand( VkDevice device
			, uint32_t first
			, VkViewportArray const & viewports );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
