/**
*\file
*	Sampler.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Sampler_HPP___
#define ___GlRenderer_Sampler_HPP___
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	class Sampler
	{
	public:
		Sampler( VkDevice device
			, VkSamplerCreateInfo const & createInfo );
		/**
		*\brief
		*	Destructeur.
		*/
		~Sampler();
		/**
		*\return
		*	L'échantillonneur OpenGL.
		*/
		inline GLuint getInternal()const noexcept
		{
			return m_sampler;
		}

	private:
		VkDevice m_device;
		GLuint m_sampler;
	};
}

#endif
