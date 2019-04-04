/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_AndroidWindowHandle_HPP___
#define ___Ashes_AndroidWindowHandle_HPP___

#include <native_window.h>

namespace ashes
{
	class IAndroidWindowHandle
		: public IWindowHandle
	{
	public:
		IAndroidWindowHandle( ANativeWindow * window )
			: m_window{ window }
		{
		}

		virtual ~IAndroidWindowHandle()
		{
		}

		virtual operator bool()
		{
			return m_window != nullptr;
		}

		inline ANativeWindow * getWindow()const
		{
			return m_window;
		}

	private:
		ANativeWindow * m_window;
	};
}

#endif
