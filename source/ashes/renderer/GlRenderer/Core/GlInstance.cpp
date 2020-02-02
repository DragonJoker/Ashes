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

#include "ashesgl_api.hpp"

namespace ashes::gl
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

	void doCheckEnabledExtensions( ashes::ArrayView< char const * const > const & extensions )
	{
		auto & available = getSupportedInstanceExtensions();

		for ( auto & extension : extensions )
		{
			if ( available.end() == std::find_if( available.begin()
				, available.end()
				, [&extension]( VkExtensionProperties const & lookup )
				{
					return lookup.extensionName == std::string{ extension };
				} ) )
			{
				throw ExtensionNotPresentException{ extension };
			}
		}
	}

	bool doHasEnabledExtensions( ashes::ArrayView< char const * const > const & extensions )
	{
		try
		{
			doCheckEnabledExtensions( extensions );
			return true;
		}
		catch ( ExtensionNotPresentException & )
		{
			return false;
		}
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
			ashes::makeVersion( 1, 0, 0 ),
		};
	}

	Instance::Instance( VkInstanceCreateInfo createInfo )
		: m_flags{ createInfo.flags }
		, m_applicationInfo{ createInfo.pApplicationInfo ? *createInfo.pApplicationInfo : doGetDefaultApplicationInfo() }
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
		glCheckError( context, "ContextInitialisation" );
		m_physicalDevices.emplace_back( VkPhysicalDevice( new PhysicalDevice{ VkInstance( this ) } ) );
		doCheckEnabledExtensions( ashes::makeArrayView( createInfo.ppEnabledExtensionNames, createInfo.enabledExtensionCount ) );
	}

	Instance::~Instance()
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
		return doHasEnabledExtensions( ashes::makeArrayView( &version, 1u ) );
	}

	void Instance::unregisterDevice( VkDevice device )
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

	void Instance::unregisterSurface( VkSurfaceKHR surface )
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
				get( device )->unlink( surface );
			}
		}
	}

	ContextPtr Instance::registerSurface( VkSurfaceKHR surface )
	{
		if ( m_context )
		{
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
		result[0] = ( float( 2 ) * zNear ) / ( right - left ),
		result[5] = ( float( 2 ) * zNear ) / ( top - bottom );
		result[8] = ( right + left ) / ( right - left );
		result[9] = ( top + bottom ) / ( top - bottom );
		result[10] = ( zNear + zFar ) / ( zNear - zFar );
		result[11] = float( -1 );
		result[13] = ( float( -2 ) * zFar * zNear ) / ( zFar - zNear );

		return result;
	}

	std::array< float, 16 > Instance::perspective( float radiansFovY
		, float aspect
		, float zNear
		, float zFar )const
	{
		// OpenGL right handed (cf. https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml)
		std::array< float, 16 > result{ 0.0f };
		float const tanHalfFovy = tan( radiansFovY / float( 2 ) );
		result[0] = float( 1 / ( tanHalfFovy * aspect ) );
		result[5] = float( 1 / tanHalfFovy );
		result[10] = float( -( zFar + zNear ) / ( zFar - zNear ) );
		result[11] = float( -1 );
		result[14] = float( -2 * zFar * zNear / ( zFar - zNear ) );

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
		result[0] = float( 2 ) / ( right - left );
		result[5] = float( 2 ) / ( top - bottom );
		result[10] = float( -2 ) / ( zFar - zNear );
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
		float const range = tan( radiansFovY / float( 2 ) ) * zNear;
		float const left = -range * aspect;
		float const right = range * aspect;
		float const bottom = -range;
		float const top = range;

		std::array< float, 16 > result{ 0.0f };
		result[0] = ( float( 2 ) * zNear ) / ( right - left );
		result[5] = ( float( 2 ) * zNear ) / ( top - bottom );
		result[10] = -float( 1 );
		result[11] = -float( 1 );
		result[14] = -float( 2 ) * zNear;
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
		, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const
	{
		for ( auto & callback : m_debugMessengers )
		{
			get( callback.debugMessenger )->submit( messageSeverity
				, messageTypes
				, callbackData
				, callback.userParam );
		}

		for ( auto & callback : m_debugAMDMessengers )
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
		, const char * pMessage )
	{
		for ( auto & callback : m_debugCallbacks )
		{
			get( callback.debugReport )->report( flags
				, objectType
				, object
				, location
				, messageCode
				, pLayerPrefix
				, pMessage );
		}

		for ( auto & callback : m_debugAMDCallbacks )
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

	VkPhysicalDeviceMemoryProperties const & Instance::getMemoryProperties()
	{
		static VkPhysicalDeviceMemoryProperties const memoryProperties = []()
		{
			VkPhysicalDeviceMemoryProperties result{};
			// Emulate one device local heap
			result.memoryHeaps[result.memoryHeapCount++] = { ~( 0ull ), VK_MEMORY_HEAP_DEVICE_LOCAL_BIT };
			// and one host visible heap
			result.memoryHeaps[result.memoryHeapCount++] = { ~( 0ull ), 0u };

			// Emulate all combinations of device local memory types
			// and all combinations of host visible memory types
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, 1u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 1u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, 1u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT, 0u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_PROTECTED_BIT, 0u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD, 0u };
			result.memoryTypes[result.memoryTypeCount++] = { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD, 0u };

			return result;
		}( );
		return memoryProperties;
	}

	VkPhysicalDeviceMemoryProperties2KHR const & Instance::getMemoryProperties2()
	{
		static VkPhysicalDeviceMemoryProperties2KHR const memoryProperties2 = []()
		{
			VkPhysicalDeviceMemoryProperties2KHR result{};
			result.memoryProperties = Instance::getMemoryProperties();
			return result;
		}( );
		return memoryProperties2;
	}
}
