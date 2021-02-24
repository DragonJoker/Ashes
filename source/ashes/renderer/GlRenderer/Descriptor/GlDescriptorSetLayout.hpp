/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_DescriptorSetLayout_HPP___
#define ___GlRenderer_DescriptorSetLayout_HPP___
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class DescriptorSetLayout
		: public AutoIdIcdObject< SurfaceKHR >
	{
	public:
		DescriptorSetLayout( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkDescriptorSetLayoutCreateInfo createInfo );

		auto begin()const
		{
			return m_bindings.begin();
		}

		auto end()const
		{
			return m_bindings.end();
		}

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkDescriptorSetLayoutCreateInfo m_createInfo;
		VkDescriptorSetLayoutBindingArray m_bindings;
	};
}

#endif
