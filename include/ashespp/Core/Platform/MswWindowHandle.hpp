/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_MswWindowHandle_HPP___
#define ___AshesPP_MswWindowHandle_HPP___

#include "ashespp/Core/WindowHandle.hpp"

namespace ashes
{
	class IMswWindowHandle
		: public IWindowHandle
	{
	public:
		explicit IMswWindowHandle( HINSTANCE hInstance
			, HWND hWnd )
			: IWindowHandle{ VK_KHR_WIN32_SURFACE_EXTENSION_NAME }
			, m_hInstance{ hInstance }
			, m_hWnd{ hWnd }
		{
		}

		operator bool()noexcept override
		{
			return m_hWnd != nullptr
				&& m_hInstance != nullptr;
		}

		HINSTANCE getHinstance()const noexcept
		{
			return m_hInstance;
		}

		HWND getHwnd()const noexcept
		{
			return m_hWnd;
		}

	private:
		HINSTANCE m_hInstance;
		HWND m_hWnd;
	};
}

#endif
