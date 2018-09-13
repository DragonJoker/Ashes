#include "Core/GlCreateRenderer.hpp"
#include "Core/GlRenderer.hpp"

extern "C"
{
	Gl4Renderer_API ashes::Renderer * createRenderer( ashes::Renderer::Configuration const & configuration )
	{
		return new gl_renderer::Renderer{ configuration };
	}

	Gl4Renderer_API char const * getShortName()
	{
		return "gl4";
	}

	Gl4Renderer_API char const * getFullName()
	{
		return "OpenGL 4.X";
	}
}
