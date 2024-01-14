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

		VkResult bindMemory( VkDeviceMemory memory
			, VkDeviceSize memoryOffset )noexcept;

		VkMemoryRequirements getMemoryRequirements()const;
		bool isMapped()const;
		void copyFrom( VkBuffer src
			, VkDeviceSize srcOffset
			, VkDeviceSize srcSize
			, VkDeviceSize dstOffset )const;

		void setDebugName( std::string name )noexcept
		{
			m_debugName = std::move( name );
		}

		VkDeviceMemory getMemory()const noexcept
		{
			assert( m_memory != nullptr );
			return m_memory;
		}

		VkDeviceSize getMemoryOffset()const noexcept
		{
			return m_memoryOffset;
		}

		ObjectMemory const & getObjectMemory()const noexcept
		{
			assert( m_objectMemory != nullptr );
			return *m_objectMemory;
		}

		VkBufferUsageFlags getUsage()const noexcept
		{
			return m_createInfo.usage;
		}

		VkDeviceSize getSize()const noexcept
		{
			return m_createInfo.size;
		}

	private:
		VkDevice m_device;
		VkBufferCreateInfo m_createInfo;
		VkDeviceMemory m_memory{};
		VkDeviceSize m_memoryOffset{};
		ObjectMemory * m_objectMemory{};
		std::string m_debugName;
	};
}

#endif
