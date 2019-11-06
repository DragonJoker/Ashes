/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestBlitImageCommand.hpp"

namespace ashes::test
{
	BlitImageCommand::BlitImageCommand( VkCommandPool pool
		, VkCommandBuffer cb
		, VkDevice device
		, VkImage srcImage
		, VkImage dstImage
		, VkImageBlitArray const & regions
		, VkFilter filter )
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
