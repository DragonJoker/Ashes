#include "Core/VkCreateInstance.hpp"
#include "Core/VkInstance.hpp"

extern "C"
{
	VkRenderer_API ashes::Instance * createInstance( ashes::Instance::Configuration const & configuration )
	{
		return new vk_renderer::Instance{ configuration };
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
