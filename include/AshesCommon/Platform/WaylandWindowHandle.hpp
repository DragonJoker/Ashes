/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_WaylandWindowHandle_HPP___
#define ___Ashes_WaylandWindowHandle_HPP___

#include <wayland-client-core.h>

#include "AshesCommon/WindowHandle.hpp"
#include <vulkan/vulkan_wayland.h>

namespace ashes
{
	class IWaylandWindowHandle
		: public IWindowHandle
	{
	public:
		IWaylandWindowHandle( wl_display * display
			, wl_surface * surface )
			: IWindowHandle{ VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME }
			, m_display{ display }
			, m_surface{ surface }
		{
		}

		virtual ~IWaylandWindowHandle()
		{
		}

		virtual operator bool()
		{
			return m_display != nullptr && m_surface != nullptr;
		}

		inline wl_display * getDisplay()const
		{
			return m_display;
		}

		inline wl_surface * getSurface()const
		{
			return m_window;
		}

	private:
		wl_display * m_display;
		wl_surface * m_surface;
	};
}

#endif
