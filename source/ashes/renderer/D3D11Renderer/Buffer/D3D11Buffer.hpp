/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_Buffer_HPP___
#define ___D3D11Renderer_Buffer_HPP___
#pragma once

#include "renderer/D3D11Renderer/Miscellaneous/D3D11DeviceMemory.hpp"

namespace ashes::d3d11
{
	class Buffer
	{
	public:
		Buffer( VkDevice device
			, VkBufferCreateInfo createInfo );
		~Buffer();
		VkResult bindMemory( VkDeviceMemory memory
			, VkDeviceSize memoryOffset );

		VkMemoryRequirements getMemoryRequirements()const;
		bool isMapped()const;
		void copyFrom( ID3D11DeviceContext * context
			, VkBuffer src
			, D3D11_BOX const & srcBox
			, UINT dstOffset )const;

		inline VkDeviceMemory getMemory()const
		{
			assert( m_memory != VK_NULL_HANDLE );
			return m_memory;
		}

		inline VkDeviceSize getMemoryOffset()const
		{
			return m_memoryOffset;
		}

		inline ObjectMemory const & getObjectMemory()const
		{
			assert( m_objectMemory != nullptr );
			return *m_objectMemory;
		}

		inline ID3D11UnorderedAccessView * getUnorderedAccessView()const
		{
			return m_unorderedAccessView;
		}

		inline ID3D11Resource * getResource()const
		{
			return getObjectMemory().resource;
		}

		inline ID3D11Buffer * getBuffer()const
		{
			return reinterpret_cast< ID3D11Buffer * >( getResource() );
		}

		inline VkBufferUsageFlags getUsage()const
		{
			return m_createInfo.usage;
		}

		inline VkDeviceSize getSize()const
		{
			return m_createInfo.size;
		}

	private:
	private:
		VkDevice m_device;
		VkBufferCreateInfo m_createInfo;
		VkDeviceMemory m_memory{ VK_NULL_HANDLE };
		VkDeviceSize m_memoryOffset{ 0u };
		ObjectMemory * m_objectMemory{ nullptr };
		ID3D11UnorderedAccessView * m_unorderedAccessView{ nullptr };
	};
}

#endif
