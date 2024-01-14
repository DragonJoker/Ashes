/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestClearColourCommand.hpp"

namespace ashes::test
{
	ClearColourCommand::ClearColourCommand( VkDevice device
		, VkImage
		, VkImageSubresourceRangeArray const &
		, VkClearColorValue const & )
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
