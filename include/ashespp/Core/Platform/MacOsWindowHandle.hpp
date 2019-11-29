/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_MacOsWindowHandle_HPP___
#define ___AshesPP_MacOsWindowHandle_HPP___

#include "ashespp/Core/WindowHandle.hpp"

namespace ashes
{
	class IMacOsWindowHandle
		: public IWindowHandle
	{
	public:
		IMacOsWindowHandle( void * view )
			: IWindowHandle{ VK_MVK_MACOS_SURFACE_EXTENSION_NAME }
			, m_view{ view }
		{
		}

		virtual ~IMacOsWindowHandle()
		{
		}

		virtual operator bool()
		{
			return m_view != nullptr;
		}

		inline void * getView()const
		{
			return m_view;
		}

	private:
		void * m_view;
	};
}

#endif
