#include "Core/GlInstance.hpp"

#include "Core/GlContext.hpp"
#include "Core/GlDebugReportCallback.hpp"
#include "Core/GlDevice.hpp"
#include "Core/GlPhysicalDevice.hpp"
#include "Core/GlSurface.hpp"
#include "Miscellaneous/GlWindow.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <sstream>

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	namespace
	{
		std::string convert( const char * ptr )
		{
			return std::string{ ptr };
		}

		StringArray convert( const char * const * ptr
			, uint32_t count )
		{
			StringArray result;

			for ( auto it = ptr; it < ptr + count; ++it )
			{
				result.push_back( convert( *it ) );
			}

			return result;
		}

		bool doCheckEnabledExtensions( VkInstance instance
			, bool report
			, ashes::ArrayView< char const * const > const & extensions )
		{
			auto & available = getSupportedInstanceExtensions();
			ashes::StringArray unsupported;

			for ( auto & extension : extensions )
			{
				if ( available.end() == std::find_if( available.begin()
					, available.end()
					, [&extension]( VkExtensionProperties const & lookup )
					{
						return lookup.extensionName == std::string{ extension };
					} ) )
				{
					unsupported.emplace_back( extension );
				}
			}

			if ( unsupported.empty() )
			{
				return true;
			}

			if ( report )
			{
				std::stringstream stream;

				for ( auto const & ext : unsupported )
				{
					stream << "\n    " << ext;
				}

				reportError( instance
					, VK_ERROR_EXTENSION_NOT_PRESENT
					, "Unspported extensions"
					, stream.str() );
			}
			return false;
		}

		bool doHasEnabledExtensions( VkInstance instance
			, ashes::ArrayView< char const * const > const & extensions )
		{
			return doCheckEnabledExtensions( instance, false, extensions );
		}

		VkApplicationInfo doGetDefaultApplicationInfo()
		{
			return
			{
				VK_STRUCTURE_TYPE_APPLICATION_INFO,
				nullptr,
				nullptr,
				ashes::makeVersion( 1, 0, 0 ),
				nullptr,
				ashes::makeVersion( 1, 0, 0 ),
				Instance::getDefaultApiVersion(),
			};
		}
	}

	Instance::Instance( [[maybe_unused]] VkAllocationCallbacks const * allocInfo
		, VkInstanceCreateInfo const & createInfo )
		: m_applicationInfo{ createInfo.pApplicationInfo ? *createInfo.pApplicationInfo : doGetDefaultApplicationInfo() }
		, m_enabledLayerNames{ convert( createInfo.ppEnabledLayerNames, createInfo.enabledLayerCount ) }
		, m_enabledExtensions{ convert( createInfo.ppEnabledExtensionNames, createInfo.enabledExtensionCount ) }
		, m_window{ new gl::RenderWindow( MinMajor, MinMinor, "GlInstance" ) }
	{
		m_extensions.initialise();
		m_features = m_extensions.getFeatures();
		m_hasViewportArray = m_extensions.find( ARB_viewport_array );
		auto it = std::find_if( m_enabledLayerNames.begin()
			, m_enabledLayerNames.end()
			, []( std::string const & lookup )
			{
				return lookup == "validation";
			} );
		m_validationEnabled = it != m_enabledLayerNames.end();
		m_context = Context::create( get( this )
			, m_window->getCreateInfo()
			, nullptr );
		ContextLock context{ *m_context };
		glCheckError( context, "ContextInitialisation", true );
		m_physicalDevices.emplace_back( VkPhysicalDevice( new PhysicalDevice{ VkInstance( this ) } ) );

		doCheckEnabledExtensions( get( this )
			, true
			, ashes::makeArrayView( createInfo.ppEnabledExtensionNames, createInfo.enabledExtensionCount ) );
	}

	Instance::~Instance()noexcept
	{
		for ( auto & physicalDevice : m_physicalDevices )
		{
			delete get( physicalDevice );
		}

		m_context.reset();
		delete m_window;
	}

	uint32_t Instance::getApiVersion()const
	{
		return m_applicationInfo.apiVersion;
	}

	bool Instance::hasExtension( std::string_view extension )const
	{
		char const * const version = extension.data();
		return doHasEnabledExtensions( get( this )
			, ashes::makeArrayView( &version, 1u ) );
	}

	bool Instance::hasEnabledExtension( std::string_view name )const
	{
		return m_enabledExtensions.end() != std::find_if( m_enabledExtensions.begin()
			, m_enabledExtensions.end()
			, [&name]( std::string const & lookup )
			{
				return lookup == name;
			} );
	}

	void Instance::unregisterDevice( VkDevice device )noexcept
	{
		m_devices.erase( device );
	}

	Context * Instance::registerDevice( VkDevice device )
	{
		m_devices.insert( device );

		if ( m_firstSurfaceContext )
		{
			return m_firstSurfaceContext;
		}

		return m_context.get();
	}

	void Instance::unregisterSurface( VkSurfaceKHR surface )noexcept
	{
		m_surfaces.erase( m_surfaces.find( surface ) );

		if ( m_surfaces.empty() )
		{
			m_firstSurfaceContext = nullptr;
			m_context = Context::create( get( this )
				, m_window->getCreateInfo()
				, nullptr );

			for ( auto & device : m_devices )
			{
				get( device )->unlink();
			}
		}
	}

	ContextPtr Instance::registerSurface( VkSurfaceKHR surface )
	{
		if ( m_context )
		{
			for ( auto & device : m_devices )
			{
				get( device )->cleanupContextDependent( *m_context );
			}

			m_context.reset();
		}

		m_surfaces.insert( surface );
		ContextPtr result;
		auto glSurface = get( surface );

#ifdef VK_KHR_display

		if ( glSurface->isDisplay() )
		{
			result = Context::create( get( this )
				, glSurface->getDisplayCreateInfo()
				, nullptr );
		}

#endif
#if _WIN32

		else if ( glSurface->isWin32() )
		{
			result = Context::create( get( this )
				, glSurface->getWin32CreateInfo()
				, nullptr );
		}

#elif __linux__

		else if ( glSurface->isXcb() )
		{
			result = Context::create( get( this )
				, glSurface->getXcbCreateInfo()
				, nullptr );
		}
		else if ( glSurface->isXlib() )
		{
			result = Context::create( get( this )
				, glSurface->getXlibCreateInfo()
				, nullptr );
		}
		else if ( glSurface->isWayland() )
		{
			result = Context::create( get( this )
				, glSurface->getWaylandCreateInfo()
				, nullptr );
		}

#elif __APPLE__

		else if ( glSurface->isMacOS() )
		{
			result = Context::create( get( this )
				, glSurface->getMacOSCreateInfo()
				, nullptr );
		}

#endif

		if ( result && !m_firstSurfaceContext )
		{
			m_firstSurfaceContext = result.get();

			for ( auto & device : m_devices )
			{
				get( device )->link( surface );
			}
		}

		return result;
	}

	VkPhysicalDeviceArray Instance::enumeratePhysicalDevices()const
	{
		return m_physicalDevices;
	}

	std::array< float, 16 > Instance::frustum( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		// OpenGL right handed (cf. https://www.opengl.org/sdk/docs/man2/xhtml/glFrustum.xml)
		std::array< float, 16 > result{ 0.0f };
		result[0] = ( 2.0f * zNear ) / ( right - left );
		result[5] = ( 2.0f * zNear ) / ( top - bottom );
		result[8] = ( right + left ) / ( right - left );
		result[9] = ( top + bottom ) / ( top - bottom );
		result[10] = ( zNear + zFar ) / ( zNear - zFar );
		result[11] = -1.0f;
		result[13] = ( -2.0f * zFar * zNear ) / ( zFar - zNear );

		return result;
	}

	std::array< float, 16 > Instance::perspective( float radiansFovY
		, float aspect
		, float zNear
		, float zFar )const
	{
		// OpenGL right handed (cf. https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml)
		std::array< float, 16 > result{ 0.0f };
		auto const tanHalfFovy = float( tan( radiansFovY / 2.0f ) );
		result[0] = 1.0f / ( tanHalfFovy * aspect );
		result[5] = 1.0f / tanHalfFovy;
		result[10] = -( zFar + zNear ) / ( zFar - zNear );
		result[11] = -1.0f;
		result[14] = -2.0f * zFar * zNear / ( zFar - zNear );

		return result;
	}

	std::array< float, 16 > Instance::ortho( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		// OpenGL right handed (cf. https://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml)
		std::array< float, 16 > result{ 0.0f };
		result[0] = 2.0f / ( right - left );
		result[5] = 2.0f / ( top - bottom );
		result[10] = -2.0f / ( zFar - zNear );
		result[12] = -( right + left ) / ( right - left );
		result[13] = -( top + bottom ) / ( top - bottom );
		result[14] = -( zFar + zNear ) / ( zFar - zNear );
		result[15] = 1.0f;

		return result;
	}

	std::array< float, 16 > Instance::infinitePerspective( float radiansFovY
		, float aspect
		, float zNear )const
	{
		auto const tanHalfFovy = float( tan( radiansFovY / 2.0f ) );
		float const range = tanHalfFovy * zNear;
		float const left = -range * aspect;
		float const right = range * aspect;
		float const bottom = -range;
		float const top = range;

		std::array< float, 16 > result{ 0.0f };
		result[0] = ( 2.0f * zNear ) / ( right - left );
		result[5] = ( 2.0f * zNear ) / ( top - bottom );
		result[10] = -1.0f;
		result[11] = -1.0f;
		result[14] = -2.0f * zNear;
		return result;
	}

#if VK_EXT_debug_utils

	void Instance::registerDebugMessenger( VkDebugUtilsMessengerEXT messenger
		, PFNGLDEBUGPROC callback
		, void * userParam )const
	{
		auto context = ContextLock{ *m_context };

		if ( context->hasDebugMessageCallback() )
		{
			m_debugMessengers.push_back( { messenger, callback, userParam } );
			glLogCall( context
				, glDebugMessageCallback
				, callback
				, userParam );
			glLogCall( context
				, glEnable
				, GL_DEBUG_OUTPUT_SYNC );
		}
	}

	void Instance::registerDebugMessengerAMD( VkDebugUtilsMessengerEXT messenger
		, PFNGLDEBUGAMDPROC callback
		, void * userParam )const
	{
		auto context = ContextLock{ *m_context };

		if ( context->hasDebugMessageCallbackAMD() )
		{
			m_debugAMDMessengers.push_back( { messenger, callback, userParam } );
			glLogCall( context
				, glDebugMessageCallbackAMD
				, callback
				, userParam );
			glLogCall( context
				, glEnable
				, GL_DEBUG_OUTPUT_SYNC );
		}
	}

	void Instance::submitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
		, VkDebugUtilsMessageTypeFlagsEXT messageTypes
		, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const noexcept
	{
		for ( auto const & callback : m_debugMessengers )
		{
			get( callback.debugMessenger )->submit( messageSeverity
				, messageTypes
				, callbackData
				, callback.userParam );
		}

		for ( auto const & callback : m_debugAMDMessengers )
		{
			get( callback.debugMessenger )->submit( messageSeverity
				, messageTypes
				, callbackData
				, callback.userParam );
		}
	}

#endif
#if VK_EXT_debug_report

	void Instance::registerDebugMessageCallback( VkDebugReportCallbackEXT report
		, PFNGLDEBUGPROC callback
		, void * userParam )const
	{
		auto context = ContextLock{ *m_context };

		if ( context->m_glDebugMessageCallback )
		{
			m_debugCallbacks.push_back( { report, callback, userParam } );
			glLogCall( context
				, glDebugMessageCallback
				, callback
				, userParam );
			glLogCall( context
				, glEnable
				, GL_DEBUG_OUTPUT_SYNC );
		}
	}

	void Instance::registerDebugMessageCallbackAMD( VkDebugReportCallbackEXT report
		, PFNGLDEBUGAMDPROC callback
		, void * userParam )const
	{
		auto context = ContextLock{ *m_context };

		if ( context->m_glDebugMessageCallbackAMD )
		{
			m_debugAMDCallbacks.push_back( { report, callback, userParam } );
			glLogCall( context
				, glDebugMessageCallbackAMD
				, callback
				, userParam );
			glLogCall( context
				, glEnable
				, GL_DEBUG_OUTPUT_SYNC );
		}
	}

	void Instance::reportMessage( VkDebugReportFlagsEXT flags
		, VkDebugReportObjectTypeEXT objectType
		, uint64_t object
		, size_t location
		, int32_t messageCode
		, const char * pLayerPrefix
		, const char * pMessage )const noexcept
	{
		for ( auto const & callback : m_debugCallbacks )
		{
			get( callback.debugReport )->report( flags
				, objectType
				, object
				, location
				, messageCode
				, pLayerPrefix
				, pMessage );
		}

		for ( auto const & callback : m_debugAMDCallbacks )
		{
			get( callback.debugReport )->report( flags
				, objectType
				, object
				, location
				, messageCode
				, pLayerPrefix
				, pMessage );
		}
	}

#endif

	void Instance::registerContext( Context & context )
	{
		ContextLock lock( context );

#if VK_EXT_debug_utils

		for ( auto & callback : m_debugMessengers )
		{
			glLogCall( lock
				, glDebugMessageCallback
				, callback.callback
				, callback.userParam );
		}

		for ( auto & callback : m_debugAMDMessengers )
		{
			glLogCall( lock
				, glDebugMessageCallbackAMD
				, callback.callback
				, callback.userParam );
		}

#endif
#if VK_EXT_debug_report

		for ( auto & callback : m_debugCallbacks )
		{
			glLogCall( lock
				, glDebugMessageCallback
				, callback.callback
				, callback.userParam );
		}

		for ( auto & callback : m_debugAMDCallbacks )
		{
			glLogCall( lock
				, glDebugMessageCallbackAMD
				, callback.callback
				, callback.userParam );
		}

#endif

		if ( false
#if VK_EXT_debug_utils
			|| !m_debugMessengers.empty()
			|| !m_debugAMDMessengers.empty()
#endif
#if VK_EXT_debug_report
			|| !m_debugCallbacks.empty()
			|| !m_debugAMDCallbacks.empty()
#endif
			)
		{
			glLogCall( lock
				, glEnable
				, GL_DEBUG_OUTPUT_SYNC );
		}
	}
}
