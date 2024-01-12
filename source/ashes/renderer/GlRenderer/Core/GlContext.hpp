/*
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"
#include "renderer/GlRenderer/Core/GlContextImpl.hpp"
#include "renderer/GlRenderer/Core/GlContextState.hpp"

#pragma warning( push )
#pragma warning( disable: 5262 )
#include <atomic>
#include <mutex>
#include <thread>
#pragma warning( pop )

namespace ashes::gl
{
	class ContextLock;

	class Context
	{
	private:
		struct BufferAlloc
		{
			GLuint name;
			GlBufferTarget target;
			GLsizeiptr size;
			GlBufferDataUsageFlags flags;
		};
		using BufferAllocCont = std::vector< BufferAlloc >;

		explicit Context( gl::ContextImplPtr impl );

	public:
		~Context()noexcept = default;

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
		void unlock()noexcept;

		GLuint createBuffer( GlBufferTarget target
			, GLsizeiptr size
			, GlBufferDataUsageFlags flags );
		void deleteBuffer( GLuint buffer )noexcept;

		void setOutOfMemory()
		{
			m_outOfMemory = true;
		}

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
			, VkSurfaceKHR surface );

		void swapBuffers()const
		{
			m_impl->swapBuffers();
		}

		bool isEnabled()const noexcept
		{
			return m_enabled
				&& m_activeThread == std::this_thread::get_id();
		}

		gl::ContextImpl const & getImpl()const
		{
			return *m_impl;
		}

		VkInstance getInstance()const
		{
			return m_instance;
		}

		VkExtent2D getExtent()const
		{
			return m_impl->getExtent();
		}

#define GL_LIB_BASE_FUNCTION( fun )\
		PFN_gl##fun m_gl##fun = nullptr;\
		template< typename ... Params >\
		auto gl##fun( Params... params )const noexcept\
		{\
			checkOutOfMemory();\
			return m_gl##fun( params... );\
		}
#define GL_LIB_FUNCTION( fun )\
		PFN_gl##fun m_gl##fun = nullptr;\
		template< typename ... Params >\
		auto gl##fun( Params... params )const noexcept\
		{\
			checkOutOfMemory();\
			return m_gl##fun( params... );\
		}
#define GL_LIB_FUNCTION_OPT( fun )\
		PFN_gl##fun m_gl##fun = nullptr;\
		template< typename ... Params >\
		auto gl##fun( Params... params )const noexcept\
		{\
			checkOutOfMemory();\
			return m_gl##fun( params... );\
		}\
		bool has##fun()const noexcept\
		{\
			return bool( m_gl##fun );\
		}
#define GL_LIB_FUNCTION_EXT( fun, ... )\
		PFN_gl##fun m_gl##fun = nullptr;\
		template< typename ... Params >\
		auto gl##fun( Params... params )const noexcept\
		{\
			checkOutOfMemory();\
			return m_gl##fun( params... );\
		}\
		bool has##fun()const noexcept\
		{\
			return bool( m_gl##fun );\
		}
#include "renderer/GlRenderer/Miscellaneous/OpenGLFunctionsList.inl"

	private:
		void loadBaseFunctions();
		void initialiseThreadState();
		GLint getBufferSize( ContextLock const & context
			, GlBufferTarget target
			, GLuint buffer )noexcept;
		GLint getBufferSize( ContextLock const & context
			, GLuint buffer )noexcept;
		BufferAllocCont::iterator findBuffer( GLuint buffer )noexcept;
		BufferAllocCont::iterator findBuffer( GLuint buffer
			, GLsizeiptr size )noexcept;
		void checkOutOfMemory()const noexcept;

	private:
		gl::ContextImplPtr m_impl;

	protected:
		VkInstance m_instance;
		std::mutex m_mutex;
		std::atomic< bool > m_enabled{ false };
		std::atomic< std::thread::id > m_activeThread;
		std::map< std::thread::id, std::unique_ptr< gl::ContextState > > m_state;
		BufferAllocCont m_buffers;
		std::atomic< bool > m_outOfMemory{ false };
	};
}
