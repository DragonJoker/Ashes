/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class BindIndexBufferCommand
		: public CommandBase
	{
	public:
		BindIndexBufferCommand( VkDevice device
			, VkBuffer ibo
			, uint64_t offset
			, VkIndexType indexType );

		void apply()const;
		CommandPtr clone()const;
	};
}
