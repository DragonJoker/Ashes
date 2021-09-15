/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"
#include "renderer/TestRenderer/Image/TestImageView.hpp"

namespace ashes::test
{
	class GenerateMipsCommand
		: public CommandBase
	{
	public:
		GenerateMipsCommand( VkDevice device
			, VkImage texture );

		void apply()const;
		CommandPtr clone()const;
	};
}
