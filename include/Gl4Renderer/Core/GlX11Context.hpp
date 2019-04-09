/*
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Core/GlContext.hpp"

#if ASHES_XLIB
#	include <X11/Xlib.h>
#	include <GL/glx.h>

namespace ashes::gl4
{
	class X11Context
		: public ContextImpl
	{
	public:
		X11Context( VkInstance instance
			, VkSurfaceCreateInfoKHR createInfo
			, Context const * mainContext );
		X11Context( VkInstance instance
			, VkSurfaceKHR surface
			, Context const * mainContext );
		~X11Context();

		void initialise( Context & parent )override;
		void loadSystemFunctions()override;
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
		inline auto glX##fun( Params... params )const\
		{\
			return m_glX##fun( params... );\
		}
#include "Gl4Renderer/Miscellaneous/OpenGLFunctionsList.inl"

	private:
		XVisualInfo * doCreateVisualInfoWithFBConfig( std::vector< int > arrayAttribs, int screen );
		XVisualInfo * doCreateVisualInfoWithoutFBConfig( std::vector< int > arrayAttribs, int screen );
		bool doCreateGl3Context( X11Context const * mainContext );

	protected:
		GLXContext m_glxContext;
		int m_glxVersion;
		GLXFBConfig * m_fbConfig;
		Context const * m_mainContext;
	};
}

#endif
