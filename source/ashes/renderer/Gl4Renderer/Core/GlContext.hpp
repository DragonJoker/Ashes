/*
See LICENSE file in root folder
*/
#pragma once

#include "GlContextState.hpp"

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
	extern char const VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME[VK_MAX_EXTENSION_NAME_SIZE];
	class ContextLock;

	class Context
	{
	public:
		class ContextImpl
		{
		protected:
			ContextImpl( VkInstance instance
				, VkSurfaceCreateInfoKHR createInfo )
				: createInfo{ std::move( createInfo ) }
				, instance{ instance }
			{
			}

		public:
			virtual ~ContextImpl() = default;

			virtual void initialise( Context & parent ) = 0;
			virtual void loadSystemFunctions() = 0;
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

			VkSurfaceCreateInfoKHR createInfo;
			VkInstance instance;
		};

	private:
		Context( std::unique_ptr< ContextImpl > impl );

	public:
		~Context();

		ContextState & getState();
		void onBaseContextCreated();

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

		static ContextPtr create( VkInstance instance
			, VkSurfaceCreateInfoKHR createInfo
			, Context const * mainContext );
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

		inline ContextImpl const & getImpl()const
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

		void initialiseThreadState( ContextState const & state );

	private:
		std::unique_ptr< ContextImpl > m_impl;

	public:
		VkSurfaceCreateInfoKHR const & createInfo;

	protected:
		VkInstance m_instance;
		std::mutex m_mutex;
		std::atomic< bool > m_enabled{ false };
		std::atomic< std::thread::id > m_activeThread;
		std::map< std::thread::id, std::unique_ptr< ContextState > > m_state;
	};
}
