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
		, renderer::MipmapMode mipFilter
		, float minLod
		, float maxLod
		, float lodBias
		, renderer::BorderColour borderColour
		, float maxAnisotropy
		, renderer::CompareOp compareOp )
		: renderer::Sampler{ device
			, wrapS
			, wrapT
			, wrapR
			, minFilter
			, magFilter
			, mipFilter
			, minLod
			, maxLod
			, lodBias
			, borderColour
			, maxAnisotropy
			, compareOp }
	{
		glGenSamplers( 1, &m_sampler );
		glBindSampler( 0u, m_sampler );
		glSamplerParameteri( m_sampler, GL_TEXTURE_MIN_FILTER, convert( minFilter, mipFilter ) );
		glSamplerParameteri( m_sampler, GL_TEXTURE_MAG_FILTER, convert( magFilter ) );
		glSamplerParameteri( m_sampler, GL_TEXTURE_WRAP_S, convert( wrapS ) );
		glSamplerParameteri( m_sampler, GL_TEXTURE_WRAP_T, convert( wrapT ) );
		glSamplerParameteri( m_sampler, GL_TEXTURE_WRAP_R, convert( wrapR ) );
		glSamplerParameterf( m_sampler, GL_TEXTURE_MIN_LOD, minLod );
		glSamplerParameterf( m_sampler, GL_TEXTURE_MAX_LOD, maxLod );

		if ( compareOp != renderer::CompareOp::eAlways )
		{
			glSamplerParameteri( m_sampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
			glSamplerParameteri( m_sampler, GL_TEXTURE_COMPARE_FUNC, convert( compareOp ) );
		}

		float fvalues[4] = { 0.0f };
		int ivalues[4] = { 0 };

		switch ( borderColour )
		{
		case renderer::BorderColour::eFloatTransparentBlack:
			glSamplerParameterfv( m_sampler, GL_TEXTURE_BORDER_COLOR, fvalues );
			break;

		case renderer::BorderColour::eIntTransparentBlack:
			glSamplerParameteriv( m_sampler, GL_TEXTURE_BORDER_COLOR, ivalues );
			break;

		case renderer::BorderColour::eFloatOpaqueBlack:
			fvalues[3] = 1.0f;
			glSamplerParameterfv( m_sampler, GL_TEXTURE_BORDER_COLOR, fvalues );
			break;

		case renderer::BorderColour::eIntOpaqueBlack:
			ivalues[3] = 255;
			glSamplerParameteriv( m_sampler, GL_TEXTURE_BORDER_COLOR, ivalues );
			break;

		case renderer::BorderColour::eFloatOpaqueWhite:
			fvalues[0] = 1.0f;
			fvalues[1] = 1.0f;
			fvalues[2] = 1.0f;
			fvalues[3] = 1.0f;
			glSamplerParameterfv( m_sampler, GL_TEXTURE_BORDER_COLOR, fvalues );
			break;

		case renderer::BorderColour::eIntOpaqueWhite:
			ivalues[0] = 255;
			ivalues[1] = 255;
			ivalues[2] = 255;
			ivalues[3] = 255;
			glSamplerParameteriv( m_sampler, GL_TEXTURE_BORDER_COLOR, ivalues );
			break;
		}
	}

	Sampler::~Sampler()
	{
		glDeleteSamplers( 1, &m_sampler );
	}
}
