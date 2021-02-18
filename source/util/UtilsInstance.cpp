/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "util/UtilsInstance.hpp"

#include "util/UtilsDebug.hpp"

#include <ashes/common/Exception.hpp>

#if !defined( NDEBUG )
#	define LOAD_VALIDATION_LAYERS 1
#else
#	define LOAD_VALIDATION_LAYERS 0
#endif

namespace utils
{
	namespace
	{
		bool isValidationLayer( std::string const & name )
		{
			static std::set< std::string > const validNames
			{
				"VK_LAYER_KHRONOS_validation",
			};
			return validNames.find( name ) != validNames.end();
		}

		void addOptionalValidationLayer( std::string const & layer
			, std::string description
			, ashes::StringArray & names )
		{
#if LOAD_VALIDATION_LAYERS
			if ( isValidationLayer( layer ) )
			{
				names.push_back( layer );
			}
#endif
		}

		void addOptionalDebugReportLayer( ashes::StringArray & names )
		{
#if LOAD_VALIDATION_LAYERS
#	if VK_EXT_debug_utils
			names.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
#	elif VK_EXT_debug_utils
			names.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
			names.push_back( VK_EXT_DEBUG_MARKER_EXTENSION_NAME );
#	endif
#endif
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
					throw std::runtime_error{ "Extension " + name + " is not supported." };
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

		PFN_vkEnumerateInstanceLayerProperties enumLayerProperties;
		enumLayerProperties = ( PFN_vkEnumerateInstanceLayerProperties )plugin.getInstanceProcAddr( VK_NULL_HANDLE,
			"vkEnumerateInstanceLayerProperties" );
		PFN_vkEnumerateInstanceExtensionProperties enumInstanceExtensionProperties;
		enumInstanceExtensionProperties = ( PFN_vkEnumerateInstanceExtensionProperties )plugin.getInstanceProcAddr( VK_NULL_HANDLE,
			"vkEnumerateInstanceExtensionProperties" );

		m_layers = enumerateLayerProperties( enumLayerProperties );
		m_globalLayerExtensions = enumerateExtensionProperties( enumInstanceExtensionProperties
			, m_globalLayer.layerName );

		// On récupère la liste d'extensions pour chaque couche de l'instance.
		for ( auto layerProperties : m_layers )
		{
			m_layersExtensions.emplace( layerProperties.layerName
				, enumerateExtensionProperties( enumInstanceExtensionProperties
					, layerProperties.layerName ) );
		}

		completeLayerNames( m_layerNames );

		m_extensionNames.push_back( VK_KHR_SURFACE_EXTENSION_NAME );
		m_extensionNames.push_back( ashes::KHR_PLATFORM_SURFACE_EXTENSION_NAME );
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

#if LOAD_VALIDATION_LAYERS
		m_debugCallback = setupDebugging( *m_instance
			, this );
#endif

		m_gpus = m_instance->enumeratePhysicalDevices();
	}

	Instance::~Instance()
	{
#if LOAD_VALIDATION_LAYERS
#	if VK_EXT_debug_utils
		m_instance->vkDestroyDebugUtilsMessengerEXT( *m_instance, m_debugCallback, nullptr );
#	elif VK_EXT_debug_report
		m_instance->vkDestroyDebugReportCallbackEXT( *m_instance, m_debugCallback, nullptr );
#	endif
#endif
	}

	void Instance::completeLayerNames( ashes::StringArray & names )const
	{
		for ( auto const & props : m_layers )
		{
			addOptionalValidationLayer( props.layerName
				, props.description
				, names );
		}
	}
}
