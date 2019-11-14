/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class BlitImageCommand
		: public CommandBase
	{
	public:
		BlitImageCommand( VkCommandPool pool
			, VkCommandBuffer cb
			, VkDevice device
			, VkImage srcImage
			, VkImage dstImage
			, VkImageBlitArray const & regions
			, VkFilter filter );

		void apply()const;
		CommandPtr clone()const;
	};
}
