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

#define WGL_LIB_FUNCTION( fun )\
		PFN_wgl##fun m_wgl##fun = nullptr;\
		template< typename ... Params >\
		auto wgl##fun( Params... params )const\
		{\
			m_selector.enableContextForCurrentThread();\
			return m_wgl##fun( params... );\
		}
#define WGL_LIB_FUNCTION_EXT( fun, ext, name )\
		PFN_wgl##fun m_wgl##fun##_##ext = nullptr;\
		template< typename ... Params >\
		auto wgl##fun##_##ext( Params... params )const\
		{\
			m_selector.enableContextForCurrentThread();\
			return m_wgl##fun##_##ext( params... );\
		}\
		bool whas##fun##_##ext()const\
		{\
			return bool( m_wgl##fun##_##ext );\
		}
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
