/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_DescriptorSetLayout_HPP___
#define ___GlRenderer_DescriptorSetLayout_HPP___
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
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
		VkDescriptorSetLayoutCreateFlags m_flags;
		VkDescriptorSetLayoutBindingArray m_bindings;
	};
}

#endif
