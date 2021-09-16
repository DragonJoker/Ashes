/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class CopyBufferToImageCommand
		: public CommandBase
	{
	public:
		CopyBufferToImageCommand( VkDevice device
			, VkBufferImageCopyArray const & copyInfo
			, VkBuffer src
			, VkImage dst );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
