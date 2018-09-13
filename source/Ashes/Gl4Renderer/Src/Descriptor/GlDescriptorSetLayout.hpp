/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_DescriptorSetLayout_HPP___
#define ___GlRenderer_DescriptorSetLayout_HPP___
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Descriptor/DescriptorSetLayout.hpp>

namespace gl_renderer
{
	class DescriptorSetLayout
		: public ashes::DescriptorSetLayout
	{
	public:
		DescriptorSetLayout( ashes::Device const & device
			, ashes::DescriptorSetLayoutBindingArray && bindings );
	};
}

#endif
