/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Image/Sampler.hpp"

#include "ashespp/Core/Device.hpp"

namespace ashes
{
	Sampler::Sampler( Device const & device
		, SamplerCreateInfo createInfo )
		: Sampler{ device, "Sampler", createInfo }
	{
	}

	Sampler::Sampler( Device const & device
		, std::string const & debugName
		, SamplerCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ createInfo }
	{
		DEBUG_DUMP( m_info );
		auto res = m_device.vkCreateSampler( m_device
			, &static_cast< VkSamplerCreateInfo const & >( m_createInfo )
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "Sampler creation" );
		registerObject( m_device, debugName, *this );
	}
	
	Sampler::~Sampler()
	{
		unregisterObject( m_device, *this );
		m_device.vkDestroySampler( m_device
			, m_internal
			, m_device.getAllocationCallbacks() );
	}
}
