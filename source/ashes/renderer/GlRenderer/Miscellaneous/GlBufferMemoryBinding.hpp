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
		~BufferMemoryBinding();

	private:
		Buffer * m_buffer;
		mutable GLenum m_copyTarget;
	};
}
