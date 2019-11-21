#include "GlRendererPrerequisites.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	uint32_t deduceMemoryType( uint32_t typeBits
		, VkMemoryPropertyFlags requirements )
	{
		return ashes::deduceMemoryType( typeBits
			, requirements
			, Instance::getMemoryProperties() );
	}
}

namespace ashes::gl
{
	char const * const getDisplayName( VkDisplayModeKHR displayMode )
	{
		return gl4::get( gl4::get( displayMode )->getDisplay() )->getName();
	}

	uint32_t getRefreshRate( VkDisplayModeKHR displayMode )
	{
		return gl4::get( displayMode )->getRefreshRate();
	}
}
