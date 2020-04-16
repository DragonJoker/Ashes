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

static constexpr VkStructureType VK_STRUCTURE_TYPE_WIN32_PIXEL_FORMAT_DESCRIPTOR_ASH = VkStructureType( 1000501001 );

struct VkWin32PixelFormatDescriptorASH
{
	VkStructureType sType{ VK_STRUCTURE_TYPE_WIN32_PIXEL_FORMAT_DESCRIPTOR_ASH };
	const void * pNext{ nullptr };
	PIXELFORMATDESCRIPTOR pfd{};
};

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
		VkWin32PixelFormatDescriptorASH m_pfd;
		HINSTANCE m_hInstance{ nullptr };
		ATOM m_class{ 0u };
		HWND m_hWnd{ nullptr };
		HDC m_hDC{ nullptr };
		HGLRC m_hContext{ nullptr };
	};
}

#endif
