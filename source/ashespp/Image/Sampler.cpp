/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Image/Sampler.hpp"

#include "AshesPP/Core/Device.hpp"

namespace ashes
{
	Sampler::Sampler( Device const & device
		, SamplerCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ createInfo }
	{
		DEBUG_DUMP( m_info );
		auto res = m_device.vkCreateSampler( m_device
			, &static_cast< VkSamplerCreateInfo const & >( m_createInfo )
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
