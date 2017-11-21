#include "Sampler.hpp"

#include "RenderingResources.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace renderer
{
	Sampler::Sampler( RenderingResources const & resources
		, WrapMode wrapS
		, WrapMode wrapT
		, Filter minFilter
		, Filter magFilter )
		: m_sampler{ resources.getDevice().createSampler( convert( minFilter )
			, convert( magFilter )
			, convert( MipmapMode::eNearest )
			, convert( wrapS )
			, convert( wrapT )
			, convert( WrapMode::eRepeat ) ) }
	{
	}
}
