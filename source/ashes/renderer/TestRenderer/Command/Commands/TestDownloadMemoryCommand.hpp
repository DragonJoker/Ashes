/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class DownloadMemoryCommand
		: public CommandBase
	{
	public:
		DownloadMemoryCommand( VkDevice device
			, ObjectMemory const * memory
			, VkDeviceSize offset
			, VkDeviceSize size );
		void apply()const;
		CommandPtr clone()const;

	private:
		std::pair< VkDeviceSize, VkDeviceSize > m_range;
	};
}
