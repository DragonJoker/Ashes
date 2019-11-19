/*
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

#include <renderer/GlRendererCommon/GlContextState.hpp>
#include <renderer/GlRendererCommon/GlContext.hpp>

#include <atomic>
#include <mutex>
#include <thread>

#ifndef NDEBUG
#	define Ashes_LogContextLocking 0
#else
#	define Ashes_LogContextLocking 0
#endif

namespace ashes::gl4
{
	class ContextLock;

	class Context
	{
	private:
		Context( gl::ContextImplPtr impl );

	public:
		~Context();

		gl::ContextState & getState();

#if VK_EXT_debug_utils
		void submitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const;
#endif
#if VK_EXT_debug_report
		void reportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage );
#endif

		void lock();
		void unlock();

		template< typename SurfaceCreateInfo >
		static ContextPtr create( VkInstance instance
			, SurfaceCreateInfo createInfo
			, Context const * mainContext )
		{
			return std::unique_ptr< Context >( new Context
				{
					gl::ContextImpl::create( instance
						, std::move( createInfo )
						, ( mainContext
							? &mainContext->getImpl()
							: nullptr ) )
				} );
		}

		static ContextPtr create( VkInstance instance
			, VkSurfaceKHR surface
			, Context const * mainContext );

		inline void swapBuffers()const
		{
			m_impl->swapBuffers();
		}

		inline bool isEnabled()const
		{
			return m_enabled
				&& m_activeThread == std::this_thread::get_id();
		}

		inline gl::ContextImpl const & getImpl()const
		{
			return *m_impl;
		}

		inline VkInstance getInstance()const
		{
			return m_instance;
		}

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
#include "renderer/Gl4Renderer/Miscellaneous/OpenGLFunctionsList.inl"

		PFN_glObjectLabel m_glObjectLabel = nullptr;
		PFN_glObjectPtrLabel m_glObjectPtrLabel = nullptr;
		PFN_glDebugMessageCallback m_glDebugMessageCallback = nullptr;
		PFN_glDebugMessageCallbackAMD m_glDebugMessageCallbackAMD = nullptr;

	private:
		void loadBaseFunctions();
		void loadDebugFunctions();

		void initialiseThreadState( gl::ContextState const & state );

	private:
		gl::ContextImplPtr m_impl;

	protected:
		VkInstance m_instance;
		std::mutex m_mutex;
		std::atomic< bool > m_enabled{ false };
		std::atomic< std::thread::id > m_activeThread;
		std::map< std::thread::id, std::unique_ptr< gl::ContextState > > m_state;
	};
}
