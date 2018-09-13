#include "Image/TestSampler.hpp"

#include "Core/TestDevice.hpp"

namespace test_renderer
{
	Sampler::Sampler( Device const & device
		, ashes::SamplerCreateInfo const & createInfo )
		: ashes::Sampler{ device, createInfo }
	{
	}

	Sampler::~Sampler()
	{
	}
}
