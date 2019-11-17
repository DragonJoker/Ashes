/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#if _WIN32

#include <ashes/ashes.hpp>
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#include <Windows.h>

namespace ashes::gl
{
	class RenderWindow
	{
	private:
		RenderWindow( int major, int minor );
		~RenderWindow();
	public:
		VkWin32SurfaceCreateInfoKHR getCreateInfo()const;
		static void create( int major, int minor );
		static void destroy();
		static RenderWindow const & get();

	private:
		void doCleanup();
		bool doSelectFormat();
		static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	private:
		HWND m_hWnd{ nullptr };
		HDC m_hDC{ nullptr };
		HGLRC m_hContext{ nullptr };
		HINSTANCE m_hInstance{ nullptr };
	};
}

#endif
