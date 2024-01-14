/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestBlitImageCommand.hpp"

namespace ashes::test
{
	BlitImageCommand::BlitImageCommand( VkCommandPool
		, VkCommandBuffer
		, VkDevice device
		, VkImage
		, VkImage
		, VkImageBlitArray const &
		, VkFilter )
		: CommandBase{ device }
	{
	}

	void BlitImageCommand::apply()const
	{
	}

	CommandPtr BlitImageCommand::clone()const
	{
		return std::make_unique< BlitImageCommand >( *this );
	}
}
