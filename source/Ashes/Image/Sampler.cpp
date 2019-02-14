/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Image/Sampler.hpp"

#include "Ashes/Core/Device.hpp"

namespace ashes
{
	Sampler::Sampler( Device const & device
		, SamplerCreateInfo const & createInfo )
		: m_device{ device }
		, m_info{ createInfo }
	{
		registerObject( m_device, "Sampler", this );
	}
	
	Sampler::~Sampler()
	{
		unregisterObject( m_device, this );
	}
}
