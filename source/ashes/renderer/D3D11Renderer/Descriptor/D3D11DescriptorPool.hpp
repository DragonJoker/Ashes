/**
*\file
*	DescriptorSetPool.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_DescriptorPool_HPP___
#define ___D3D11Renderer_DescriptorPool_HPP___
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Descriptor/DescriptorPool.hpp>

#include <vector>

namespace ashes::d3d11
{
	class DescriptorPool
		: public ashes::DescriptorPool
	{
	public:
		DescriptorPool( VkDevice device
			, VkDescriptorPoolCreateFlags flags
			, uint32_t maxSets
			, ashes::VkDescriptorPoolSizeArray poolSizes );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~DescriptorPool();
		/**
		*\copydoc	ashes::DescriptorSetPool::createDescriptorSet
		*/
		ashes::DescriptorSetPtr createDescriptorSet( ashes::DescriptorSetLayout const & layout
			, uint32_t bindingPoint )const;

	private:
		VkDevice m_device;
	};
}

#endif
