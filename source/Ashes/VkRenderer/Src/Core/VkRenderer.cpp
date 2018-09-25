#include "Core/VkRenderer.hpp"

#include "Core/VkConnection.hpp"
#include "Core/VkDevice.hpp"
#include "Core/VkPhysicalDevice.hpp"
#include "Core/VkSwapChain.hpp"

#include <cmath>
#include <iomanip>
#include <locale>

namespace vk_renderer
{
#if ASHES_WIN32
	static char const * const VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
#elif ASHES_XCB
	static char const * const VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
#elif ASHES_XLIB
	static char const * const VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME = VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
#elif ASHES_ANDROID
	static char const * const VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME = VK_KHR_ANDROID_SURFACE_EXTENSION_NAME;
#elif ASHES_MIR
	static char const * const VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME = VK_KHR_MIR_SURFACE_EXTENSION_NAME;
#elif ASHES_WAYLAND
	static char const * const VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME = VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME;
#endif

	Renderer::Renderer( Configuration const & configuration )
		: ashes::Renderer{ ashes::ClipDirection::eTopDown, "vk", configuration }
#if defined( _WIN32 )
		, m_library{ "vulkan-1.dll" }
#elif defined( __linux__ )
		, m_library{ "libvulkan.so.1" }
#else
#	error Unsupported platform
#endif
	{
		m_features.hasTexBufferRange = true;
		m_features.hasImageTexture = true;
		m_features.hasBaseInstance = true;
		m_features.hasClearTexImage = true;
		m_features.hasComputeShaders = true;
		m_features.hasStorageBuffers = true;
		m_library.getFunction( "vkGetInstanceProcAddr", GetInstanceProcAddr );

		if ( !GetInstanceProcAddr )
		{
			throw std::runtime_error{ std::string{ "Couldn't load vkGetInstanceProcAddr" } };
		}

#define VK_LIB_GLOBAL_FUNCTION( fun ) fun = reinterpret_cast< PFN_##fun >( GetInstanceProcAddr( nullptr, #fun ) );
#include "Miscellaneous/VulkanFunctionsList.inl"

		doInitLayersProperties();
		doInitInstance();
		doEnumerateDevices();
	}

	Renderer::~Renderer()
	{
		if ( m_instance != VK_NULL_HANDLE )
		{
			cleanupDebugging( m_instance, *this, m_msgCallback );
			m_layers.clear();
			m_instanceExtensionNames.clear();
			m_instanceLayerNames.clear();
			m_gpus.clear();
			vkDestroyInstance( m_instance, nullptr );
			m_instance = VK_NULL_HANDLE;
		}

		DEBUG_WRITE( "VkRenderer.log" );
	}

	ashes::DevicePtr Renderer::createDevice( ashes::ConnectionPtr && connection )const
	{
		ashes::DevicePtr result;

		try
		{
			result = std::make_shared< Device >( *this, std::move( connection ) );
		}
		catch ( std::exception & exc )
		{
			ashes::Logger::logError( std::string{ "Could not initialise logical device:\n" } + exc.what() );
		}

		return result;
	}

	ashes::ConnectionPtr Renderer::createConnection( uint32_t deviceIndex
		, ashes::WindowHandle && handle )const
	{
		return std::make_unique< Connection >( *this
			, deviceIndex
			, std::move( handle ) );
	}

	std::array< float, 16 > Renderer::frustum( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		std::array< float, 16 > result{ 0.0f };
		result[0] = ( float( 2 ) * zNear ) / ( right - left );
		result[5] = ( float( 2 ) * zNear ) / ( top - bottom );
		result[8] = ( right + left ) / ( right - left );
		result[9] = ( top + bottom ) / ( top - bottom );
		result[11] = float( -1 );
		result[10] = zFar / ( zNear - zFar );
		result[14] = -( zFar * zNear ) / ( zFar - zNear );

		return result;
	}

	std::array< float, 16 > Renderer::perspective( float radiansFovY
		, float aspect
		, float zNear
		, float zFar )const
	{
		float const tanHalfFovy = tan( radiansFovY / float( 2 ) );

		std::array< float, 16 > result{ 0.0f };
		result[0] = float( 1 ) / ( aspect * tanHalfFovy );
		result[5] = float( 1 ) / ( tanHalfFovy );
		result[11] = -float( 1 );
		result[10] = zFar / ( zNear - zFar );
		result[14] = -( zFar * zNear ) / ( zFar - zNear );

		return result;
	}

	std::array< float, 16 > Renderer::ortho( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		std::array< float, 16 > result{ 0.0f };
		result[0] = float( 2 ) / ( right - left );
		result[5] = float( 2 ) / ( top - bottom );
		result[12] = -( right + left ) / ( right - left );
		result[13] = -( top + bottom ) / ( top - bottom );
		result[10] = -float( 1 ) / ( zFar - zNear );
		result[14] = -zNear / ( zFar - zNear );
		result[15] = 1.0f;

		return result;
	}

	void Renderer::completeLayerNames( std::vector< char const * > & names )const
	{
		for ( auto const & props : m_layers )
		{
			addOptionalValidationLayer( props.layerName
				, props.description
				, names );
		}
	}

	void Renderer::doInitLayersProperties()
	{
		VkResult res{ VK_SUCCESS };
		std::vector< VkLayerProperties > vkProperties;
		// Il est possible, bien que rare, que le nombre de couches
		// d'instance change. Par exemple, en installant quelque chose
		// qui ajoute de nouvelles couches que le loader utiliserait
		// entre la requête initiale pour le compte et la récupération
		// des VkLayerProperties. Le loader l'indique en retournant
		// VK_INCOMPLETE et en mettant à jour le compte.
		// Le compte va alors être mis à jour avec le nombre d'entrées
		// chargées dans le pointeur de données, dans le cas où
		// le nombre de couches a diminué ou est inférieur à la taille
		// donnée.
		do
		{
			uint32_t instanceLayerCount{ 0 };
			res = vkEnumerateInstanceLayerProperties( &instanceLayerCount, nullptr );
			checkError( res, "Instance layer properties enumeration" );

			if ( instanceLayerCount == 0 )
			{
				res = VK_SUCCESS;
			}
			else
			{
				vkProperties.resize( instanceLayerCount );
				res = vkEnumerateInstanceLayerProperties( &instanceLayerCount
					, vkProperties.data() );
			}
		}
		while ( res == VK_INCOMPLETE );

		doInitLayerExtensionProperties( m_globalLayer );

		// On récupère la liste d'extensions pour chaque couche de l'instance.
		for ( auto prop : vkProperties )
		{
			ashes::LayerProperties layerProperties{ convert( prop ) };
			doInitLayerExtensionProperties( layerProperties );
			m_layers.push_back( layerProperties );
		}

		checkError( res, "Layer properties retrieval" );
	}

	void Renderer::doInitLayerExtensionProperties( ashes::LayerProperties & layerProps )
	{
		VkResult res{ VK_SUCCESS };

		// Récupération des extensions supportées par la couche.
		do
		{
			uint32_t extensionCount{ 0 };
			res = vkEnumerateInstanceExtensionProperties( layerProps.layerName.c_str()
				, &extensionCount
				, nullptr );
			checkError( res, "Instance extension properties enumeration" );

			if ( extensionCount > 0 )
			{
				std::vector< VkExtensionProperties > extensions;
				extensions.resize( extensionCount );
				res = vkEnumerateInstanceExtensionProperties( layerProps.layerName.c_str()
					, &extensionCount
					, extensions.data() );
				layerProps.extensions = convert< ashes::ExtensionProperties >( extensions );
			}
		}
		while ( res == VK_INCOMPLETE );

		checkError( res, "Extensions properties retrieval" );
	}

	void Renderer::doInitInstance()
	{
		completeLayerNames( m_instanceLayerNames );
		m_instanceExtensionNames.push_back( VK_KHR_SURFACE_EXTENSION_NAME );
		m_instanceExtensionNames.push_back( VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME );
		addOptionalDebugReportLayer( m_instanceExtensionNames );
		checkExtensionsAvailability( m_globalLayer.extensions, m_instanceExtensionNames );
		uint32_t instanceVersion = VulkanVersion;

		if ( vkEnumerateInstanceVersion )
		{
			vkEnumerateInstanceVersion( &instanceVersion );
		}

		m_apiVersion = VK_MAKE_VERSION( VK_VERSION_MAJOR( instanceVersion )
			, VK_VERSION_MINOR( instanceVersion )
			, VK_VERSION_PATCH( VK_HEADER_VERSION ) );

		VkApplicationInfo appInfo
		{
			VK_STRUCTURE_TYPE_APPLICATION_INFO,
			nullptr,
			m_configuration.appName.c_str(),                                // pApplicationName
			VK_MAKE_VERSION( VersionMajor, VersionMinor, VersionBuild ),    // applicationVersion
			m_configuration.engineName.c_str(),                             // pEngineName
			VK_MAKE_VERSION( VersionMajor, VersionMinor, VersionBuild ),    // engineVersion
			VulkanVersion                                                   // apiVersion
		};

		VkInstanceCreateInfo instInfo
		{
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			nullptr,
			0,                                                                             // flags
			&appInfo,                                                                      // pApplicationInfo
			static_cast< uint32_t >( m_instanceLayerNames.size() ),                        // enabledLayerCount
			m_instanceLayerNames.empty() ? nullptr : m_instanceLayerNames.data(),          // ppEnabledLayerNames
			static_cast< uint32_t >( m_instanceExtensionNames.size() ),                    // enabledExtensionCount
			m_instanceExtensionNames.empty() ? nullptr : m_instanceExtensionNames.data(),  // ppEnabledExtensionNames
		};
		DEBUG_DUMP( instInfo );

		auto res = vkCreateInstance( &instInfo, nullptr, &m_instance );
		checkError( res, "Instance creation" );

#define VK_LIB_INSTANCE_FUNCTION( fun ) fun = reinterpret_cast< PFN_##fun >( getInstanceProcAddr( #fun ) );
#include "Miscellaneous/VulkanFunctionsList.inl"

		setupDebugging( m_instance
			, *this
			, m_msgCallback );
	}

	void Renderer::doEnumerateDevices()
	{
		// On récupère les GPU physiques.
		uint32_t gpuCount{ 0u };
		auto res = vkEnumeratePhysicalDevices( m_instance
			, &gpuCount
			, nullptr );
		checkError( res, "GPU enumeration" );
		std::vector< VkPhysicalDevice > gpus( gpuCount, VK_NULL_HANDLE );
		res = vkEnumeratePhysicalDevices( m_instance
			, &gpuCount
			, gpus.data() );
		checkError( res, "GPU enumeration" );

		if ( !gpuCount )
		{
			checkError( VK_ERROR_FEATURE_NOT_PRESENT, "Vulkan supporting GPU" );
		}

		// Et on les stocke dans des PhysicalDevice.
		m_gpus.reserve( gpuCount );

		for ( auto gpu : gpus )
		{
			m_gpus.push_back( std::make_unique< PhysicalDevice >( *this, gpu ) );
		}
	}

	PFN_vkVoidFunction Renderer::getInstanceProcAddr( char const * const name )
	{
		auto result = GetInstanceProcAddr( m_instance, name );

		if ( !result )
		{
			ashes::Logger::logError( std::string{ "Couldn't load function " } + name );
		}

		return result;
	}
}
