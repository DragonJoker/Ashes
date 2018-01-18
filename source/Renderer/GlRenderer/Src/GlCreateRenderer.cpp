#include "GlCreateRenderer.hpp"
#include "GlRenderer.hpp"

extern "C"
{
	GlRenderer_API renderer::Renderer * createRenderer()
	{
		return new gl_renderer::Renderer;
	}
}
