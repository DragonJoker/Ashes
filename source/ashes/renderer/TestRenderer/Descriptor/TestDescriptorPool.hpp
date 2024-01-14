/**
*\file
*	DescriptorSetPool.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_DescriptorPool_HPP___
#define ___TestRenderer_DescriptorPool_HPP___
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

#include <vector>

namespace ashes::test
{
	class DescriptorPool
		: public NonCopyable
	{
	public:
		DescriptorPool( VkDevice device
			, VkDescriptorPoolCreateInfo createInfos );
		~DescriptorPool()noexcept;

		void registerSet( VkDescriptorSet set );
		VkResult reset( VkDescriptorPoolResetFlags flags );
		VkResult freeDescriptors( VkDescriptorSetArray const & sets );

		VkDevice getDevice()const noexcept
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkDescriptorPoolSizeArray m_poolSizes;
		VkDescriptorPoolCreateInfo m_createInfos;
		VkDescriptorSetArray m_sets;
		VkDescriptorSetArray m_allSets;
	};
}

#endif
