/*
See LICENSE file in root folder
*/
#ifndef ___GL_CONTEXT_H___
#define ___GL_CONTEXT_H___

#include "GlContextSelector.hpp"

#include "Core/GlConnection.hpp"

namespace gl_renderer
{
	class Context
	{
	protected:
		Context( PhysicalDevice const & gpu
			, renderer::ConnectionPtr && connection );

	public:
		virtual ~Context();
		/**
		*\brief
		*	Active le contexte.
		*/
		virtual void setCurrent()const = 0;
		/**
		*\brief
		*	Désactive le contexte.
		*/
		virtual void endCurrent()const = 0;
		/**
		*\brief
		*	Echange les tampons.
		*/
		virtual void swapBuffers()const = 0;
		/**
		*\brief
		*	Crée un contexte.
		*/
		static ContextPtr create( PhysicalDevice const & gpu
			, renderer::ConnectionPtr && connection );

		inline std::thread::id getThreadId()const
		{
			return m_threadId;
		}

#define GL_LIB_BASE_FUNCTION( fun )\
		PFN_gl##fun m_gl##fun = nullptr;\
		template< typename ... Params >\
		auto gl##fun( Params... params )const\
		{\
			m_selector.enableContextForCurrentThread();\
			return m_gl##fun( params... );\
		}
#define GL_LIB_FUNCTION( fun )\
		PFN_gl##fun m_gl##fun = nullptr; \
		template< typename ... Params >\
		auto gl##fun( Params... params )const\
		{\
			m_selector.enableContextForCurrentThread();\
			return m_gl##fun( params... );\
		}
#define GL_LIB_FUNCTION_OPT( fun )\
		PFN_gl##fun m_gl##fun = nullptr; \
		template< typename ... Params >\
		auto gl##fun( Params... params )const\
		{\
			m_selector.enableContextForCurrentThread();\
			return m_gl##fun( params... );\
		}\
		bool has##fun()const\
		{\
			return bool( m_gl##fun );\
		}
#include "Miscellaneous/OpenGLFunctionsList.inl"

	protected:
		ContextSelector const & m_selector;
		PhysicalDevice const & m_gpu;
		renderer::ConnectionPtr m_connection;
		std::thread::id m_threadId;
	};
}

#endif
