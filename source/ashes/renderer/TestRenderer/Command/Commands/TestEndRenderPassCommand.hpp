/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class EndRenderPassCommand
		: public CommandBase
	{
	public:
		EndRenderPassCommand( VkDevice device
			, VkRenderPass renderPass
			, VkFramebuffer frameBuffer );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
