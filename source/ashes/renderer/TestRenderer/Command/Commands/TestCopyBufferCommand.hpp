/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class CopyBufferCommand
		: public CommandBase
	{
	public:
		CopyBufferCommand( VkDevice device
			, VkBufferCopy const & copyInfo
			, VkBuffer src
			, VkBuffer dst );

		void apply()const;
		CommandPtr clone()const;
	};
}
