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
		: Sampler{ device, "Sampler", std::move( createInfo ) }
	{
	}

	Sampler::Sampler( Device const & device
		, VkSampler sampler
		, SamplerCreateInfo createInfo )
		: VkObject{ "Sampler" }
		, m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
		, m_internal{ sampler }
	{
	}
	
	Sampler::Sampler( Device const & device
		, std::string const & debugName
		, SamplerCreateInfo createInfo )
		: VkObject{ debugName }
		, m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		DEBUG_DUMP( m_info );
		auto res = m_device.vkCreateSampler( m_device
			, &static_cast< VkSamplerCreateInfo const & >( m_createInfo )
			, m_device.getAllocationCallbacks()
			, &m_ownInternal );
		m_internal = m_ownInternal;
		checkError( res, "Sampler creation" );
		registerObject( m_device, debugName, *this );
	}

	Sampler::Sampler( Device const & device
		, VkSamplerCreateInfo createInfo )
		: Sampler{ device, "Sampler", std::move( createInfo ) }
	{
	}

	Sampler::Sampler( Device const & device
		, std::string const & debugName
		, VkSamplerCreateInfo createInfo )
		: VkObject{ debugName }
		, m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		DEBUG_DUMP( m_info );
		auto res = m_device.vkCreateSampler( m_device
			, &static_cast< VkSamplerCreateInfo const & >( m_createInfo )
			, m_device.getAllocationCallbacks()
			, &m_ownInternal );
		m_internal = m_ownInternal;
		checkError( res, "Sampler creation" );
		registerObject( m_device, debugName, *this );
	}
	
	Sampler::~Sampler()noexcept
	{
		if ( m_ownInternal )
		{
			unregisterObject( m_device, *this );
			m_device.vkDestroySampler( m_device
				, m_ownInternal
				, m_device.getAllocationCallbacks() );
		}
	}
}
