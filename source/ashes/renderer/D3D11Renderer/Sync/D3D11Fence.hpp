/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class Fence
	{
	public:
		Fence( VkDevice device
			, VkFenceCreateFlags flags = 0 );
		~Fence();

		VkResult wait( uint64_t timeout )const;
		void reset()const;
		VkResult getStatus()const;

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
	};
}
