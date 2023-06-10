/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class BeginRenderPassCommand
		: public CommandBase
	{
	public:
		BeginRenderPassCommand( VkDevice device
			, VkRenderPass renderPass
			, VkFramebuffer frameBuffer
			, VkClearValueArray const & clearValues );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
