#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	namespace
	{
		char const * convert( std::string const & value )
		{
			return value.c_str();
		}

		std::vector< char const * > convert( ashes::StringArray const & values )
		{
			std::vector< char const * > result;

			for ( auto & value : values )
			{
				result.push_back( convert( value ) );
			}

			return result;
		}
	}

	VkDeviceCreateInfo convert( ashes::DeviceCreateInfo const & value
		, std::vector< VkDeviceQueueCreateInfo > & queueCreateInfos
		, std::vector< char const * > & enabledLayerNames
		, std::vector< char const * > & enabledExtensionNames
		, VkPhysicalDeviceFeatures & enabledFeatures )
	{
		queueCreateInfos = convert< VkDeviceQueueCreateInfo >( value.queueCreateInfos );
		enabledLayerNames = convert( value.enabledLayerNames );
		enabledExtensionNames = convert( value.enabledExtensionNames );
		enabledFeatures = convert( value.enabledFeatures );
		return VkDeviceCreateInfo
		{
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			nullptr,
			convert( value.flags ),
			static_cast< uint32_t >( queueCreateInfos.size() ),                      // queueCreateInfoCount
			queueCreateInfos.data(),                                                 // pQueueCreateInfos
			static_cast< uint32_t >( enabledLayerNames.size() ),                     // enabledLayerCount
			enabledLayerNames.empty() ? nullptr : enabledLayerNames.data(),          // ppEnabledLayerNames
			static_cast< uint32_t >( enabledExtensionNames.size() ),                 // enabledExtensionCount
			enabledExtensionNames.empty() ? nullptr : enabledExtensionNames.data(),  // ppEnabledExtensionNames
			&enabledFeatures                                                         // pEnabledFeatures
		};
	}
}
