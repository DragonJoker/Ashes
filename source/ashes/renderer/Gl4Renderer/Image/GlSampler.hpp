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
		: public IcdObject
	{
	public:
		Sampler( VkDevice device
			, VkSamplerCreateInfo const & createInfo );
		~Sampler();

	private:
		VkDevice m_device;
	};
}

#endif
