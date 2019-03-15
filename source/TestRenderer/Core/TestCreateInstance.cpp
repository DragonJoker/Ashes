#include "Core/TestCreateInstance.hpp"
#include "Core/TestInstance.hpp"

extern "C"
{
	TestRenderer_API ashes::Instance * createInstance( ashes::Instance::Configuration const & configuration )
	{
		return new test_renderer::Instance{ configuration };
	}

	TestRenderer_API char const * getShortName()
	{
		return "test";
	}

	TestRenderer_API char const * getFullName()
	{
		return "Test";
	}
}
