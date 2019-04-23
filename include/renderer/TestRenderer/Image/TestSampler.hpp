/**
*\file
*	Sampler.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Ashes/Image/Sampler.hpp>

namespace test_renderer
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
	};
}
