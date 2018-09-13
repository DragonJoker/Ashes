#include "VkCreateRenderer.hpp"
#include "Core/VkRenderer.hpp"

extern "C"
{
	VkRenderer_API ashes::Renderer * createRenderer( ashes::Renderer::Configuration const & configuration )
	{
		return new vk_renderer::Renderer{ configuration };
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
