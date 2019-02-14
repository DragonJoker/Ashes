/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_DescriptorSetLayout_HPP___
#define ___GlRenderer_DescriptorSetLayout_HPP___
#pragma once

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Descriptor/DescriptorSetLayout.hpp>

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
