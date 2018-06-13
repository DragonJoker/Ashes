/**
*\file
*	Sampler.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_Sampler_HPP___
#define ___TestRenderer_Sampler_HPP___
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Image/Sampler.hpp>

namespace test_renderer
{
	class Sampler
		: public renderer::Sampler
	{
	public:
		Sampler( Device const & device
			, renderer::SamplerCreateInfo const & createInfo );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~Sampler();
	};
}

#endif
