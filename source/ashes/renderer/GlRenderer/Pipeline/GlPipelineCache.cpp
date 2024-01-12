#include "Pipeline/GlPipelineCache.hpp"

#include "Core/GlDevice.hpp"
#include "Core/GlPhysicalDevice.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	PipelineCache::PipelineCache( [[maybe_unused]] VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, VkPipelineCacheCreateInfo createInfo )
		: m_header{ sizeof( Header )
			, 1u
			, get( get( device )->getPhysicalDevice() )->getProperties().vendorID
			, get( get( device )->getPhysicalDevice() )->getProperties().deviceID
			, {} }
		, m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		m_data.resize( sizeof( Header ) + m_createInfo.initialDataSize );
		auto buffer = m_data.data();
		std::memcpy( buffer, &m_header, sizeof( Header ) );
		buffer += sizeof( Header );

		if ( m_createInfo.pInitialData && m_createInfo.initialDataSize )
		{
			std::memcpy( buffer, m_createInfo.pInitialData, m_createInfo.initialDataSize );
		}

		registerObject( m_device, *this );
	}

	PipelineCache::~PipelineCache()noexcept
	{
		unregisterObject( m_device, *this );
	}

	VkResult PipelineCache::merge( [[maybe_unused]] ArrayView< VkPipelineCache const > pipelines )const
	{
		return VK_SUCCESS;
	}
}
