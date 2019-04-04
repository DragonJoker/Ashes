/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Image/Sampler.hpp"

#include "AshesPP/Core/Device.hpp"

namespace ashes
{
	Sampler::Sampler( Device const & device
		, VkSamplerCreateInfo const & createInfo )
		: m_device{ device }
		, m_info{ createInfo }
	{
		DEBUG_DUMP( m_info );
		auto res = m_device.vkCreateSampler( m_device
			, &m_info
			, nullptr
			, &m_internal );
		checkError( res, "Sampler creation" );
		registerObject( m_device, "Sampler", this );
	}
	
	Sampler::~Sampler()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroySampler( m_device
			, m_internal
			, nullptr );
	}
}
