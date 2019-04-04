/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_DescriptorSetLayout_HPP___
#define ___GlRenderer_DescriptorSetLayout_HPP___
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	class DescriptorSetLayout
	{
	public:
		DescriptorSetLayout( VkDevice device
			, VkDescriptorSetLayoutCreateInfo createInfo );
	};
}

#endif
