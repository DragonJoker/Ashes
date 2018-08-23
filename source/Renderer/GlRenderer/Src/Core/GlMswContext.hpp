/*
See LICENSE file in root folder
*/
#pragma once

#include "Core/GlContext.hpp"

#if RENDERLIB_WIN32
#	include <Windows.h>

namespace gl_renderer
{
	class MswContext
		: public Context
	{
	public:
		MswContext( PhysicalDevice const & gpu
			, renderer::ConnectionPtr && connection );
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

#define WGL_LIB_FUNCTION( fun ) PFN_wgl##fun wgl##fun;
#define WGL_LIB_FUNCTION_OPT( fun ) PFN_wgl##fun wgl##fun;
#include "Miscellaneous/OpenGLFunctionsList.inl"

	private:
		void doLoadBaseFunctions();
		void doLoadMswFunctions();
		HGLRC doCreateDummyContext();
		bool doSelectFormat();
		bool doCreateGl3Context();

	private:
		HDC m_hDC;
		HGLRC m_hContext;
		HWND m_hWnd;
	};
}

#endif
