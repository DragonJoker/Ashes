#include "Core/GlCreateRenderer.hpp"
#include "Core/GlRenderer.hpp"

extern "C"
{
	Gl3Renderer_API renderer::Renderer * createRenderer( renderer::Renderer::Configuration const & configuration )
	{
		return new gl_renderer::Renderer{ configuration };
	}

	Gl3Renderer_API char const * getShortName()
	{
		return "gl3";
	}

	Gl3Renderer_API char const * getFullName()
	{
		return "OpenGL 3.X";
	}
}
