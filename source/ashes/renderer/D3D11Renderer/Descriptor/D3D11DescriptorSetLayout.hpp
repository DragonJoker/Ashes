/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_DescriptorSetLayout_HPP___
#define ___D3D11Renderer_DescriptorSetLayout_HPP___
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
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
