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

		VkDevice getDevice()const noexcept
		{
			return m_device;
		}

		VkSamplerCreateInfo const & getCreateInfo()const noexcept
		{
			return m_createInfo;
		}

	private:
		VkDevice m_device;
		VkSamplerCreateInfo m_createInfo;
	};
}

#endif
