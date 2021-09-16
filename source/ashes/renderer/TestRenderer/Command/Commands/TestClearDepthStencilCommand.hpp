/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class ClearDepthStencilCommand
		: public CommandBase
	{
	public:
		ClearDepthStencilCommand( VkDevice device
			, VkImage image
			, VkImageSubresourceRangeArray ranges
			, VkClearDepthStencilValue value );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
