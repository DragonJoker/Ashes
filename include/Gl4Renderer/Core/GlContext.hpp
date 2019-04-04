/*
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <atomic>

namespace ashes::gl4
{
	extern char const VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME[VK_MAX_EXTENSION_NAME_SIZE];

	class Context
	{
	protected:
		Context( VkInstance instance );

	public:
		virtual ~Context();
		/**
		*\brief
		*	Active le contexte.
		*/
		virtual void enable()const = 0;
		/**
		*\brief
		*	Désactive le contexte.
		*/
		virtual void disable()const = 0;
		/**
		*\brief
		*	Echange les tampons.
		*/
		virtual void swapBuffers()const = 0;

		inline bool isEnabled()const
		{
			return m_enabled;
		}

#if defined( VK_USE_PLATFORM_ANDROID_KHR )

		static ContextPtr create( VkInstance instance
			, VkAndroidSurfaceCreateInfoKHR createInfo
			, Context const * mainContext );
		VkAndroidSurfaceCreateInfoKHR m_createInfo;

#elif defined( VK_USE_PLATFORM_FUCHSIA )

		static ContextPtr create( VkInstance instance
			, VkImagePipeSurfaceCreateInfoFUCHSIA createInfo
			, Context const * mainContext );
		VkImagePipeSurfaceCreateInfoFUCHSIA m_createInfo;

#elif defined( VK_USE_PLATFORM_IOS_MVK )

		static ContextPtr create( VkInstance instance
			, VkIOSSurfaceCreateInfoMVK createInfo
			, Context const * mainContext );
		VkIOSSurfaceCreateInfoMVK m_createInfo;

#elif defined( VK_USE_PLATFORM_MACOS_MVK )

		static ContextPtr create( VkInstance instance
			, VkMacOSSurfaceCreateInfoMVK createInfo
			, Context const * mainContext );
		VkMacOSSurfaceCreateInfoMVK m_createInfo;

#elif defined( VK_USE_PLATFORM_VI_NN )

		static ContextPtr create( VkInstance instance
			, VkViSurfaceCreateInfoNN createInfo
			, Context const * mainContext );
		VkViSurfaceCreateInfoNN m_createInfo;

#elif defined( VK_USE_PLATFORM_XCB_KHR )

		static ContextPtr create( VkInstance instance
			, VkXcbSurfaceCreateInfoKHR createInfo
			, Context const * mainContext );
		VkXcbSurfaceCreateInfoKHR m_createInfo;

#elif defined( VK_USE_PLATFORM_XLIB_KHR )

		static ContextPtr create( VkInstance instance
			, VkXlibSurfaceCreateInfoKHR createInfo
			, Context const * mainContext );
		VkXlibSurfaceCreateInfoKHR m_createInfo;

#elif defined( VK_USE_PLATFORM_WAYLAND_KHR )

		static ContextPtr create( VkInstance instance
			, VkWaylandSurfaceCreateInfoKHR createInfo
			, Context const * mainContext );
		VkWaylandSurfaceCreateInfoKHR m_createInfo;

#elif defined( VK_USE_PLATFORM_WIN32_KHR )

		static ContextPtr create( VkInstance instance
			, VkWin32SurfaceCreateInfoKHR createInfo
			, Context const * mainContext );
		VkWin32SurfaceCreateInfoKHR m_createInfo;

#endif

		static ContextPtr create( VkInstance instance
			, VkSurfaceKHR surface
			, Context const * mainContext );

#define GL_LIB_BASE_FUNCTION( fun )\
		PFN_gl##fun m_gl##fun = nullptr;\
		template< typename ... Params >\
		inline auto gl##fun( Params... params )const\
		{\
			return m_gl##fun( params... );\
		}
#define GL_LIB_FUNCTION( fun )\
		PFN_gl##fun m_gl##fun = nullptr; \
		template< typename ... Params >\
		inline auto gl##fun( Params... params )const\
		{\
			return m_gl##fun( params... );\
		}
#define GL_LIB_FUNCTION_OPT( fun )\
		PFN_gl##fun m_gl##fun = nullptr; \
		template< typename ... Params >\
		inline auto gl##fun( Params... params )const\
		{\
			return m_gl##fun( params... );\
		}\
		inline bool has##fun()const\
		{\
			return bool( m_gl##fun );\
		}
#include "Gl4Renderer/Miscellaneous/OpenGLFunctionsList.inl"

		PFN_glObjectLabel glObjectLabel = nullptr;
		PFN_glObjectPtrLabel glObjectPtrLabel = nullptr;

	protected:
		VkInstance m_instance;
		mutable std::atomic< bool > m_enabled{ false };

		using PFN_glDebugMessageCallback = void ( GLAPIENTRY * )( PFNGLDEBUGPROC callback, void * userParam );
		using PFN_glDebugMessageCallbackAMD = void ( GLAPIENTRY * )( PFNGLDEBUGAMDPROC callback, void * userParam );

		PFN_glDebugMessageCallback glDebugMessageCallback = nullptr;
		PFN_glDebugMessageCallbackAMD glDebugMessageCallbackAMD = nullptr;
	};

	class ContextLock
	{
	public:
		ContextLock( ContextLock const & ) = delete;
		ContextLock & operator=( ContextLock const & ) = delete;

		inline ContextLock( Context const & context )
			: m_context{ &context }
			, m_disable{ !context.isEnabled() }
		{
			if ( m_disable )
			{
				m_context->enable();
			}
		}

		ContextLock( ContextLock && rhs )
			: m_context{ rhs.m_context }
			, m_disable{ rhs.m_disable }
		{
			rhs.m_context = nullptr;
		}

		ContextLock & operator=( ContextLock && rhs )
		{
			if ( &rhs != this )
			{
				m_context = rhs.m_context;
				m_disable = rhs.m_disable;
				rhs.m_context = nullptr;
			}

			return *this;
		}

		inline ~ContextLock()
		{
			if ( m_context && m_disable )
			{
				m_context->disable();
			}
		}

		Context const * operator->()const
		{
			return m_context;
		}

	private:
		Context const * m_context;
		bool m_disable;
	};
}
