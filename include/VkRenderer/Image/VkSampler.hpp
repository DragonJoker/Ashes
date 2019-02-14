/**
*\file
*	Sampler.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_Sampler_HPP___
#define ___VkRenderer_Sampler_HPP___
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Image/Sampler.hpp>

namespace vk_renderer
{
	class Sampler
		: public ashes::Sampler
	{
	public:
		Sampler( Device const & device
			, ashes::SamplerCreateInfo const & createInfo );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~Sampler();
		/**
		*\~french
		*\brief
		*	Opérateur de conversion implicite vers VkSampler.
		*\~english
		*\brief
		*	VkSampler implicit cast operator.
		*/
		inline operator VkSampler const &( )const
		{
			return m_sampler;
		}

	private:
		Device const & m_device;
		VkSampler m_sampler{};
	};
}

#endif
