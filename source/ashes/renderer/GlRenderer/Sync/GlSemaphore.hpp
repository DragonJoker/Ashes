/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class Semaphore
		: public AutoIdIcdObject< Semaphore >
	{
	public:
		Semaphore( VkAllocationCallbacks const * allocInfo
			, VkDevice device );
		~Semaphore();

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
	};
}
