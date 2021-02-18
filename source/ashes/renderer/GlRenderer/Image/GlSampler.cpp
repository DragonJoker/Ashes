#include "Image/GlSampler.hpp"

#include "Core/GlDevice.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	Sampler::Sampler( VkDevice device
		, VkSamplerCreateInfo const & createInfo )
		: m_device{ device }
		, m_lodBias{ createInfo.mipLodBias }
	{
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glGenSamplers
			, 1
			, &m_internal );
		glLogCall( context
			, glBindSampler
			, 0u
			, m_internal );
		glLogCall( context
			, glSamplerParameteri
			, m_internal
			, GL_SAMPLER_PARAMETER_MIN_FILTER
			, convert( createInfo.minFilter, createInfo.mipmapMode, createInfo.minLod, createInfo.maxLod ) );
		glLogCall( context
			, glSamplerParameteri
			, m_internal
			, GL_SAMPLER_PARAMETER_MAG_FILTER
			, convert( createInfo.magFilter ) );
		glLogCall( context
			, glSamplerParameteri
			, m_internal
			, GL_SAMPLER_PARAMETER_WRAP_S
			, convert( createInfo.addressModeU ) );
		glLogCall( context
			, glSamplerParameteri
			, m_internal
			, GL_SAMPLER_PARAMETER_WRAP_T
			, convert( createInfo.addressModeV ) );
		glLogCall( context
			, glSamplerParameteri
			, m_internal
			, GL_SAMPLER_PARAMETER_WRAP_R
			, convert( createInfo.addressModeW ) );
		glLogCall( context
			, glSamplerParameterf
			, m_internal
			, GL_SAMPLER_PARAMETER_MIN_LOD
			, createInfo.minLod );
		glLogCall( context
			, glSamplerParameterf
			, m_internal
			, GL_SAMPLER_PARAMETER_MAX_LOD
			, createInfo.maxLod );
		glLogCall( context
			, glSamplerParameterf
			, m_internal
			, GL_SAMPLER_PARAMETER_LOD_BIAS
			, createInfo.mipLodBias );

		if ( hasSamplerAnisotropy( device ) && createInfo.anisotropyEnable )
		{
			glLogCall( context
				, glSamplerParameterf
				, m_internal
				, GL_SAMPLER_PARAMETER_MAX_ANISOTROPY
				, createInfo.maxAnisotropy );
		}

		if ( createInfo.compareEnable )
		{
			glLogCall( context
				, glSamplerParameteri
				, m_internal
				, GL_SAMPLER_PARAMETER_COMPARE_MODE
				, GL_SAMPLER_PARAMETER_COMPARE_REF_TO_TEXTURE );
			glLogCall( context
				, glSamplerParameteri
				, m_internal
				, GL_SAMPLER_PARAMETER_COMPARE_FUNC
				, convert( createInfo.compareOp ) );
		}

		float fvalues[4] = { 0.0f };
		int ivalues[4] = { 0 };

		switch ( createInfo.borderColor )
		{
		case VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK:
			glLogCall( context
				, glSamplerParameterfv
				, m_internal
				, GL_SAMPLER_PARAMETER_BORDER_COLOR
				, fvalues );
			break;

		case VK_BORDER_COLOR_INT_TRANSPARENT_BLACK:
			glLogCall( context
				, glSamplerParameteriv
				, m_internal
				, GL_SAMPLER_PARAMETER_BORDER_COLOR
				, ivalues );
			break;

		case VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK:
			fvalues[3] = 1.0f;
			glLogCall( context
				, glSamplerParameterfv
				, m_internal
				, GL_SAMPLER_PARAMETER_BORDER_COLOR
				, fvalues );
			break;

		case VK_BORDER_COLOR_INT_OPAQUE_BLACK:
			ivalues[3] = 255;
			glLogCall( context
				, glSamplerParameteriv
				, m_internal
				, GL_SAMPLER_PARAMETER_BORDER_COLOR
				, ivalues );
			break;

		case VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE:
			fvalues[0] = 1.0f;
			fvalues[1] = 1.0f;
			fvalues[2] = 1.0f;
			fvalues[3] = 1.0f;
			glLogCall( context
				, glSamplerParameterfv
				, m_internal
				, GL_SAMPLER_PARAMETER_BORDER_COLOR
				, fvalues );
			break;

		case VK_BORDER_COLOR_INT_OPAQUE_WHITE:
			ivalues[0] = 255;
			ivalues[1] = 255;
			ivalues[2] = 255;
			ivalues[3] = 255;
			glLogCall( context
				, glSamplerParameteriv
				, m_internal
				, GL_SAMPLER_PARAMETER_BORDER_COLOR
				, ivalues );
			break;

		default:
			assert( false && "Unsupported VkBorderColor" );
			break;
		}
	}

	Sampler::~Sampler()
	{
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glDeleteSamplers
			, 1
			, &m_internal );
	}
}
