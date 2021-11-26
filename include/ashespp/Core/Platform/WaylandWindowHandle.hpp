/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_WaylandWindowHandle_HPP___
#define ___AshesPP_WaylandWindowHandle_HPP___

#include "ashespp/Core/WindowHandle.hpp"

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

		operator bool()override
		{
			return m_display != nullptr && m_surface != nullptr;
		}

		wl_display * getDisplay()const
		{
			return m_display;
		}

		wl_surface * getSurface()const
		{
			return m_surface;
		}

	private:
		wl_display * m_display;
		wl_surface * m_surface;
	};
}

#endif
