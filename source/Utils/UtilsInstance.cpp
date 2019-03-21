/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Utils/UtilsInstance.hpp"

#include "Utils/UtilsDebug.hpp"
#include "Utils/Factory.hpp"

namespace utils
{
	namespace
	{
		void addOptionalValidationLayer( std::string const & layer
			, std::string description
			, ashes::StringArray & names )
		{
#if LOAD_VALIDATION_LAYERS
			if ( layerName.find( "validation" ) != std::string::npos
				|| description.find( "LunarG Validation" ) != std::string::npos )
			{
				names.push_back( layerName );
			}
#endif
		}

		void addOptionalDebugReportLayer( ashes::StringArray & names )
		{
#if LOAD_VALIDATION_LAYERS
			names.push_back( ashes::EXT_DEBUG_REPORT_EXTENSION_NAME );
			//names.push_back( ashes::EXT_DEBUG_MARKER_EXTENSION_NAME );
#endif
		}

		void checkExtensionsAvailability( std::vector< ashes::ExtensionProperties > const & available
			, ashes::StringArray const & requested )
		{
			for ( auto const & name : requested )
			{
				if ( available.end() == std::find_if( available.begin()
					, available.end()
					, [&name]( ashes::ExtensionProperties const & extension )
					{
						return extension.extensionName == name;
					} ) )
				{
					throw std::runtime_error{ "Extension " + name + " is not supported." };
				}
			}
		}
	}

	Instance::Instance( InstanceFactory const & factory
		, std::string const & name
		, ashes::ApplicationInfo applicationInfo )
	{
		auto plugin = factory.findPlugin( name );
		m_layers = plugin->enumerateLayerProperties();
		m_globalLayerExtensions = plugin->enumerateExtensionProperties( m_globalLayer.layerName );

		// On récupère la liste d'extensions pour chaque couche de l'instance.
		for ( auto layerProperties : m_layers )
		{
			m_layersExtensions.emplace( layerProperties.layerName
				, plugin->enumerateExtensionProperties( layerProperties.layerName ) );
			m_layerNames.push_back( layerProperties.layerName );
		}

		completeLayerNames( m_layerNames );
		m_extensionNames.push_back( ashes::KHR_SURFACE_EXTENSION_NAME );
		m_extensionNames.push_back( ashes::KHR_PLATFORM_SURFACE_EXTENSION_NAME );
		addOptionalDebugReportLayer( m_extensionNames );

		checkExtensionsAvailability( m_globalLayerExtensions, m_extensionNames );

		ashes::InstanceCreateInfo createInfo;
		createInfo.enabledLayerNames = m_layerNames;
		createInfo.applicationInfo = std::move( applicationInfo );
		createInfo.enabledExtensionNames = m_extensionNames;

		m_instance = plugin->create( std::move( createInfo ) );

		m_debugCallback = setupDebugging( *m_instance
			, this );
		m_gpus = m_instance->enumeratePhysicalDevices();

		//uint32_t instanceVersion = enumerateVersion();
		//m_apiVersion = VK_MAKE_VERSION( VK_VERSION_MAJOR( instanceVersion )
		//	, VK_VERSION_MINOR( instanceVersion )
		//	, VK_VERSION_PATCH( VK_HEADER_VERSION ) );

//		// On récupère les extensions supportées par le GPU.
//		m_layerExtensions = enumerateLayerProperties();
//		m_extensions = enumerateExtensionProperties( std::string{} );
//
//#if VkRenderer_UsePhysicalDeviceExtensions
//
//		for ( auto & layer : m_layerExtensions )
//		{
//			layer.extensions = getLayerExtensions( m_instance
//				, m_gpu
//				, layer.layerName.c_str() );
//		}
//
//		m_instance.completeLayerNames( m_layerNames );
//		m_extensionNames.push_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
//		checkExtensionsAvailability( m_extensions, m_extensionNames );
//
//#else
//
//		for ( auto layer : m_instance.getLayers() )
//		{
//			layer.extensions = getLayerExtensions( m_instance
//				, m_gpu
//				, layer.layerName.c_str() );
//			m_layerExtensions.push_back( layer );
//		}
//
//#endif
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
