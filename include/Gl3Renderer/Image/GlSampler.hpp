/**
*\file
*	Sampler.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Sampler_HPP___
#define ___GlRenderer_Sampler_HPP___
#pragma once

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Image/Sampler.hpp>

namespace gl_renderer
{
	class Sampler
		: public ashes::Sampler
	{
	public:
		Sampler( Device const & device
			, ashes::SamplerCreateInfo const & createInfo );
		/**
		*\brief
		*	Destructeur.
		*/
		~Sampler();
		/**
		*\return
		*	L'échantillonneur OpenGL.
		*/
		inline GLuint getSampler()const noexcept
		{
			return m_sampler;
		}

	private:
		Device const & m_device;
		GLuint m_sampler;
	};
}

#endif
