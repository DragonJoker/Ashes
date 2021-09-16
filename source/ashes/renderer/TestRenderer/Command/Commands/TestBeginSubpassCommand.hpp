/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class BeginSubpassCommand
		: public CommandBase
	{
	public:
		BeginSubpassCommand( VkDevice device
			, VkRenderPass renderPass
			, VkFramebuffer frameBuffer
			, VkSubpassDescription const & subpass );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
