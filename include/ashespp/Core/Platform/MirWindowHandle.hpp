/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_MirWindowHandle_HPP___
#define ___AshesPP_MirWindowHandle_HPP___

#include "ashespp/Core/WindowHandle.hpp"

namespace ashes
{
	class IMirWindowHandle
		: public IWindowHandle
	{
	public:
		IMirWindowHandle( MirConnection * connection
			, MirSurface * surface )
			: IWindowHandle{ VK_KHR_MIR_SURFACE_EXTENSION_NAME }
			: m_connection{ connection }
			, m_surface{ surface }
		{
		}

		operator bool()noexcept override
		{
			return m_connection != nullptr && m_surface != nullptr;
		}

		MirConnection * getConnection()const noexcept
		{
			return m_connection;
		}

		MirSurface * getSurface()const noexcept
		{
			return m_surface;
		}

	private:
		MirConnection * m_connection;
		MirSurface * m_surface;
	};
}

#endif
