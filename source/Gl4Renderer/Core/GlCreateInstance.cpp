#include "Core/GlCreateInstance.hpp"
#include "Core/GlInstance.hpp"

extern "C"
{
	Gl4Renderer_API ashes::Instance * createInstance( ashes::Instance::Configuration const & configuration )
	{
		return new gl_renderer::Instance{ configuration };
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
