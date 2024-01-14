/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestClearAttachmentsCommand.hpp"

namespace ashes::test
{
	ClearAttachmentsCommand::ClearAttachmentsCommand( VkDevice device
		, VkRenderPass
		, VkSubpassDescription const &
		, VkFramebuffer
		, VkClearAttachmentArray const &
		, VkClearRectArray const & )
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
