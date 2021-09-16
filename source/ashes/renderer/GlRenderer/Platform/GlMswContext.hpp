/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Core/GlContextImpl.hpp"
#include "renderer/GlRenderer/Platform/GlMswWindow.hpp"

#if _WIN32

#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	include <Windows.h>
#	include <gl/GL.h>

namespace ashes::gl
{
	class MswContext
		: public ContextImpl
	{
	public:
		MswContext( VkInstance instance
			, VkWin32SurfaceCreateInfoKHR createInfo
			, ContextImpl const * mainContext );
		MswContext( VkInstance instance
			, VkDisplaySurfaceCreateInfoKHR createInfo
			, ContextImpl const * mainContext );
		~MswContext()noexcept override;

		void preInitialise( int major, int minor )override;
		void postInitialise()override;
		void enable()const override;
		void disable()const override;
		void swapBuffers()const override;
		VkExtent2D getExtent()const override;

	private:
		void doSelectFormat();
		void doLoadSystemFunctions();
		void doSetFullscreen();
		void doCreateModernContext();

	private:
		VkWin32SurfaceCreateInfoKHR win32CreateInfo{};
		VkDisplaySurfaceCreateInfoKHR displayCreateInfo{};
		VkWin32PixelFormatDescriptorASH const * m_pfd;
		int m_minor{ 0 };
		int m_major{ 0 };
		HWND m_hWnd{ nullptr };
		HDC m_hDC{ nullptr };
		HGLRC m_hContext{ nullptr };
		MswContext const * m_mainContext;

		using PFN_wglCreateContextAttribsARB = HGLRC( * )( HDC hDC, HGLRC hShareContext, int const * attribList );
		using PFN_wglSwapIntervalEXT = BOOL( * )( int );
		PFN_wglCreateContextAttribsARB wglCreateContextAttribsARB;
		PFN_wglSwapIntervalEXT wglSwapIntervalEXT;
	};
}

#endif
