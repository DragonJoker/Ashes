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

		static ContextPtr create( VkInstance instance
			, VkSurfaceCreateInfoKHR createInfo
			, Context const * mainContext );
		VkSurfaceCreateInfoKHR m_createInfo;
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
