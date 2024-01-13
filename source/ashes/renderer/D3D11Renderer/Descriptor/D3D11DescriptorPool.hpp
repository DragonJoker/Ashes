/**
*\file
*	DescriptorSetPool.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_DescriptorPool_HPP___
#define ___D3D11Renderer_DescriptorPool_HPP___
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <vector>

namespace ashes::d3d11
{
	class DescriptorPool
		: public NonCopyable
	{
	public:
		DescriptorPool( VkDevice device
			, VkDescriptorPoolCreateInfo createInfos );
		~DescriptorPool()noexcept;

		void registerSet( VkDescriptorSet set );
		VkResult reset();
		VkResult freeDescriptors( ArrayView< VkDescriptorSet const > sets );

		inline VkDevice getDevice()const
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
