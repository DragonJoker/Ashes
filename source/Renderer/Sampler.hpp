/**
*\file
*	Sampler.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_Sampler_HPP___
#define ___Renderer_Sampler_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/Sampler.hpp>

namespace renderer
{
	/**
	*\brief
	*	Un échantillonneur.
	*/
	class Sampler
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] resources
		*	Les ressources de rendu.
		*\param[in] wrapS, wrapT
		*	Les modes de wrap de texture.
		*\param[in] minFilter, magFilter
		*	Les filtres de minification et magnification.
		*/
		Sampler( RenderingResources const & resources
			, WrapMode wrapS = WrapMode::eClampToEdge
			, WrapMode wrapT = WrapMode::eClampToEdge
			, Filter minFilter = Filter::eLinear
			, Filter magFilter = Filter::eLinear );
		/**
		*\return
		*	L'échantillonneur vulkan.
		*/
		inline vk::Sampler getSampler()const noexcept
		{
			return *m_sampler;
		}

	private:
		//! L'échantillonneur.
		vk::SamplerPtr m_sampler;
	};
}

#endif
