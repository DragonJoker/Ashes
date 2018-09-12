#include "VkCreateRenderer.hpp"
#include "Core/VkRenderer.hpp"

extern "C"
{
	VkRenderer_API renderer::Renderer * createRenderer( renderer::Renderer::Configuration const & configuration )
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
