/**
*\file
*	Sampler.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Sampler_HPP___
#define ___GlRenderer_Sampler_HPP___
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	class Sampler
	{
	public:
		Sampler( VkDevice device
			, VkSamplerCreateInfo const & createInfo );
		~Sampler();

		inline GLuint getInternal()const noexcept
		{
			return m_sampler;
		}

		inline float getLodBias()const
		{
			return m_lodBias;
		}

	private:
		VkDevice m_device;
		GLuint m_sampler;
		float m_lodBias;
	};
}

#endif
