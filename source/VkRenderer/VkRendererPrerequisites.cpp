#include "VkRendererPrerequisites.hpp"

#include <algorithm>
#include <cstring>

namespace vk_renderer
{
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
