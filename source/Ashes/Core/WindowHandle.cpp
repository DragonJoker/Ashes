/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Core/WindowHandle.hpp"

namespace ashes
{
	WindowHandle::WindowHandle( IWindowHandlePtr handle )
		: m_handle{ std::move( handle ) }
	{
	}

	WindowHandle::operator bool()
	{
		return m_handle && m_handle->operator bool();
	}
}
