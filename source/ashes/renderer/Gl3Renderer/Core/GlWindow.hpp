/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#if defined( VK_USE_PLATFORM_WIN32_KHR )
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	include <Windows.h>
#elif defined( VK_USE_PLATFORM_XLIB_KHR )
#	include <X11/X.h>
#	include <X11/Xlib.h>
#	include <GL/glx.h>
#endif

namespace ashes::gl3
{
#if defined( VK_USE_PLATFORM_WIN32_KHR )

	class RenderWindow
	{
	public:
		RenderWindow();
		~RenderWindow();
		VkWin32SurfaceCreateInfoKHR getCreateInfo()const;

	private:
		bool doSelectFormat();
		static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	private:
		HWND m_hWnd{ nullptr };
		HDC m_hDC{ nullptr };
		HGLRC m_hContext{ nullptr };
		HINSTANCE m_hInstance{ nullptr };
	};

#elif defined( VK_USE_PLATFORM_XLIB_KHR )

	class RenderWindow
	{
	public:
		RenderWindow();
		~RenderWindow();
		VkXlibSurfaceCreateInfoKHR getCreateInfo()const;

	private:
		Colormap m_map{ 0 };
		Display * m_display{ nullptr };
		Window m_xWindow{ 0 };
		GLXFBConfig m_fbConfig{ nullptr };
		GLXContext m_glxContext;
	};

#endif
}
