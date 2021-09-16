/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class CopyImageCommand
		: public CommandBase
	{
	public:
		CopyImageCommand( VkDevice device
			, VkImageCopy const & copyInfo
			, VkImage src
			, VkImage dst );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
