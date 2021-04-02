#include "Core/GlContext.hpp"

#include "Core/GlContextLock.hpp"
#include "Core/GlSurface.hpp"

#include "ashesgl_api.hpp"

#if _WIN32
#	include <gl/GL.h>
#endif

#include <iostream>
#include <locale>

#ifndef NDEBUG
#	define Ashes_LogContextLocking 0
#else
#	define Ashes_LogContextLocking 0
#endif

#if Ashes_LogContextLocking

#define logContextLock()\
	std::stringstream stream;\
	stream.imbue( std::locale{ "C" } );\
	stream << "Context [0x" << std::setfill( '0' ) << std::setw( 8 ) << std::hex << this << "] locked by thread [" << std::this_thread::get_id() << "]";\
	reportVerbose( m_instance\
		, "CONTEXT"\
		, stream.str().c_str() )

#define logContextUnlock()\
	std::stringstream stream;\
	stream.imbue( std::locale{ "C" } );\
	stream << "Context [0x" << std::setfill( '0' ) << std::setw( 8 ) << std::hex << this << "] unlocked by thread [" << std::this_thread::get_id() << "]";\
	reportVerbose( m_instance\
		, "CONTEXT"\
		, stream.str().c_str() )

#else

#define logContextLock()
#define logContextUnlock()

#endif

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl
{
	//*************************************************************************

	namespace
	{
		GLuint allocateBuffer( ContextLock const & context
			, GLuint result
			, GlBufferTarget target
			, GLsizeiptr size
			, GlBufferDataUsageFlags flags )
		{
			glLogCall( context
				, glBindBuffer
				, target
				, result );
			glLogCall( context
				, glBufferData
				, target
				, size
				, nullptr
				, flags );
			glLogCall( context
				, glBindBuffer
				, target
				, 0u );
			return result;
		}
	}

	//*************************************************************************

	Context const & getContext( ContextLock const & lock )
	{
		return lock.getContext();
	}

	//*************************************************************************

	Context::Context( gl::ContextImplPtr impl )
		: m_impl{ std::move( impl ) }
		, m_instance{ m_impl->instance }
	{
		m_impl->preInitialise( MinMajor, MinMinor );
		m_impl->enable();
		loadBaseFunctions();
		m_impl->disable();
		m_impl->postInitialise();
	}

	Context::~Context()
	{
	}

#if _WIN32

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface )
	{
		if ( get( surface )->isWin32() )
		{
			return create( instance
				, get( surface )->getWin32CreateInfo()
				, &get( instance )->getCurrentContext() );
		}
		else if ( get( surface )->isDisplay() )
		{
			return create( instance
				, get( surface )->getDisplayCreateInfo()
				, &get( instance )->getCurrentContext() );
		}

		return nullptr;
	}

#elif __linux__

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface )
	{
		if ( get( surface )->isXlib() )
		{
			return create( instance
				, get( surface )->getXlibCreateInfo()
				, &get( instance )->getCurrentContext() );
		}
		else if ( get( surface )->isXcb() )
		{
			return create( instance
				, get( surface )->getXcbCreateInfo()
				, &get( instance )->getCurrentContext() );
		}
		else if ( get( surface )->isWayland() )
		{
			return create( instance
				, get( surface )->getWaylandCreateInfo()
				, &get( instance )->getCurrentContext() );
		}
		else if ( get( surface )->isDisplay() )
		{
			return create( instance
				, get( surface )->getDisplayCreateInfo()
				, &get( instance )->getCurrentContext() );
		}

		return nullptr;
	}

#elif __APPLE__

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface )
	{
		if ( get( surface )->isMacOS() )
		{
			return create( instance
				, get( surface )->getMacOSCreateInfo()
				, &get( instance )->getCurrentContext() );
		}
		else if ( get( surface )->isDisplay() )
		{
			return create( instance
				, get( surface )->getDisplayCreateInfo()
				, &get( instance )->getCurrentContext() );
		}

		return nullptr;
	}

#endif

	ContextState & Context::getState()
	{
		auto id = std::this_thread::get_id();
		auto it = m_state.find( id );

		if ( it == m_state.end() )
		{
			it = m_state.emplace( std::move( id )
				, std::make_unique< ContextState >() ).first;
			initialiseThreadState( *it->second );
		}

		return *it->second;
	}

#if VK_EXT_debug_utils

	void Context::submitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
		, VkDebugUtilsMessageTypeFlagsEXT messageTypes
		, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const
	{
		get( m_instance )->submitDebugUtilsMessenger( messageSeverity
			, messageTypes
			, callbackData );
	}

#endif
#if VK_EXT_debug_report

	void Context::reportMessage( VkDebugReportFlagsEXT flags
		, VkDebugReportObjectTypeEXT objectType
		, uint64_t object
		, size_t location
		, int32_t messageCode
		, const char * pLayerPrefix
		, const char * pMessage )
	{
		get( m_instance )->reportMessage( flags
			, objectType
			, object
			, location
			, messageCode
			, pLayerPrefix
			, pMessage );
	}

#endif

	void Context::lock()
	{
		m_mutex.lock();
		m_enabled = true;
		m_activeThread = std::this_thread::get_id();
		m_impl->enable();
		logContextLock();
	}

	void Context::unlock()
	{
		logContextUnlock();
		assert( isEnabled() );
		m_impl->disable();
		m_activeThread.exchange( {} );
		m_enabled = false;
		m_mutex.unlock();
	}

	GLuint Context::createBuffer( GlBufferTarget target
		, GLsizeiptr size
		, GlBufferDataUsageFlags flags )
	{
		GLuint result;
		ContextLock context{ *this };
		assert( isEnabled() );
		glLogCreateCall( context
			, glGenBuffers
			, 1u
			, &result );
		auto it = findBuffer( result );

		while ( it != m_buffers.end() )
		{
			std::stringstream err;
			err << "Buffer " << result << " is being reused";
			reportWarning( m_instance, VK_SUCCESS, "Buffer memory", err.str() );
			allocateBuffer( context, it->name, it->target, it->size, it->flags );
			glLogCreateCall( context
				, glGenBuffers
				, 1u
				, &result );
			it = findBuffer( result );
		}

		assert( isEnabled() );
		allocateBuffer( context, result, target, size, flags );
		GLint realSize = getBufferSize( context, target, result );
		assert( isEnabled() );
		assert( realSize >= size );
		m_buffers.push_back( { result, target, GLsizeiptr( realSize ), flags } );
		return result;
	}

	void Context::deleteBuffer( GLuint buffer )
	{
		if ( buffer != GL_INVALID_INDEX )
		{
			ContextLock context{ *this };
			GLint size = getBufferSize( context, buffer );

			if ( size != 0 )
			{
				auto it = findBuffer( buffer, size );

				if ( it != m_buffers.end() )
				{
					m_buffers.erase( it );
					glLogCall( context
						, glDeleteBuffers
						, 1u
						, &buffer );
				}
				else
				{
					std::stringstream err;
					err << "Couldn't find buffer " << buffer << " it has probably been reused";
					reportWarning( m_instance, VK_SUCCESS, "Buffer memory", err.str() );
				}
			}
			else
			{
				std::stringstream err;
				err << "Couldn't find buffer " << buffer << " it has probably been reused";
				reportWarning( m_instance, VK_SUCCESS, "Buffer memory", err.str() );
			}
		}
	}

	void Context::loadBaseFunctions()
	{
#if _WIN32
#	define GL_LIB_BASE_FUNCTION( fun )\
		m_gl##fun = PFN_gl##fun( &::gl##fun );\
		if ( !m_gl##fun )\
		{\
			throw std::runtime_error{ std::string{ "Couldn't load base function " } + "gl"#fun };\
		}
#else
#	define GL_LIB_BASE_FUNCTION( fun )\
		std::stringstream err##fun;\
		if ( !( getFunction( "gl"#fun, m_gl##fun, err##fun ) ) )\
		{\
			throw std::runtime_error{ std::string{ "Couldn't load base function " } + "gl"#fun + err##fun.str() };\
		}
#endif
#define GL_LIB_FUNCTION( fun )\
		std::stringstream err##fun;\
		if ( !( getFunction( "gl"#fun, m_gl##fun, err##fun ) ) )\
		{\
			throw std::runtime_error{ std::string{ "Couldn't load function " } + "gl"#fun + err##fun.str() };\
		}

#define GL_LIB_FUNCTION_OPT( fun, ... )\
		std::stringstream err##fun;\
		if ( !( getFunction( "gl"#fun, m_gl##fun, err##fun ) ) )\
		{\
			reportWarning( m_instance, VK_SUCCESS, "Context", "Couldn't load optional function gl"#fun + err##fun.str() );\
		}

#define GL_LIB_FUNCTION_EXT( fun, ... )\
		std::stringstream err##fun;\
		if ( !( getFunction( "gl"#fun, m_gl##fun, err##fun, __VA_ARGS__ ) ) )\
		{\
			reportWarning( m_instance, VK_SUCCESS, "Context", "Couldn't load function gl"#fun + err##fun.str() );\
		}
#include "Miscellaneous/OpenGLFunctionsList.inl"
	}

	void Context::initialiseThreadState( ContextState const & state )
	{
		ContextLock lock{ *this };
		glLogCall( lock
			, glEnable
			, GL_TEXTURE_CUBE_MAP_SEAMLESS );
	}

	GLint Context::getBufferSize( ContextLock const & context
		, GlBufferTarget target
		, GLuint buffer )
	{
		GLint result = 0;
		auto it = findBuffer( buffer );

		if ( it != m_buffers.end() )
		{
			target = it->target;
		}

		glLogCall( context
			, glBindBuffer
			, target
			, buffer );
		glLogCall( context
			, glGetBufferParameteriv
			, target
			, GL_BUFFER_PARAMETER_SIZE
			, &result );
		glLogCall( context
			, glBindBuffer
			, target
			, 0u );
		return result;
	}

	GLint Context::getBufferSize( ContextLock const & context
		, GLuint buffer )
	{
		return getBufferSize( context
			, GL_BUFFER_TARGET_COPY_WRITE
			, buffer );
	}

	Context::BufferAllocCont::iterator Context::findBuffer( GLuint buffer )
	{
		return std::find_if( m_buffers.begin()
			, m_buffers.end()
			, [buffer]( BufferAlloc const & lookup )
			{
				return lookup.name == buffer;
			} );
	}

	Context::BufferAllocCont::iterator Context::findBuffer( GLuint buffer
		, GLsizeiptr size )
	{
		return std::find_if( m_buffers.begin()
			, m_buffers.end()
			, [buffer, size]( BufferAlloc const & lookup )
			{
				return lookup.name == buffer
					&& lookup.size == size;
			} );
	}

	void Context::checkOutOfMemory()const
	{
		if ( m_outOfMemory )
		{
			throw Exception{ VK_ERROR_OUT_OF_DEVICE_MEMORY, "" };
		}
	}

	//*************************************************************************
}
