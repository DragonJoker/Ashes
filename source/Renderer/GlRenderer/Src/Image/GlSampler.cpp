#include "Image/GlSampler.hpp"

#include "Core/GlDevice.hpp"

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
		glLogCall( glGenSamplers, 1, &m_sampler );
		glLogCall( glBindSampler, 0u, m_sampler );
		glLogCall( glSamplerParameteri, m_sampler, GL_TEXTURE_MIN_FILTER, convert( minFilter, mipFilter ) );
		glLogCall( glSamplerParameteri, m_sampler, GL_TEXTURE_MAG_FILTER, convert( magFilter ) );
		glLogCall( glSamplerParameteri, m_sampler, GL_TEXTURE_WRAP_S, convert( wrapS ) );
		glLogCall( glSamplerParameteri, m_sampler, GL_TEXTURE_WRAP_T, convert( wrapT ) );
		glLogCall( glSamplerParameteri, m_sampler, GL_TEXTURE_WRAP_R, convert( wrapR ) );
		glLogCall( glSamplerParameterf, m_sampler, GL_TEXTURE_MIN_LOD, minLod );
		glLogCall( glSamplerParameterf, m_sampler, GL_TEXTURE_MAX_LOD, maxLod );

		if ( compareOp != renderer::CompareOp::eAlways )
		{
			glLogCall( glSamplerParameteri, m_sampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
			glLogCall( glSamplerParameteri, m_sampler, GL_TEXTURE_COMPARE_FUNC, convert( compareOp ) );
		}

		float fvalues[4] = { 0.0f };
		int ivalues[4] = { 0 };

		switch ( borderColour )
		{
		case renderer::BorderColour::eFloatTransparentBlack:
			glLogCall( glSamplerParameterfv, m_sampler, GL_TEXTURE_BORDER_COLOR, fvalues );
			break;

		case renderer::BorderColour::eIntTransparentBlack:
			glLogCall( glSamplerParameteriv, m_sampler, GL_TEXTURE_BORDER_COLOR, ivalues );
			break;

		case renderer::BorderColour::eFloatOpaqueBlack:
			fvalues[3] = 1.0f;
			glLogCall( glSamplerParameterfv, m_sampler, GL_TEXTURE_BORDER_COLOR, fvalues );
			break;

		case renderer::BorderColour::eIntOpaqueBlack:
			ivalues[3] = 255;
			glLogCall( glSamplerParameteriv, m_sampler, GL_TEXTURE_BORDER_COLOR, ivalues );
			break;

		case renderer::BorderColour::eFloatOpaqueWhite:
			fvalues[0] = 1.0f;
			fvalues[1] = 1.0f;
			fvalues[2] = 1.0f;
			fvalues[3] = 1.0f;
			glLogCall( glSamplerParameterfv, m_sampler, GL_TEXTURE_BORDER_COLOR, fvalues );
			break;

		case renderer::BorderColour::eIntOpaqueWhite:
			ivalues[0] = 255;
			ivalues[1] = 255;
			ivalues[2] = 255;
			ivalues[3] = 255;
			glLogCall( glSamplerParameteriv, m_sampler, GL_TEXTURE_BORDER_COLOR, ivalues );
			break;
		}
	}

	Sampler::~Sampler()
	{
		glLogCall( glDeleteSamplers, 1, &m_sampler );
	}
}
