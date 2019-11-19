/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_XWindowHandle_HPP___
#define ___AshesPP_XWindowHandle_HPP___

#include "ashespp/Core/WindowHandle.hpp"

namespace ashes
{
	class IXWindowHandle
		: public IWindowHandle
	{
	public:
		IXWindowHandle( Window drawable
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

		inline Window getDrawable()const
		{
			return m_drawable;
		}

		inline Display * getDisplay()const
		{
			return m_display;
		}

	private:
		Window m_drawable;
		Display * m_display;
	};
}

#endif
