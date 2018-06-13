#include "Image/TestSampler.hpp"

#include "Core/TestDevice.hpp"

namespace test_renderer
{
	Sampler::Sampler( Device const & device
		, renderer::SamplerCreateInfo const & createInfo )
		: renderer::Sampler{ device, createInfo }
	{
	}

	Sampler::~Sampler()
	{
	}
}
