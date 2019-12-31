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
	public:
		RenderWindow( int major
			, int minor
			, std::string const & name );
		~RenderWindow();
		VkWin32SurfaceCreateInfoKHR getCreateInfo()const;

	private:
		void doCleanup();

	private:
		HINSTANCE m_hInstance{ nullptr };
		ATOM m_class{ 0u };
		HWND m_hWnd{ nullptr };
		HDC m_hDC{ nullptr };
		HGLRC m_hContext{ nullptr };
	};
}

#endif
