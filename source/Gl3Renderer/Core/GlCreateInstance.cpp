#include "Core/GlCreateInstance.hpp"
#include "Core/GlInstance.hpp"

extern "C"
{
	Gl3Renderer_API ashes::Instance * createInstance( ashes::Instance::Configuration const & configuration )
	{
		return new gl_renderer::Instance{ configuration };
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
