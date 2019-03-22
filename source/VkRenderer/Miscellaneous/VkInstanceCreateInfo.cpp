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

	VkInstanceCreateInfo convert( ashes::InstanceCreateInfo const & value
		, VkApplicationInfo & applicationInfo
		, std::vector< char const * > & enabledLayerNames
		, std::vector< char const * > & enabledExtensionNames )
	{
		applicationInfo = convert( value.applicationInfo );
		enabledLayerNames = convert( value.enabledLayerNames );
		enabledExtensionNames = convert( value.enabledExtensionNames );
		return VkInstanceCreateInfo
		{
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			nullptr,
			convert( value.flags ),
			&applicationInfo,
			static_cast< uint32_t >( enabledLayerNames.size() ),
			enabledLayerNames.empty() ? nullptr : enabledLayerNames.data(),
			static_cast< uint32_t >( enabledExtensionNames.size() ),
			enabledExtensionNames.empty() ? nullptr : enabledExtensionNames.data(),
		};
	}
}
