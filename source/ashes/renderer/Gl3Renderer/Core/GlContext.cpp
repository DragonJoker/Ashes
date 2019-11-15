#include "Core/GlContext.hpp"

#include "Core/GlContextLock.hpp"

#if VK_USE_PLATFORM_WIN32_KHR
#	include "Core/GlMswContext.hpp"
#elif VK_USE_PLATFORM_XLIB_KHR
#	include "Core/GlX11Context.hpp"
#else
#	error "Unsupported surface type"
#endif

#include "ashesgl3_api.hpp"

#include <iostream>
#include <locale>

#if Ashes_LogContextLocking

#define logContextLock()\
	std::stringstream stream;\
	stream.imbue( std::locale{ "C" } );\
	stream << "Context [0x" << std::setfill( '0' ) << std::setw( 8 ) << std::hex << this << "] locked by thread [" << std::this_thread::get_id() << "]";\
	get( m_instance )->reportMessage( VK_DEBUG_REPORT_DEBUG_BIT_EXT\
		, VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT\
		, uint64_t( m_instance )\
		, 0u\
		, 0u\
		, "CONTEXT"\
		, stream.str().c_str() )

#define logContextUnlock()\
	std::stringstream stream;\
	stream.imbue( std::locale{ "C" } );\
	stream << "Context [0x" << std::setfill( '0' ) << std::setw( 8 ) << std::hex << this << "] unlocked by thread [" << std::this_thread::get_id() << "]";\
	get( m_instance )->reportMessage( VK_DEBUG_REPORT_DEBUG_BIT_EXT\
		, VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT\
		, uint64_t( m_instance )\
		, 0u\
		, 0u\
		, "CONTEXT"\
		, stream.str().c_str() )

#else

#define logContextLock()
#define logContextUnlock()

#endif

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl3
{
	//*************************************************************************

	Context const & getContext( ContextLock const & lock )
	{
		return lock.getContext();
	}

	//*************************************************************************

	Context::Context( std::unique_ptr< ContextImpl > impl )
		: m_impl{ std::move( impl ) }
		, createInfo{ m_impl->createInfo }
		, m_instance{ m_impl->instance }
	{
		m_impl->initialise( *this );
	}

	Context::~Context()
	{
	}

#if defined( VK_USE_PLATFORM_ANDROID_KHR )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

#elif defined( VK_USE_PLATFORM_FUCHSIA )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

#elif defined( VK_USE_PLATFORM_IOS_MVK )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

#elif defined( VK_USE_PLATFORM_MACOS_MVK )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

#elif defined( VK_USE_PLATFORM_VI_NN )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

#elif defined( VK_USE_PLATFORM_XCB_KHR )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

#elif defined( VK_USE_PLATFORM_XLIB_KHR )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		return std::unique_ptr< Context >( new Context
			{
				std::make_unique< X11Context >( instance
					, std::move( createInfo )
					, mainContext )
			} );
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		return std::unique_ptr< Context >( new Context
			{
				std::make_unique< X11Context >( instance
					, surface
					, mainContext )
			} );
	}

#elif defined( VK_USE_PLATFORM_WAYLAND_KHR )

ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		throw std::runtime_error{ "Not implemented" };
	}

#elif defined( VK_USE_PLATFORM_WIN32_KHR )

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
	{
		return std::unique_ptr< Context >( new Context
			{
				std::make_unique< MswContext >( instance
					, std::move( createInfo )
					, mainContext )
			} );
	}

	ContextPtr Context::create( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
	{
		return std::unique_ptr< Context >( new Context
			{
				std::make_unique< MswContext >( instance
					, surface
					, mainContext )
			} );
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

	void Context::onBaseContextCreated()
	{
		loadBaseFunctions();
		loadDebugFunctions();
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
		m_impl->disable();
		m_enabled = false;
		m_mutex.unlock();
	}

	void Context::loadBaseFunctions()
	{
#define GL_LIB_BASE_FUNCTION( fun )\
		m_gl##fun = PFN_gl##fun( &::gl##fun );
#define GL_LIB_FUNCTION( fun )\
		if ( !( getFunction( "gl"#fun, m_gl##fun ) ) )\
		{\
			throw std::runtime_error{ std::string{ "Couldn't load function " } + "gl"#fun };\
		}
#if !Ashes_Gl3RemoveExtensions
#	define GL_LIB_FUNCTION_EXT( fun, ext, name )\
		if ( !( getFunction( "gl"#fun, m_gl##fun##_##ext ) ) )\
		{\
			std::cerr << "Couldn't load function " << "gl"#fun << std::endl;\
		}
#	define GL_LIB_FUNCTION_VSN( fun, version )\
		if ( !( getFunction( "gl"#fun, m_gl##fun##_##version ) ) )\
		{\
			std::cerr << "Couldn't load function " << "gl"#fun << std::endl;\
		}
#endif
#include "Miscellaneous/OpenGLFunctionsList.inl"
	}

	void Context::loadDebugFunctions()
	{
		auto & extensions = get( m_impl->instance )->getExtensions();

		if ( extensions.find( KHR_debug ) )
		{
			if ( !getFunction( "glDebugMessageCallback", m_glDebugMessageCallback ) )
			{
				if ( !getFunction( "glDebugMessageCallbackKHR", m_glDebugMessageCallback ) )
				{
					std::cerr << "Unable to retrieve function glDebugMessageCallback" << std::endl;
				}
			}
		}
		else if ( extensions.find( ARB_debug_output ) )
		{
			if ( !getFunction( "glDebugMessageCallback", m_glDebugMessageCallback ) )
			{
				if ( !getFunction( "glDebugMessageCallbackARB", m_glDebugMessageCallback ) )
				{
					std::cerr << "Unable to retrieve function glDebugMessageCallback" << std::endl;
				}
			}
		}
		else if ( extensions.find( AMDX_debug_output ) )
		{
			if ( !getFunction( "glDebugMessageCallbackAMD", m_glDebugMessageCallbackAMD ) )
			{
				std::cerr << "Unable to retrieve function glDebugMessageCallbackAMD" << std::endl;
			}
		}
	}

	void Context::initialiseThreadState( ContextState const & state )
	{
		ContextLock lock{ *this };
		glLogCall( lock
			, glEnable
			, GlTweak( GL_TEXTURE_CUBE_MAP_SEAMLESS ) );
	}

	//*************************************************************************
}
