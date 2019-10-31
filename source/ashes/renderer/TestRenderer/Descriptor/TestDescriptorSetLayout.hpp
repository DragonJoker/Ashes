/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_DescriptorSetLayout_HPP___
#define ___TestRenderer_DescriptorSetLayout_HPP___
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class DescriptorSetLayout
	{
	public:
		DescriptorSetLayout( VkDevice device
			, VkDescriptorSetLayoutCreateInfo createInfo );
		~DescriptorSetLayout();

		VkDescriptorSetLayoutBinding const & getBinding( uint32_t bindingPoint
			, uint32_t index )const;

		auto begin()const
		{
			return m_bindings.begin();
		}

		auto end()const
		{
			return m_bindings.end();
		}

	private:
		VkDevice m_device;
		VkDescriptorSetLayoutBindingArray m_bindings;
		VkDescriptorSetLayoutCreateInfo m_createInfo;
	};
}

#endif
