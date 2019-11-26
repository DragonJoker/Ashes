/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestClearDepthStencilCommand.hpp"

namespace ashes::test
{
	ClearDepthStencilCommand::ClearDepthStencilCommand( VkDevice device
		, VkImage image
		, VkImageSubresourceRangeArray ranges
		, VkClearDepthStencilValue value )
		: CommandBase{ device }
	{
	}

	void ClearDepthStencilCommand::apply()const
	{
	}

	CommandPtr ClearDepthStencilCommand::clone()const
	{
		return std::make_unique< ClearDepthStencilCommand >( *this );
	}
}
