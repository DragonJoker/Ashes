/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Image/Sampler.hpp"

#include "Core/Device.hpp"

namespace renderer
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
