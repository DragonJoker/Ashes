/**
*\file
*	DescriptorSetPool.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_DescriptorPool_HPP___
#define ___GlRenderer_DescriptorPool_HPP___
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

#include <unordered_set>

namespace ashes::gl
{
	class DescriptorPool
		: public AutoIdIcdObject< DescriptorPool >
	{
	public:
		DescriptorPool( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkDescriptorPoolCreateInfo const & createInfo );
		~DescriptorPool()noexcept;

		VkResult allocate( VkDescriptorSetLayout layout
			, VkDescriptorSet & set );
		VkResult reset();
		VkResult freeDescriptors( ArrayView< VkDescriptorSet const > const & sets );

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkDescriptorPoolCreateFlags m_flags;
		uint32_t m_maxSets;
		VkDescriptorPoolSizeArray m_poolSizes;
		std::unordered_set< VkDescriptorSet > m_sets;
		ByteArray m_memory;
		std::vector< VkDescriptorSet > m_free;
	};
}

#endif
