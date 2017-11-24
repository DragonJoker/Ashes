#include "Sampler.hpp"

#include "Device.hpp"
#include "RenderingResources.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace renderer
{
	Sampler::Sampler( Device const & device
		, WrapMode wrapS
		, WrapMode wrapT
		, WrapMode wrapR
		, Filter minFilter
		, Filter magFilter )
		: m_sampler{ device.getDevice().createSampler( convert( minFilter )
			, convert( magFilter )
			, convert( MipmapMode::eNearest )
			, convert( wrapS )
			, convert( wrapT )
			, convert( wrapR ) ) }
	{
	}
}
