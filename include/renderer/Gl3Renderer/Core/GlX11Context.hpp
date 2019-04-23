/*
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Core/GlContext.hpp"

#if ASHES_XLIB
#	include <X11/Xlib.h>
#	include <GL/glx.h>

namespace gl_renderer
{
	class X11Context
		: public Context
	{
	public:
		X11Context( Instance const & instance
			, ashes::WindowHandle const & handle
			, Context const * mainContext );
		~X11Context();

		void enable()const override;
		void disable()const override;
		void swapBuffers()const override;

		inline GLXContext getContext()
		{
			return m_glxContext;
		}

#define GLX_LIB_FUNCTION( fun )\
		PFN_glX##fun m_glX##fun = nullptr;\
		template< typename ... Params >\
		auto glX##fun( Params... params )const\
		{\
			return m_glX##fun( params... );\
		}
#include "Gl3Renderer/Miscellaneous/OpenGLFunctionsList.inl"

	private:
		void doLoadBaseFunctions();
		void doLoadGLXFunctions();
		void doLoadDebugFunctions();
		XVisualInfo * doCreateVisualInfoWithFBConfig( std::vector< int > arrayAttribs, int screen );
		XVisualInfo * doCreateVisualInfoWithoutFBConfig( std::vector< int > arrayAttribs, int screen );
		bool doCreateGl3Context( X11Context const * mainContext );

	protected:
		GLXContext m_glxContext;
		int m_glxVersion{ 0 };
		Display * m_display{ nullptr };
		GLXDrawable m_drawable{ 0 };
		GLXFBConfig * m_fbConfig{ nullptr };
	};
}

#endif
