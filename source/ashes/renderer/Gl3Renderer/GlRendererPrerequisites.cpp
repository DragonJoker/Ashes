#include "GlRendererPrerequisites.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
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
	using gl3::get;

	uint32_t getScreenIndex( VkDisplayModeKHR displayMode )
	{
		return get( get( displayMode )->getDisplay() )->getScreenIndex();
	}

	VkDisplayModeParametersKHR getDisplayModeParameters( VkDisplayModeKHR displayMode )
	{
		return get( displayMode )->getParameters();
	}

	VkExtent2D getDisplayResolution( VkDisplayModeKHR displayMode )
	{
		return get( get( displayMode )->getDisplay() )->getResolution();
	}
}