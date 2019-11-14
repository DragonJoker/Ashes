/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_XWindowHandle_HPP___
#define ___AshesPP_XWindowHandle_HPP___

#include <GL/glx.h>

#include "ashespp/Core/WindowHandle.hpp"
#include <vulkan/vulkan_xlib.h>

namespace ashes
{
	class IXWindowHandle
		: public IWindowHandle
	{
	public:
		IXWindowHandle( GLXDrawable drawable
			, Display * display )
			: IWindowHandle{ VK_KHR_XLIB_SURFACE_EXTENSION_NAME }
			, m_drawable{ drawable }
			, m_display{ display }
		{
		}

		virtual ~IXWindowHandle()
		{
		}

		virtual operator bool()
		{
			return m_drawable != 0 && m_display != nullptr;
		}

		inline GLXDrawable getDrawable()const
		{
			return m_drawable;
		}

		inline Display * getDisplay()const
		{
			return m_display;
		}

	private:
		GLXDrawable m_drawable;
		Display * m_display;
	};
}

#endif
