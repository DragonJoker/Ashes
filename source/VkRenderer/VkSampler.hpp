/**
*\file
*	Sampler.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_Sampler_HPP___
#define ___VkRenderer_Sampler_HPP___
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/Sampler.hpp>
#include <Renderer/Sampler.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Un échantillonneur.
	*/
	class Sampler
		: public renderer::Sampler
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] wrapS, wrapT
		*	Les modes de wrap de texture.
		*\param[in] minFilter, magFilter
		*	Les filtres de minification et magnification.
		*/
		Sampler( renderer::Device const & device
			, renderer::WrapMode wrapS
			, renderer::WrapMode wrapT
			, renderer::WrapMode wrapR
			, renderer::Filter minFilter
			, renderer::Filter magFilter
			, renderer::MipmapMode mipFilter );
		/**
		*\return
		*	L'échantillonneur vulkan.
		*/
		inline vk::Sampler const & getSampler()const noexcept
		{
			return *m_sampler;
		}

	private:
		//! L'échantillonneur.
		vk::SamplerPtr m_sampler;
	};
}

#endif
