/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_XcbWindowHandle_HPP___
#define ___AshesPP_XcbWindowHandle_HPP___

#include <xcb/xcb.h>

#include "ashespp/Core/WindowHandle.hpp"
#include <vulkan/vulkan_xcb.h>

namespace ashes
{
	class IXcbWindowHandle
		: public IWindowHandle
	{
	public:
		IXcbWindowHandle( xcb_connection_t * connection
			, xcb_window_t window )
			: IWindowHandle{ VK_KHR_XCB_SURFACE_EXTENSION_NAME }
			, m_connection{ connection }
			, m_window{ window }
		{
		}

		virtual ~IXcbWindowHandle()
		{
		}

		virtual operator bool()
		{
			return m_connection != nullptr && m_window != 0;
		}

		inline xcb_connection_t * getConnection()const
		{
			return m_connection;
		}

		inline xcb_window_t getWindow()const
		{
			return m_window;
		}

	private:
		xcb_connection_t * m_connection;
		xcb_window_t m_window;
	};
}

#endif
