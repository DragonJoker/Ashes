/*
See LICENSE file in root folder
*/
#pragma once

#if defined( _WIN32 )
#include "GlContext.hpp"

#include <Windows.h>

namespace gl_renderer
{
	class MswContext
		: public Context
	{
	public:
		MswContext( renderer::ConnectionPtr && connection );
		~MswContext();

		void setCurrent()const override;
		void endCurrent()const override;
		void swapBuffers()const override;

		inline HDC getHDC()const
		{
			return m_hDC;
		}

		inline HGLRC getContext()const
		{
			return m_hContext;
		}

	private:
		HGLRC doCreateDummyContext();
		bool doSelectPixelFormat();
		bool doCreateGl3Context();

	private:
		HDC m_hDC;
		HGLRC m_hContext;
		HWND m_hWnd;
	};
}

#endif
