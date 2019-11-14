/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_MswWindowHandle_HPP___
#define ___AshesPP_MswWindowHandle_HPP___

#ifndef NOMINMAX
#	define NOMINMAX
#endif
#include <Windows.h>

#include "ashespp/Core/WindowHandle.hpp"
#include <vulkan/vulkan_win32.h>

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

		virtual ~IMswWindowHandle()
		{
		}

		virtual operator bool()
		{
			return m_hWnd != nullptr
				&& m_hInstance != nullptr;
		}

		inline HINSTANCE getHinstance()const
		{
			return m_hInstance;
		}

		inline HWND getHwnd()const
		{
			return m_hWnd;
		}

	private:
		HINSTANCE m_hInstance;
		HWND m_hWnd;
	};
}

#endif
