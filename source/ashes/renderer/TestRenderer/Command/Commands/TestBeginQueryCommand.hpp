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
		explicit BeginQueryCommand( VkDevice device );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
