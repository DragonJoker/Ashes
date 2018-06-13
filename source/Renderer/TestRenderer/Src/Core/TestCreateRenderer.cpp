#include "TestCreateRenderer.hpp"
#include "Core/TestRenderer.hpp"

extern "C"
{
	TestRenderer_API renderer::Renderer * createRenderer( renderer::Renderer::Configuration const & configuration )
	{
		return new test_renderer::Renderer{ configuration };
	}
}
