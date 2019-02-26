/*
See LICENSE file in root folder
*/
#ifndef ___GL_CONTEXT_H___
#define ___GL_CONTEXT_H___

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include "Gl3Renderer/Core/GlConnection.hpp"

#include <atomic>

namespace gl_renderer
{
	class Context
	{
	protected:
		Context( PhysicalDevice const & gpu
			, ashes::Connection const & connection );

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
		static ContextPtr create( PhysicalDevice const & gpu
			, ashes::Connection const & connection
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

#if !defined( NDEBUG )
		PFN_glObjectLabel glObjectLabel = nullptr;
		PFN_glObjectPtrLabel glObjectPtrLabel = nullptr;
#endif

	protected:
		PhysicalDevice const & m_gpu;
		ashes::Connection const & m_connection;
		mutable std::atomic< bool > m_enabled{ false };

#if !defined( NDEBUG )
		using PFNGLDEBUGPROC = void ( GLAPIENTRY * )( uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int length, const char * message, void * userParam );
		using PFNGLDEBUGAMDPROC = void ( GLAPIENTRY * )( uint32_t id, uint32_t category, uint32_t severity, int length, const char* message, void* userParam );
		using PFN_glDebugMessageCallback = void ( GLAPIENTRY * )( PFNGLDEBUGPROC callback, void * userParam );
		using PFN_glDebugMessageCallbackAMD = void ( GLAPIENTRY * )( PFNGLDEBUGAMDPROC callback, void * userParam );

		PFN_glDebugMessageCallback glDebugMessageCallback = nullptr;
		PFN_glDebugMessageCallbackAMD glDebugMessageCallbackAMD = nullptr;
#endif
	};
}

#endif
