/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
{
	class DownloadMemoryCommand
		: public CommandBase
	{
	public:
		DownloadMemoryCommand( VkDevice device
			, ObjectMemory const * memory
			, VkDeviceSize offset
			, VkDeviceSize size
			, UINT subresource );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		ObjectMemory const * m_memory;
		std::pair< VkDeviceSize, VkDeviceSize > m_range;
		UINT m_subresource;
	};
}
