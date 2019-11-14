/**
*\file
*	DescriptorSetPool.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_DescriptorPool_HPP___
#define ___GlRenderer_DescriptorPool_HPP___
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	class DescriptorPool
	{
	public:
		DescriptorPool( VkDevice device
			, VkDescriptorPoolCreateInfo createInfo );
		~DescriptorPool();

		void registerSet( VkDescriptorSet set );
		VkResult reset( VkDescriptorPoolResetFlags flags );
		VkResult free( VkDescriptorSetArray sets );

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkDescriptorPoolCreateFlags m_flags;
		uint32_t m_maxSets;
		VkDescriptorPoolSizeArray m_poolSizes;
		VkDescriptorSetArray m_sets;
	};
}

#endif
