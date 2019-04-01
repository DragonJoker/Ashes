/*
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include <atomic>

namespace gl_renderer
{
	class Context
	{
	protected:
		Context( Instance const & instance );

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

		/**
		*\brief
		*	Crée un contexte.
		*/
		static ContextPtr create( Instance const & instance
			, ashes::WindowHandle const & handle
			, Context const * mainContext );

#define GL_LIB_BASE_FUNCTION( fun )\
		PFN_gl##fun m_gl##fun = nullptr; \
		template< typename ... Params >\
		auto gl##fun( Params... params )const\
		{\
			return m_gl##fun( params... );\
		}
#define GL_LIB_FUNCTION( fun )\
		PFN_gl##fun m_gl##fun = nullptr; \
		template< typename ... Params >\
		auto gl##fun( Params... params )const\
		{\
			return m_gl##fun( params... );\
		}
#define GL_LIB_FUNCTION_EXT( fun, ext, name )\
		PFN_gl##fun m_gl##fun##_##ext = nullptr; \
		template< typename ... Params >\
		auto gl##fun##_##ext( Params... params )const\
		{\
			return m_gl##fun##_##ext( params... );\
		}\
		bool has##fun##_##ext()const\
		{\
			return bool( m_gl##fun##_##ext );\
		}
#define GL_LIB_FUNCTION_VSN( fun, version )\
		PFN_gl##fun m_gl##fun##_##version = nullptr; \
		template< typename ... Params >\
		auto gl##fun##_##version( Params... params )const\
		{\
			return m_gl##fun##_##version( params... );\
		}\
		bool has##fun##_##version()const\
		{\
			return bool( m_gl##fun##_##version );\
		}
#include "Gl3Renderer/Miscellaneous/OpenGLFunctionsList.inl"

		PFN_glObjectLabel glObjectLabel = nullptr;
		PFN_glObjectPtrLabel glObjectPtrLabel = nullptr;

	protected:
		Instance const & m_instance;
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
