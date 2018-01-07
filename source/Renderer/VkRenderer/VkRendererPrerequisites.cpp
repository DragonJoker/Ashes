#include "VkRendererPrerequisites.hpp"

#include <cstring>

namespace vk_renderer
{
	void checkExtensionsAvailability( std::vector< VkExtensionProperties > const & available
		, std::vector< char const * > const & requested )
	{
		for ( auto const & name : requested )
		{
			if ( available.end() == std::find_if( available.begin()
				, available.end()
				, [&name]( auto const & extension )
				{
					return strcmp( extension.extensionName, name ) == 0;
				} ) )
			{
				throw std::runtime_error{ "Extension " + std::string( name ) + " is not supported." };
			}
		}
	}

	bool isDepthStencilFormat( VkFormat format )
	{
		return format == VK_FORMAT_D16_UNORM_S8_UINT
			|| format == VK_FORMAT_D24_UNORM_S8_UINT
			|| format == VK_FORMAT_D32_SFLOAT_S8_UINT;
	}

	bool isStencilFormat( VkFormat format )
	{
		return format == VK_FORMAT_S8_UINT;
	}

	bool isDepthFormat( VkFormat format )
	{
		return format == VK_FORMAT_D16_UNORM
			|| format == VK_FORMAT_X8_D24_UNORM_PACK32
			|| format == VK_FORMAT_D32_SFLOAT;
	}
}
