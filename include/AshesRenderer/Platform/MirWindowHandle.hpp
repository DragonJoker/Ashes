/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_MirWindowHandle_HPP___
#define ___Ashes_MirWindowHandle_HPP___

#include <mir_toolkit/mir_connection.h>
#include <mir_toolkit/mir_surface.h>

namespace ashes
{
	class IMirWindowHandle
		: public IWindowHandle
	{
	public:
		IMirWindowHandle( MirConnection * connection
			, MirSurface * surface )
			: m_connection{ connection }
			, m_surface{ surface }
		{
		}

		virtual ~IMirWindowHandle()
		{
		}

		virtual operator bool()
		{
			return m_connection != nullptr && m_surface != nullptr;
		}

		inline MirConnection * getConnection()const
		{
			return m_connection;
		}

		inline MirSurface * getSurface()const
		{
			return m_surface;
		}

	private:
		MirConnection * m_connection;
		MirSurface * m_surface;
	};
}

#endif
