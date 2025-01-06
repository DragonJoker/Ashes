/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___XBOXRenderer_DescriptorSetLayout_HPP___
#define ___XBOXRenderer_DescriptorSetLayout_HPP___
#pragma once

#include "renderer/XBoxRenderer/XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	class DescriptorSetLayout
	{
	public:
		DescriptorSetLayout( VkDevice device
			, VkDescriptorSetLayoutCreateInfo createInfo );

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

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkDescriptorSetLayoutBindingArray m_bindings;
		VkDescriptorSetLayoutCreateInfo m_createInfo;
	};
}

#endif
