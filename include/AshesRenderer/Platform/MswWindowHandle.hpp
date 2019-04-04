/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_MswWindowHandle_HPP___
#define ___Ashes_MswWindowHandle_HPP___

#include <Windows.h>

namespace ashes
{
	class IMswWindowHandle
		: public IWindowHandle
	{
	public:
		explicit IMswWindowHandle( HINSTANCE hInstance
			, HWND hWnd )
			: m_hInstance{ hInstance }
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
