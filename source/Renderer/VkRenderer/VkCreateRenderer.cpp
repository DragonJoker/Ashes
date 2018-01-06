#include "VkCreateRenderer.hpp"
#include "VkRenderer.hpp"

extern "C"
{
	VkRenderer_API renderer::Renderer * createRenderer()
	{
		return new vk_renderer::Renderer;
	}
}
