/*
See LICENSE file in root folder
*/
#pragma once

#if defined( VK_USE_PLATFORM_WIN32_KHR )
#	include "renderer/Gl3Renderer/Core/GlContext.hpp"

#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	include <Windows.h>
#	include <gl/GL.h>
#	include "vulkan/vulkan_win32.h"

namespace ashes::gl3
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
#include "renderer/Gl3Renderer/Miscellaneous/OpenGLFunctionsList.inl"

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
