/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_PlatformWindowHandle_HPP___
#define ___Ashes_PlatformWindowHandle_HPP___

#include "WindowHandle.hpp"

//*************************************************************************************************
//
// INCLUDE THIS FILE ONLY WHERE YOU NEED IT (for instance not in precompiled header file)
//
//*************************************************************************************************

#if VK_USE_PLATFORM_WIN32_KHR
#	include "Platform/MswWindowHandle.hpp"
#elif VK_USE_PLATFORM_XLIB_KHR
#	include "Platform/XWindowHandle.hpp"
#elif VK_USE_PLATFORM_ANDROID_KHR
#	include "Platform/AndroidWindowHandle.hpp"
#elif VK_USE_PLATFORM_XCB_KHR
#	include "Platform/XcbWindowHandle.hpp"
#elif VK_USE_PLATFORM_MIR_KHR
#	include "Platform/MirWindowHandle.hpp"
#elif VK_USE_PLATFORM_WAYLAND_KHR
#	include "Platform/WaylandWindowHandle.hpp"
#else
#	error "Yet unsupported OS"
#endif

namespace ashes
{
	class IWindowlessHandle
		: public IWindowHandle
	{
	public:
		explicit IWindowlessHandle()
			: IWindowHandle{ "windowless" }
		{
		}

		virtual ~IWindowlessHandle()
		{
		}

		virtual operator bool()
		{
			return true;
		}
	};
}

#endif
