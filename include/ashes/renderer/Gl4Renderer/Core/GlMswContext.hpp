/*
See LICENSE file in root folder
*/
#pragma once

#if _WIN32
#	include "renderer/Gl4Renderer/Core/GlContext.hpp"

#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	include <Windows.h>
#	include <gl/GL.h>
#	include "vulkan/vulkan_win32.h"

namespace ashes::gl4
{
	class MswContext
		: public Context::ContextImpl
	{
	public:
		MswContext( VkInstance instance
			, VkSurfaceCreateInfoKHR createInfo
			, Context const * mainContext );
		MswContext( VkInstance instance
			, VkSurfaceKHR surface
			, Context const * mainContext );
		~MswContext();

		void initialise( Context & parent )override;
		void loadSystemFunctions()override;
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
		inline auto wgl##fun( Params... params )const\
		{\
			return m_wgl##fun( params... );\
		}
#define WGL_LIB_FUNCTION_OPT( fun )\
		PFN_wgl##fun m_wgl##fun = nullptr;\
		template< typename ... Params >\
		inline auto wgl##fun( Params... params )const\
		{\
			return m_wgl##fun( params... );\
		};
#include "renderer/Gl4Renderer/Miscellaneous/OpenGLFunctionsList.inl"

	private:
		HGLRC doCreateDummyContext();
		bool doSelectFormat();
		bool doCreateGl3Context( Context const * mainContext );

	private:
		HDC m_hDC;
		HGLRC m_hContext;
		Context const * m_mainContext;
	};
}

#endif
