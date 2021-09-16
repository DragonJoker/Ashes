/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class SetDepthBiasCommand
		: public CommandBase
	{
	public:
		SetDepthBiasCommand( VkDevice device
			, float constantFactor
			, float clamp
			, float slopeFactor );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
