/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::D3D11_NAMESPACE
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
