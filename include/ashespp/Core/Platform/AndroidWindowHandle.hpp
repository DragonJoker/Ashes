/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_AndroidWindowHandle_HPP___
#define ___AshesPP_AndroidWindowHandle_HPP___

#include "ashespp/Core/WindowHandle.hpp"

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

		operator bool()noexcept override
		{
			return m_window != nullptr;
		}

		ANativeWindow * getWindow()const noexcept
		{
			return m_window;
		}

	private:
		ANativeWindow * m_window;
	};
}

#endif
