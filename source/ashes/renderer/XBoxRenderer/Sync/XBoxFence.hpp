/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	class Fence
		: public NonCopyable
	{
	public:
		explicit Fence( VkDevice device )noexcept;

		VkResult wait()const noexcept;
		void reset()const noexcept;
		VkResult getStatus()const noexcept;

		VkDevice getDevice()const noexcept
		{
			return m_device;
		}

	private:
		VkDevice m_device;
	};
}
