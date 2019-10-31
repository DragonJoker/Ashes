/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_VertexBuffer_HPP___
#define ___AshesPP_VertexBuffer_HPP___
#pragma once

#include "ashespp/Buffer/Buffer.hpp"

#include <vector>

namespace ashes
{
	/**
	*\brief
	*	Base class for a vertex buffer.
	*/
	class VertexBufferBase
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] size
		*	The buffer size.
		*\param[in] usage
		*	The buffer usage flags.
		*\param[in] sharingMode
		*	The buffer sharing mode.
		*/
		VertexBufferBase( Device const & device
			, VkDeviceSize size
			, VkBufferUsageFlags usage
			, QueueShare sharingMode = {} );
		/**
		*\brief
		*	Destructor.
		*/
		~VertexBufferBase() = default;
		/**
		*\brief
		*	Binds this buffer to given device memory object.
		*\param[in] memory
		*	The memory object.
		*/
		void bindMemory( DeviceMemoryPtr memory );
		/**
		*\name
		*	Getters.
		**/
		/**@{*/
		VkMemoryRequirements getMemoryRequirements()const;

		inline VkDeviceSize getSize()const
		{
			return m_size;
		}

		inline BufferBase const & getBuffer()const
		{
			return *m_buffer;
		}

		inline BufferBase & getBuffer()
		{
			return *m_buffer;
		}

		inline Device const & getDevice()const
		{
			return m_device;
		}
		/**@}*/
		/**
		*\brief
		*	VkBuffer implicit cast operator.
		*/
		inline operator VkBuffer const & ()const
		{
			return *m_buffer;
		}

	protected:
		Device const & m_device;
		VkDeviceSize m_size;
		BufferBasePtr m_buffer;
	};
	/**
	*\brief
	*	Vertex buffer creation helper.
	*\param[in] device
	*	The logical device.
	*\param[in] size
	*	The buffer size.
	*\param[in] usage
	*	The buffer usage flags.
	*\param[in] sharingMode
	*	The buffer sharing mode.
	*\return
	*	The created buffer.
	*/
	inline VertexBufferBasePtr makeVertexBufferBase( Device const & device
		, VkDeviceSize size
		, VkBufferUsageFlags usage
		, QueueShare sharingMode = {} )
	{
		return std::make_unique< VertexBufferBase >( device
			, size
			, usage
			, std::move( sharingMode ) );
	}
	/**
	*\brief
	*	Typed vertex buffer.
	*/
	template< typename T >
	class VertexBuffer
		: public VertexBufferBase
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] count
		*	The buffer elements count.
		*\param[in] usage
		*	The buffer usage flags.
		*\param[in] sharingMode
		*	The buffer sharing mode.
		*/
		inline VertexBuffer( Device const & device
			, VkDeviceSize count
			, VkBufferUsageFlags usage
			, QueueShare sharingMode = {} );
		/**
		*\brief
		*	Binds this buffer to given device memory object.
		*\param[in] memory
		*	The memory object.
		*/
		inline void bindMemory( DeviceMemoryPtr memory )
		{
			m_buffer->bindMemory( std::move( memory ) );
		}
		/**
		*\brief
		*	Maps a range of the buffer's memory in RAM.
		*\param[in] offset
		*	The range elements beginning offset.
		*\param[in] count
		*	The range elements count.
		*\param[in] flags
		*	The mapping flags.
		*\return
		*	\p nullptr if mapping failed.
		*/
		inline T * lock( VkDeviceSize offset
			, VkDeviceSize count
			, VkMemoryMapFlags flags )const
		{
			auto size = doComputeSize( count, offset );
			return reinterpret_cast< T * >( m_buffer->lock( offset, size, flags ) );
		}
		/**
		*\brief
		*	Updates the VRAM.
		*\param[in] offset
		*	The mapped elements starting offset.
		*\param[in] count
		*	The range elements count.
		*/
		inline void flush( VkDeviceSize offset
			, VkDeviceSize count )const
		{
			auto size = doComputeSize( count, offset );
			m_buffer->flush( offset, size );
		}
		/**
		*\brief
		*	Invalidates the buffer content.
		*\param[in] offset
		*	The mapped elements starting offset.
		*\param[in] count
		*	The range elements count.
		*/
		inline void invalidate( VkDeviceSize offset
			, VkDeviceSize count )const
		{
			auto size = doComputeSize( count, offset );
			m_buffer->invalidate( offset, size );
		}
		/**
		*\brief
		*	Unmaps the buffer's memory from RAM.
		*/
		void unlock()const
		{
			m_buffer->unlock();
		}
		/**
		*\name
		*	Getters.
		**/
		/**@{*/
		inline VkMemoryRequirements getMemoryRequirements()const
		{
			return m_buffer->getMemoryRequirements();
		}

		inline VkDeviceSize getCount()const
		{
			return m_buffer->getSize() / sizeof( T );
		}
		/**@}*/
		/**
		*\brief
		*	VkBuffer implicit cast operator.
		*/
		inline operator VkBuffer const & ()const
		{
			return *m_buffer;
		}

	private:
		uint64_t doComputeSize( VkDeviceSize count
			, VkDeviceSize & offset )const
		{
			offset *= sizeof( T );
			count *= sizeof( T );
			auto const aligned = getAlignedSize( count
				, m_device.getProperties().limits.nonCoherentAtomSize );
			auto result = count == m_buffer->getSize()
				? ( offset == 0ull
					? WholeSize
					: aligned )
				: ( offset + count == m_buffer->getSize()
					? count
					: aligned );
			assert( result == WholeSize || offset + result <= m_buffer->getSize() );
			return result;
		}
	};
	/**
	*\brief
	*	Typed vertex buffer creation helper.
	*\param[in] device
	*	The logical device.
	*\param[in] count
	*	The buffer elements count.
	*\param[in] usage
	*	The buffer usage flags.
	*\param[in] sharingMode
	*	The buffer sharing mode.
	*\return
	*	The created buffer.
	*/
	template< typename T >
	inline VertexBufferPtr< T > makeVertexBuffer( Device const & device
		, VkDeviceSize count
		, VkBufferUsageFlags usage
		, QueueShare sharingMode = {} )
	{
		return std::make_unique< VertexBuffer< T > >( device
			, count
			, usage
			, std::move( sharingMode ) );
	}
}

#include "ashespp/Buffer/VertexBuffer.inl"

#endif
