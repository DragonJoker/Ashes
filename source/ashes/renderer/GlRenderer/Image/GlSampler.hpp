/**
*\file
*	Sampler.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Sampler_HPP___
#define ___GlRenderer_Sampler_HPP___
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class Sampler
		: public IcdObject
	{
	public:
		Sampler( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkSamplerCreateInfo const & createInfo );
		~Sampler();

		inline float getLodBias()const
		{
			return m_lodBias;
		}

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		float m_lodBias;
	};
}

#endif
