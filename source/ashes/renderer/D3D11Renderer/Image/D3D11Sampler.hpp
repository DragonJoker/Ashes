/**
*\file
*	Sampler.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_Sampler_HPP___
#define ___D3D11Renderer_Sampler_HPP___
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
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
