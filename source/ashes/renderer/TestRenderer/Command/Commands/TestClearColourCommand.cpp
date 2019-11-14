/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestClearColourCommand.hpp"

namespace ashes::test
{
	ClearColourCommand::ClearColourCommand( VkDevice device
		, VkImage image
		, VkImageSubresourceRangeArray ranges
		, VkClearColorValue const & colour )
		: CommandBase{ device }
	{
	}

	void ClearColourCommand::apply()const
	{
	}

	CommandPtr ClearColourCommand::clone()const
	{
		return std::make_unique< ClearColourCommand >( *this );
	}
}
