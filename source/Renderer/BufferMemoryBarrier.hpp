/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "Buffer.hpp"
#include "StagingBuffer.hpp"
#include "UniformBuffer.hpp"
#include "VertexBuffer.hpp"

#include <VkLib/VkLibPrerequisites.hpp>

namespace renderer
{
	/**
	*\brief
	*	Encapsulation d'un VkBufferMemoryBarrier.
	*/
	class BufferMemoryBarrier
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		template< typename T >
		BufferMemoryBarrier( AccessFlags srcAccessMask
			, AccessFlags dstAccessMask
			, uint32_t srcQueueFamilyIndex
			, uint32_t dstQueueFamilyIndex
			, Buffer< T > const & buffer
			, uint64_t offset
			, uint64_t size )
			: m_barrier
			{
				VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
				nullptr,
				srcAccessMask,
				dstAccessMask,
				srcQueueFamilyIndex,
				dstQueueFamilyIndex,
				buffer.getBuffer(),
				offset,
				size
			}
		{
		}
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		template< typename T >
		BufferMemoryBarrier( AccessFlags srcAccessMask
			, AccessFlags dstAccessMask
			, uint32_t srcQueueFamilyIndex
			, uint32_t dstQueueFamilyIndex
			, VertexBuffer< T > const & buffer
			, uint64_t offset
			, uint64_t size )
			: m_barrier
			{
				VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
				nullptr,
				srcAccessMask,
				dstAccessMask,
				srcQueueFamilyIndex,
				dstQueueFamilyIndex,
				buffer.getVbo(),
				offset,
				size
			}
		{
		}
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		template< typename T >
		BufferMemoryBarrier( AccessFlags srcAccessMask
			, AccessFlags dstAccessMask
			, uint32_t srcQueueFamilyIndex
			, uint32_t dstQueueFamilyIndex
			, UniformBuffer< T > const & buffer
			, uint64_t offset
			, uint64_t size )
			: m_barrier
			{
				VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
				nullptr,
				srcAccessMask,
				dstAccessMask,
				srcQueueFamilyIndex,
				dstQueueFamilyIndex,
				buffer.getUbo(),
				offset,
				size
			}
		{
		}
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		BufferMemoryBarrier( AccessFlags srcAccessMask
			, AccessFlags dstAccessMask
			, uint32_t srcQueueFamilyIndex
			, uint32_t dstQueueFamilyIndex
			, StagingBuffer const & buffer
			, uint64_t offset
			, uint64_t size )
			: m_barrier
			{
				VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
				nullptr,
				srcAccessMask,
				dstAccessMask,
				srcQueueFamilyIndex,
				dstQueueFamilyIndex,
				buffer.getStagingBuffer().getBuffer(),
				offset,
				size
			}
		{
		}
		/**
		*\return
		*	Le barrière mémoire vulkan.
		*/
		VkBufferMemoryBarrier const & getBarrier()const
		{
			return m_barrier;
		}

	private:
		VkBufferMemoryBarrier m_barrier;
	};
}
