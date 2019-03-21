#include "Core/VkCreateInstance.hpp"
#include "Core/VkInstance.hpp"

#include <Ashes/Core/Exception.hpp>

struct VkLibrary
{
	std::unique_ptr< ashes::DynamicLibrary > library;
	PFN_vkGetInstanceProcAddr getInstanceProcAddr;
	PFN_vkEnumerateInstanceVersion enumerateInstanceVersion{ nullptr };
	PFN_vkEnumerateInstanceLayerProperties enumerateInstanceLayerProperties{ nullptr };
	PFN_vkEnumerateInstanceExtensionProperties enumerateInstanceExtensionProperties{ nullptr };

	ashes::Result init()
	{
		ashes::Result result = ashes::Result::eSuccess;

		if ( !enumerateInstanceLayerProperties
			|| !enumerateInstanceExtensionProperties )
		{
			result = ashes::Result::eErrorInitializationFailed;
#if defined( _WIN32 )
			library = std::make_unique< ashes::DynamicLibrary >( "vulkan-1.dll" );
#elif defined( __linux__ )
			library = std::make_unique< ashes::DynamicLibrary >( "libvulkan.so.1" );
#else
#	error Unsupported platform
#endif
			library->getFunction( "vkGetInstanceProcAddr", getInstanceProcAddr );

			if ( getInstanceProcAddr )
			{
				enumerateInstanceVersion = ( PFN_vkEnumerateInstanceVersion )getInstanceProcAddr( nullptr
					, "vkEnumerateInstanceVersion" );
				enumerateInstanceLayerProperties = ( PFN_vkEnumerateInstanceLayerProperties )getInstanceProcAddr( nullptr
					, "vkEnumerateInstanceLayerProperties" );
				enumerateInstanceExtensionProperties = ( PFN_vkEnumerateInstanceExtensionProperties )getInstanceProcAddr( nullptr
					, "vkEnumerateInstanceExtensionProperties" );

				if ( enumerateInstanceLayerProperties
					&& enumerateInstanceExtensionProperties )
				{
					result = ashes::Result::eSuccess;
				}
			}
		}

		return result;
	}
};

thread_local VkLibrary g_library;

extern "C"
{
	VkRenderer_API ashes::Result createInstance( ashes::InstanceCreateInfo createInfo
		, ashes::Instance ** instance )
	{
		ashes::Result result = g_library.init();

		if ( result == ashes::Result::eSuccess )
		{
			try
			{
				assert( instance );
				*instance = new vk_renderer::Instance{ std::move( g_library.library )
					, g_library.getInstanceProcAddr
					, std::move( createInfo ) };
			}
			catch ( ashes::Exception & exc )
			{
				result = exc.getResult();
			}
			catch ( ... )
			{
				result = ashes::Result::eErrorInitializationFailed;
			}
		}

		return result;
	}

	VkRenderer_API ashes::Result enumerateVersion( uint32_t * version )
	{
		ashes::Result result = g_library.init();

		if ( result == ashes::Result::eSuccess )
		{
			assert( version );
			*version = vk_renderer::VulkanVersion;

			if ( g_library.enumerateInstanceVersion )
			{
				result = ashes::Result( g_library.enumerateInstanceVersion( version ) );
			}
		}

		return result;
	}

	VkRenderer_API ashes::Result enumerateLayerProperties( uint32_t * count
		, ashes::LayerProperties * props )
	{
		ashes::Result result = g_library.init();

		if ( result == ashes::Result::eSuccess )
		{
			assert( count );

			if ( props )
			{
				std::vector< VkLayerProperties > properties;
				properties.resize( *count );
				result = ashes::Result( g_library.enumerateInstanceLayerProperties( count, properties.data() ) );

				if ( result == ashes::Result::eSuccess )
				{
					for ( auto & prop : properties )
					{
						*props = vk_renderer::convert( prop );
						++props;
					}
				}
			}
			else
			{
				result = ashes::Result( g_library.enumerateInstanceLayerProperties( count, nullptr ) );
			}
		}

		return result;
	}

	VkRenderer_API ashes::Result enumerateExtensionProperties( char const * const layerName
		, uint32_t * count
		, ashes::ExtensionProperties * props )
	{
		ashes::Result result = g_library.init();

		if ( result == ashes::Result::eSuccess )
		{
			assert( count );

			if ( props )
			{
				std::vector< VkExtensionProperties > properties;
				properties.resize( *count );
				result = ashes::Result( g_library.enumerateInstanceExtensionProperties( layerName
					, count
					, properties.data() ) );

				if ( result == ashes::Result::eSuccess )
				{
					for ( auto & prop : properties )
					{
						*props = vk_renderer::convert( prop );
						++props;
					}
				}
			}
			else
			{
				result = ashes::Result( g_library.enumerateInstanceExtensionProperties( layerName
					, count
					, nullptr ) );
			}
		}

		return result;
	}

	VkRenderer_API char const * getShortName()
	{
		return "vk";
	}

	VkRenderer_API char const * getFullName()
	{
		return "Vulkan";
	}
}
