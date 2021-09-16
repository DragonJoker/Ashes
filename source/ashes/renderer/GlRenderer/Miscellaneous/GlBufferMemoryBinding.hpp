/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Miscellaneous/GlDeviceMemoryBinding.hpp"

namespace ashes::gl
{
	class BufferMemoryBinding
		: public DeviceMemoryBinding
	{
	public:
		BufferMemoryBinding( VkDeviceMemory parent
			, VkDevice device
			, VkBuffer buffer
			, VkDeviceSize memoryOffset );
		~BufferMemoryBinding()override;

	private:
		Buffer * m_buffer;
	};
}
