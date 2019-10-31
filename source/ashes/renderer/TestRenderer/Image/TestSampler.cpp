#include "Image/TestSampler.hpp"

#include "Core/TestDevice.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	Sampler::Sampler( VkDevice device
		, VkSamplerCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
	}
}
