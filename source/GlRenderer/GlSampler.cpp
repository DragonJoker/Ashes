#include "GlSampler.hpp"

#include "GlDevice.hpp"

namespace gl_renderer
{
	Sampler::Sampler( renderer::Device const & device
		, renderer::WrapMode wrapS
		, renderer::WrapMode wrapT
		, renderer::WrapMode wrapR
		, renderer::Filter minFilter
		, renderer::Filter magFilter
		, renderer::MipmapMode mipFilter )
		: renderer::Sampler{ device
			, wrapS
			, wrapT
			, wrapR
			, minFilter
			, magFilter
			, mipFilter }
		, m_sampler{ static_cast< Device const & >( device ).getDevice().createSampler( convert( minFilter )
			, convert( magFilter )
			, convert( mipFilter )
			, convert( wrapS )
			, convert( wrapT )
			, convert( wrapR ) ) }
	{
	}
}
