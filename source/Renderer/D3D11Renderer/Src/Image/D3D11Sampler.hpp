/**
*\file
*	Sampler.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_Sampler_HPP___
#define ___D3D11Renderer_Sampler_HPP___
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Image/Sampler.hpp>

namespace d3d11_renderer
{
	class Sampler
		: public renderer::Sampler
	{
	public:
		Sampler( Device const & device
			, renderer::SamplerCreateInfo const & createInfo );
		~Sampler();

		inline ID3D11SamplerState * getSampler()const
		{
			return m_sampler;
		}

	private:
		ID3D11SamplerState * m_sampler;
	};
}

#endif
