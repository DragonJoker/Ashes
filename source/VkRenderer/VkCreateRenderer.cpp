#include "VkCreateRenderer.hpp"
#include "VkRenderer.hpp"

namespace vk_renderer
{
	VkRenderer_API renderer::RendererPtr createRenderer()
	{
		return std::make_unique< Renderer >();
	}
}
