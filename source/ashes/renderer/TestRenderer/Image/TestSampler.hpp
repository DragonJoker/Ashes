/**
*\file
*	Sampler.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_Sampler_HPP___
#define ___TestRenderer_Sampler_HPP___
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class Sampler
	{
	public:
		Sampler( VkDevice device
			, VkSamplerCreateInfo createInfo );

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkSamplerCreateInfo m_createInfo;
	};
}

#endif
