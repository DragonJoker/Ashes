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
	{
		glGenSamplers( 1, &m_sampler );
		glBindSampler( 0u, m_sampler );
		glSamplerParameteri( m_sampler, GL_TEXTURE_MIN_FILTER, convert( minFilter, mipFilter ) );
		glSamplerParameteri( m_sampler, GL_TEXTURE_MAG_FILTER, convert( magFilter ) );
		glSamplerParameteri( m_sampler, GL_TEXTURE_WRAP_S, convert( wrapS ) );
		glSamplerParameteri( m_sampler, GL_TEXTURE_WRAP_T, convert( wrapT ) );
		glSamplerParameteri( m_sampler, GL_TEXTURE_WRAP_R, convert( wrapR ) );
	}

	Sampler::~Sampler()
	{
		glDeleteSamplers( 1, &m_sampler );
	}
}
