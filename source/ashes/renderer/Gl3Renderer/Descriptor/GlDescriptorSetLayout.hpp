/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_DescriptorSetLayout_HPP___
#define ___GlRenderer_DescriptorSetLayout_HPP___
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	class DescriptorSetLayout
	{
	public:
		DescriptorSetLayout( VkDevice device
			, VkDescriptorSetLayoutCreateInfo createInfo );

		auto begin()const
		{
			return m_bindings.begin();
		}

		auto end()const
		{
			return m_bindings.end();
		}

	private:
		VkDescriptorSetLayoutCreateInfo m_createInfo;
		VkDescriptorSetLayoutBindingArray m_bindings;
	};
}

#endif
