/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_AndroidWindowHandle_HPP___
#define ___AshesPP_AndroidWindowHandle_HPP___

#include "ashespp/Core/WindowHandle.hpp"
#include <vulkan/vulkan_android.h>

namespace ashes
{
	class IAndroidWindowHandle
		: public IWindowHandle
	{
	public:
		IAndroidWindowHandle( ANativeWindow * window )
			: IWindowHandle{ VK_KHR_ANDROID_SURFACE_EXTENSION_NAME }
			, m_window{ window }
		{
		}

		virtual ~IAndroidWindowHandle()
		{
		}

		virtual operator bool()
		{
			return m_window != nullptr;
		}

		inline ANativeWindow * getWindow()const
		{
			return m_window;
		}

	private:
		ANativeWindow * m_window;
	};
}

#endif
