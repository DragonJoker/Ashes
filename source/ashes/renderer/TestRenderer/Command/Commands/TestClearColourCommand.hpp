/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class ClearColourCommand
		: public CommandBase
	{
	public:
		ClearColourCommand( VkDevice device
			, VkImage image
			, VkImageSubresourceRangeArray ranges
			, VkClearColorValue const & colour );

		void apply()const;
		CommandPtr clone()const;
	};
}
