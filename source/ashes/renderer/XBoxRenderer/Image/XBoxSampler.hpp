/**
*\file
*	Sampler.h
*\author
*	Sylvain Doremus
*/
#ifndef ___XBOXRenderer_Sampler_HPP___
#define ___XBOXRenderer_Sampler_HPP___
#pragma once

#include "renderer/XBoxRenderer/XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	class Sampler
		: public NonCopyable
	{
	public:
		Sampler( VkDevice device
			, VkSamplerCreateInfo createInfo );
		~Sampler()noexcept;

		ID3D11SamplerState * getSampler()const
		{
			return m_sampler;
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkSamplerCreateInfo m_createInfo;
		ID3D11SamplerState * m_sampler;
	};
}

#endif
