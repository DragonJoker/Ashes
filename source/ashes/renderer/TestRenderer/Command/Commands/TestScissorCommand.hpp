/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class ScissorCommand
		: public CommandBase
	{
	public:
		ScissorCommand( VkDevice device
			, uint32_t first
			, VkScissorArray const & scissors );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
