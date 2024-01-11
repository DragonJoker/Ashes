/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Core/WindowHandle.hpp"

namespace ashes
{
	WindowHandle::WindowHandle( IWindowHandlePtr handle )noexcept
		: m_handle{ std::move( handle ) }
	{
	}

	WindowHandle::operator bool()noexcept
	{
		return m_handle && m_handle->operator bool();
	}
}
