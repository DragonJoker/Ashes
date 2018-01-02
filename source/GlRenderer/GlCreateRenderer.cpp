#include "GlCreateRenderer.hpp"
#include "GlRenderer.hpp"

namespace gl_renderer
{
	GlRenderer_API renderer::RendererPtr createRenderer()
	{
		return std::make_unique< Renderer >();
	}
}
