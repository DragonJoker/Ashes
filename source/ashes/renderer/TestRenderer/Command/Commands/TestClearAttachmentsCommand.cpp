/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestClearAttachmentsCommand.hpp"

namespace ashes::test
{
	ClearAttachmentsCommand::ClearAttachmentsCommand( VkDevice device
		, VkRenderPass renderPass
		, VkSubpassDescription const & subpass
		, VkFramebuffer framebuffer
		, VkClearAttachmentArray const & clearAttaches
		, VkClearRectArray const & clearRects )
		: CommandBase{ device }
	{
	}

	void ClearAttachmentsCommand::apply()const
	{
	}

	CommandPtr ClearAttachmentsCommand::clone()const
	{
		return std::make_unique< ClearAttachmentsCommand >( *this );
	}
}
