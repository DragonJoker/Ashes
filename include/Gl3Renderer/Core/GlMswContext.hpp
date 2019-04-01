/*
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Core/GlContext.hpp"

#if ASHES_WIN32

#	include <Windows.h>

namespace gl_renderer
{
	class MswContext
		: public Context
	{
	public:
		MswContext( Instance const & instance
			, ashes::WindowHandle const & handle
			, Context const * mainContext );
		~MswContext();

		void enable()const override;
		void disable()const override;
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
			return m_wgl##fun( params... );\
		}
#define WGL_LIB_FUNCTION_EXT( fun, ext, name )\
		PFN_wgl##fun m_wgl##fun##_##ext = nullptr;\
		template< typename ... Params >\
		auto wgl##fun##_##ext( Params... params )const\
		{\
			return m_wgl##fun##_##ext( params... );\
		}\
		bool whas##fun##_##ext()const\
		{\
			return bool( m_wgl##fun##_##ext );\
		}
#include "Gl3Renderer/Miscellaneous/OpenGLFunctionsList.inl"

	private:
		void doLoadBaseFunctions();
		void doLoadMswFunctions();
		void doLoadDebugFunctions();
		HGLRC doCreateDummyContext();
		bool doSelectFormat();
		bool doCreateGl3Context( MswContext const * mainContext );

	private:
		HDC m_hDC;
		HGLRC m_hContext;
		HWND m_hWnd;
	};
}

#endif
