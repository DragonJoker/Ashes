/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma GCC diagnostic ignored "-Wunused-private-field"

#include "util/UtilsInstance.hpp"

#include "util/UtilsDebug.hpp"

#include <ashes/common/Exception.hpp>

namespace utils
{
	namespace
	{
#if !defined( NDEBUG )
		bool constexpr LoadValidationLayers = true;
#else
		bool constexpr LoadValidationLayers = false;
#endif

		bool isValidationLayer( std::string const & name )
		{
			static std::set< std::string, std::less<> > const validNames
			{
				"VK_LAYER_KHRONOS_validation",
			};
			return validNames.find( name ) != validNames.end();
		}

		void addOptionalValidationLayer( std::string const & layer
			, ashes::StringArray & names )
		{
			if constexpr ( LoadValidationLayers )
			{
				if ( isValidationLayer( layer ) )
				{
					names.push_back( layer );
				}
			}
		}

		void addOptionalDebugReportLayer( ashes::StringArray & names )
		{
			if constexpr ( LoadValidationLayers )
			{
#	if VK_EXT_debug_utils
				names.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
#	elif VK_EXT_debug_utils
				names.emplace_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
				names.emplace_back( VK_EXT_DEBUG_MARKER_EXTENSION_NAME );
#	endif
			}
		}

		void checkExtensionsAvailability( std::vector< VkExtensionProperties > const & available
			, ashes::StringArray const & requested )
		{
			for ( auto const & name : requested )
			{
				if ( available.end() == std::find_if( available.begin()
					, available.end()
					, [&name]( VkExtensionProperties const & extension )
					{
						return extension.extensionName == name;
					} ) )
				{
					throw ashes::BaseException{ "Extension " + name + " is not supported." };
				}
			}
		}

		ashes::VkLayerPropertiesArray enumerateLayerProperties( PFN_vkEnumerateInstanceLayerProperties enumLayerProperties )
		{
			if ( !enumLayerProperties )
			{
				return {};
			}

			uint32_t count;
			std::vector< VkLayerProperties > result;
			VkResult res;

			do
			{
				res = enumLayerProperties( &count, nullptr );

				if ( count )
				{
					result.resize( count );
					res = enumLayerProperties( &count, result.data() );
				}
			}
			while ( res == VK_INCOMPLETE );

			if ( res != VK_SUCCESS )
			{
				throw ashes::Exception{ res, "Instance layers retrieval" };
			}

			return result;
		}

		ashes::VkExtensionPropertiesArray enumerateExtensionProperties( PFN_vkEnumerateInstanceExtensionProperties enumInstanceExtensionProperties
			, std::string const & layerName )
		{
			if ( !enumInstanceExtensionProperties )
			{
				return {};
			}

			uint32_t count;
			std::vector< VkExtensionProperties > result;
			VkResult res;

			do
			{
				res = enumInstanceExtensionProperties( layerName.empty() ? nullptr : layerName.c_str()
					, &count
					, nullptr );

				if ( count )
				{
					result.resize( count );
					res = enumInstanceExtensionProperties( layerName.empty() ? nullptr : layerName.c_str()
						, &count
						, result.data() );
				}
			}
			while ( res == VK_INCOMPLETE );

			if ( res != VK_SUCCESS )
			{
				throw ashes::Exception{ res, "Instance layer extensions retrieval" };
			}

			return result;
		}
	}

	Instance::Instance( ashes::RendererList const & rendererList
		, std::string const & name
		, ashes::ApplicationInfo applicationInfo )
	{
		auto plugin = rendererList.getSelectedPlugin();

		if ( !name.empty() )
		{
			plugin = rendererList.selectPlugin( name );
		}

#pragma warning( push )
#pragma warning( disable: 4191 )
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type-strict"
		PFN_vkEnumerateInstanceLayerProperties enumLayerProperties;
		enumLayerProperties = reinterpret_cast< PFN_vkEnumerateInstanceLayerProperties >( plugin.getInstanceProcAddr( nullptr,
			"vkEnumerateInstanceLayerProperties" ) );
		PFN_vkEnumerateInstanceExtensionProperties enumInstanceExtensionProperties;
		enumInstanceExtensionProperties = reinterpret_cast< PFN_vkEnumerateInstanceExtensionProperties >( plugin.getInstanceProcAddr( nullptr,
			"vkEnumerateInstanceExtensionProperties" ) );
#pragma clang diagnostic pop
#pragma warning( pop )

		m_layers = enumerateLayerProperties( enumLayerProperties );
		m_globalLayerExtensions = enumerateExtensionProperties( enumInstanceExtensionProperties
			, m_globalLayer.layerName );

		// On récupère la liste d'extensions pour chaque couche de l'instance.
		for ( auto const & layerProperties : m_layers )
		{
			m_layersExtensions.try_emplace( layerProperties.layerName
				, enumerateExtensionProperties( enumInstanceExtensionProperties
					, layerProperties.layerName ) );
		}

		completeLayerNames( m_layerNames );

		m_extensionNames.emplace_back( VK_KHR_SURFACE_EXTENSION_NAME );
#if defined( VK_USE_PLATFORM_ANDROID_KHR )
		m_extensionNames.emplace_back( VK_KHR_ANDROID_SURFACE_EXTENSION_NAME );
#endif
#if defined( VK_USE_PLATFORM_MIR_KHR )
		m_extensionNames.emplace_back( VK_KHR_MIR_SURFACE_EXTENSION_NAME );
#endif
#if defined( VK_USE_PLATFORM_FUCHSIA )
		m_extensionNames.emplace_back( VK_FUCHSIA_IMAGEPIPE_SURFACE_EXTENSION_NAME );
#endif
#if defined( VK_USE_PLATFORM_IOS_MVK )
		m_extensionNames.emplace_back( VK_MVK_IOS_SURFACE_EXTENSION_NAME );
#endif
#if defined( VK_USE_PLATFORM_MACOS_MVK )
		m_extensionNames.emplace_back( VK_MVK_MACOS_SURFACE_EXTENSION_NAME );
#endif
#if defined( VK_USE_PLATFORM_VI_NN )
		m_extensionNames.emplace_back( VK_NN_VI_SURFACE_EXTENSION_NAME );
#endif
#if defined( VK_USE_PLATFORM_XCB_KHR )
		m_extensionNames.emplace_back( VK_KHR_XCB_SURFACE_EXTENSION_NAME );
#endif
#if defined( VK_USE_PLATFORM_XLIB_KHR )
		m_extensionNames.emplace_back( VK_KHR_XLIB_SURFACE_EXTENSION_NAME );
#endif
#if defined( VK_USE_PLATFORM_WAYLAND_KHR )
		m_extensionNames.emplace_back( VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME );
#endif
#if defined( VK_USE_PLATFORM_WIN32_KHR )
		m_extensionNames.emplace_back( VK_KHR_WIN32_SURFACE_EXTENSION_NAME );
#endif
		addOptionalDebugReportLayer( m_extensionNames );
		checkExtensionsAvailability( m_globalLayerExtensions, m_extensionNames );
		ashes::InstanceCreateInfo createInfo
		{
			0u,
			std::move( applicationInfo ),
			m_layerNames,
			m_extensionNames,
		};
		m_instance = std::make_unique< ashes::Instance >( std::move( plugin )
			, nullptr
			, std::move( createInfo ) );

		if constexpr ( LoadValidationLayers )
		{
			m_debugCallback = setupDebugging( *m_instance, this );
		}

		m_gpus = m_instance->enumeratePhysicalDevices();
	}

	Instance::~Instance()noexcept
	{
		if constexpr ( LoadValidationLayers )
		{
#	if VK_EXT_debug_utils
			m_instance->vkDestroyDebugUtilsMessengerEXT( *m_instance, m_debugCallback, nullptr );
#	elif VK_EXT_debug_report
			m_instance->vkDestroyDebugReportCallbackEXT( *m_instance, m_debugCallback, nullptr );
#	endif
		}
	}

	void Instance::completeLayerNames( ashes::StringArray & names )const
	{
		for ( auto const & props : m_layers )
		{
			addOptionalValidationLayer( props.layerName, names );
		}
	}
}
