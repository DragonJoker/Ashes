/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_Buffer_HPP___
#define ___D3D11Renderer_Buffer_HPP___
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class Buffer
	{
	public:
		Buffer( VkDevice device
			, VkBufferCreateInfo createInfo );
		~Buffer();
		VkMemoryRequirements getMemoryRequirements()const;

		inline ID3D11Buffer * getBuffer()const
		{
			return m_buffer;
		}

		inline ID3D11UnorderedAccessView * getUnorderedAccessView()const
		{
			return m_unorderedAccessView;
		}

	private:
		void doBindMemory();

	private:
		VkDevice m_device;
		VkBufferCreateInfo m_createInfo;
		VkDeviceMemory m_memory{ VK_NULL_HANDLE };
		ID3D11Buffer * m_buffer{ nullptr };
		ID3D11UnorderedAccessView * m_unorderedAccessView{ nullptr };
	};
}

#endif
