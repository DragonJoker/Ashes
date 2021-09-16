/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class CopyImageToBufferCommand
		: public CommandBase
	{
	public:
		CopyImageToBufferCommand( VkDevice device
			, VkBufferImageCopyArray const & copyInfo
			, VkImage src
			, VkBuffer dst );

		void apply()const override;
		CommandPtr clone()const override;
	};
}
