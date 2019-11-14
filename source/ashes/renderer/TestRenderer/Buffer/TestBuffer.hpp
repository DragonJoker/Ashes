/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_Buffer_HPP___
#define ___TestRenderer_Buffer_HPP___
#pragma once

#include "renderer/TestRenderer/Miscellaneous/TestDeviceMemory.hpp"

namespace ashes::test
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
		void copyFrom( VkBuffer src
			, VkDeviceSize srcOffset
			, VkDeviceSize srcSize
			, VkDeviceSize dstOffset )const;

		inline void setDebugName( std::string name )
		{
			m_debugName = std::move( name );
		}

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
		std::string m_debugName;
	};
}

#endif
