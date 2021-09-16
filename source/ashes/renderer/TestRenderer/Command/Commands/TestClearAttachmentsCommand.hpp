/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class ClearAttachmentsCommand
		: public CommandBase
	{
	public:
		ClearAttachmentsCommand( VkDevice device
			, VkRenderPass renderPass
			, VkSubpassDescription const & subpass
			, VkFramebuffer framebuffer
			, VkClearAttachmentArray const & clearAttaches
			, VkClearRectArray const & clearRects );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
