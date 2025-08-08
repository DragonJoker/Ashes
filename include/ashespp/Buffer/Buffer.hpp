/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Buffer_HPP___
#define ___AshesPP_Buffer_HPP___
#pragma once

#include "ashespp/Core/Device.hpp"
#include "ashespp/Miscellaneous/DeviceMemory.hpp"
#include "ashespp/Miscellaneous/QueueShare.hpp"

#include <ashes/common/Exception.hpp>

namespace ashes
{
	inline VkBufferCreateInfo makeCreateInfo( VkDeviceSize size
		, VkBufferUsageFlags usage
		, QueueShare const & sharingMode = QueueShare{} )
	{
		return VkBufferCreateInfo
		{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			nullptr,
			0u,
			size,
			usage,
			sharingMode.sharingMode,
			sharingMode.queueFamilyIndexCount,
			sharingMode.pQueueFamilyIndices,
		};
	}
	/**
	*\brief
	*	GPU buffer class.
	*/
	class BufferBase
		: public VkObject
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] createInfo
		*	The creation informations.
		*/
		BufferBase( Device const & device
			, VkBufferCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] size
		*	The buffer size.
		*\param[in] usage
		*	The buffer usage flags.
		*/
		BufferBase( Device const & device
			, VkDeviceSize size
			, VkBufferUsageFlags usage
			, QueueShare const & sharingMode = {} );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] createInfo
		*	The creation informations.
		*/
		BufferBase( Device const & device
			, std::string const & debugName
			, VkBufferCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] size
		*	The buffer size.
		*\param[in] usage
		*	The buffer usage flags.
		*/
		BufferBase( Device const & device
			, std::string const & debugName
			, VkDeviceSize size
			, VkBufferUsageFlags usage
			, QueueShare const & sharingMode = {} );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] buffer
		*	The buffer.
		*/
		BufferBase( Device const & device
			, VkBuffer buffer
			, VkDeviceMemory storage );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] buffer
		*	The buffer.
		*/
		BufferBase( Device const & device
			, VkBuffer buffer
			, VkDeviceMemory storage
			, VkBufferCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] buffer
		*	The buffer.
		*/
		BufferBase( Device const & device
			, std::string const & debugName
			, VkBuffer buffer
			, VkDeviceMemory storage
			, VkBufferCreateInfo createInfo );
		/**
		*\brief
		*	Destructor.
		*/
		~BufferBase()noexcept;
		/**
		*\brief
		*	Binds this buffer to given device memory object.
		*\param[in] memory
		*	The memory object.
		*/
		void bindMemory( DeviceMemoryPtr memory );
		/**
		*\brief
		*	Maps a range of the buffer's memory in RAM.
		*\param[in] offset
		*	The range beginning offset.
		*\param[in] size
		*	The range size.
		*\param[in] flags
		*	The mapping flags.
		*\return
		*	\p nullptr if mapping failed.
		*/
		uint8_t * lock( VkDeviceSize offset
			, VkDeviceSize size
			, VkMemoryMapFlags flags )const;
		/**
		*\brief
		*	Invalidates the buffer content.
		*\param[in] offset
		*	The mapped memory starting offset.
		*\param[in] size
		*	The range size.
		*/
		void invalidate( VkDeviceSize offset
			, VkDeviceSize size )const;
		/**
		*\brief
		*	Updates the VRAM.
		*\param[in] offset
		*	The mapped memory starting offset.
		*\param[in] size
		*	The range size.
		*/
		void flush( VkDeviceSize offset
			, VkDeviceSize size )const;
		/**
		*\brief
		*	Unmaps the buffer's memory from RAM.
		*/
		void unlock()const;

#if VK_KHR_buffer_device_address

		/**
		*\return
		*	 An address of the buffer.
		*/
		VkDeviceAddress getDeviceAddress()const;
		/**
		*\return
		*	An opaque capture address of the buffer.
		*/
		uint64_t getOpaqueCaptureAddress()const;

#endif
		/**
		*\return
		*	The compatible pipeline stage flags for current access.
		*/
		VkPipelineStageFlags getCompatibleStageFlags()const;
		/**
		*\brief
		*	Prepares a buffer memory barrier, to a transfer destination layout.
		*\return
		*	The memory barrier.
		*/
		VkBufferMemoryBarrier makeTransferDestination( uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
		*\brief
		*	Prepares a buffer memory barrier, to a transfer source layout.
		*\return
		*	The memory barrier.
		*/
		VkBufferMemoryBarrier makeTransferSource( uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
		*\brief
		*	Prepares a buffer memory barrier, to a vertex shader input layout.
		*\return
		*	The memory barrier.
		*/
		VkBufferMemoryBarrier makeVertexShaderInputResource( uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
		*\brief
		*	Prepares a buffer memory barrier, to a uniform buffer input layout.
		*\return
		*	The memory barrier.
		*/
		VkBufferMemoryBarrier makeUniformBufferInput( uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
		*\brief
		*	Prepares a buffer memory barrier, to a host read layout.
		*\return
		*	The memory barrier.
		*/
		VkBufferMemoryBarrier makeHostRead( uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
		*\brief
		*	Prepares a buffer memory barrier, to a host write layout.
		*\return
		*	The memory barrier.
		*/
		VkBufferMemoryBarrier makeHostWrite( uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
		*\brief
		*	Prepares a buffer memory barrier, to a given memory layout.
		*\param[in] dstAccessFlags
		*	The wanted access flags, after the transition.
		*\param[in] srcQueueFamily
		*	The source queue family, if a responsibility transfer must happen.
		*\param[in] dstQueueFamily
		*	The destination queue family, if a responsibility transfer must happen.
		*\return
		*	The memory barrier.
		*/
		VkBufferMemoryBarrier makeMemoryTransitionBarrier( VkAccessFlags dstAccessFlags
			, uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
		*\brief
		*	Prepares a buffer memory barrier, to a given memory layout.
		*\param[in] dstAccessFlags
		*	The wanted access flags, after the transition.
		*\param[in] srcQueueFamily
		*	The source queue family, if a responsibility transfer must happen.
		*\param[in] dstQueueFamily
		*	The destination queue family, if a responsibility transfer must happen.
		*\return
		*	The memory barrier.
		*/
		VkBufferMemoryBarrier makeMemoryTransitionBarrier( VkAccessFlags dstAccessFlags
			, VkPipelineStageFlags dstStageFlags
			, uint32_t srcQueueFamily
			, uint32_t dstQueueFamily )const;
		/**
		*\return
		*	The memory requirements for this buffer.
		*/
		 VkMemoryRequirements getMemoryRequirements()const;
		/**
		*\return
		*	The buffer size.
		*/
		VkDeviceSize getSize()const
		{
			return m_createInfo.size;
		}
		/**
		*\return
		*	The buffer usage flags.
		*/
		VkBufferUsageFlags getUsage()const
		{
			return m_createInfo.usage;
		}
		/**
		*\return
		*	The buffer device memory.
		*/
		DeviceMemory const & getStorage()const
		{
			return *m_storage;
		}
		/**
		*\brief
		*	Conversion implicite vers VkBuffer.
		*/
		operator VkBuffer const & ()const
		{
			return m_internal;
		}
		/**
		*\brief
		*	Conversion implicite vers VkBufferCreateInfo.
		*/
		VkBufferCreateInfo const & operator->()const
		{
			return m_createInfo;
		}

	private:
		Device const & m_device;
		QueueShare m_sharingMode;
		VkBufferCreateInfo m_createInfo;
		VkBuffer m_internal{};
		DeviceMemoryPtr m_storage;
		bool m_ownInternal{ true };
		mutable VkAccessFlags m_currentAccessFlags{ VK_ACCESS_MEMORY_WRITE_BIT };
		mutable VkPipelineStageFlags m_compatibleStageFlags{ VK_PIPELINE_STAGE_HOST_BIT };
	};
	/**
	*\brief
	*	Helper function tor create a Buffer< T >.
	*\param[in] device
	*	The logical device.
	*\param[in] size
	*	The buffer size.
	*\param[in] usage
	*	The buffer usage flags.
	*\param[in] flags
	*	The buffer memory flags.
	*\return
	*	The created buffer.
	*/
	inline BufferBasePtr makeBufferBase( Device const & device
		, VkDeviceSize size
		, VkBufferUsageFlags usage
		, QueueShare sharingMode )
	{
		return std::make_unique< BufferBase >( device
			, size
			, usage
			, std::move( sharingMode ) );
	}
	/**
	*\brief
	*	Helper function tor create a Buffer< T >.
	*\param[in] device
	*	The logical device.
	*\param[in] size
	*	The buffer size.
	*\param[in] usage
	*	The buffer usage flags.
	*\param[in] flags
	*	The buffer memory flags.
	*\return
	*	The created buffer.
	*/
	inline BufferBasePtr makeBufferBase( Device const & device
		, std::string const & debugName
		, VkDeviceSize size
		, VkBufferUsageFlags usage
		, QueueShare sharingMode )
	{
		return std::make_unique< BufferBase >( device
			, debugName
			, size
			, usage
			, std::move( sharingMode ) );
	}
	/**
	*\brief
	*	Template class wrapping a GPU buffer, to store structured Data.
	*/
	template< typename T >
	class Buffer
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
		*/
		Buffer( Device const & device
			, VkDeviceSize count
			, VkBufferUsageFlags usage
			, QueueShare sharingMode = {} );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] count
		*	The buffer elements count.
		*\param[in] usage
		*	The buffer usage flags.
		*/
		Buffer( Device const & device
			, std::string const & debugName
			, VkDeviceSize count
			, VkBufferUsageFlags usage
			, QueueShare sharingMode = {} );
		/**
		*\return
		*	The memory requirements for this buffer.
		*/
		VkMemoryRequirements getMemoryRequirements()const
		{
			return m_buffer->getMemoryRequirements();
		}
		/**
		*\brief
		*	Binds this buffer to given device memory object.
		*\param[in] memory
		*	The memory object.
		*/
		void bindMemory( DeviceMemoryPtr memory )
		{
			m_buffer->bindMemory( std::move( memory ) );
		}
		/**
		*\return
		*	The elements count.
		*/
		VkDeviceSize getCount()const
		{
			return m_buffer->getSize() / sizeof( T );
		}
		/**
		*\return
		*	The GPU buffer.
		*/
		BufferBase const & getBuffer()const
		{
			return *m_buffer;
		}
		/**
		*\return
		*	The GPU buffer.
		*/
		BufferBase & getBuffer()
		{
			return *m_buffer;
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
		T * lock( VkDeviceSize offset
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
		void flush( VkDeviceSize offset
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
		void invalidate( VkDeviceSize offset
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
		*\brief
		*	Conversion implicite vers VkBuffer.
		*/
		operator VkBuffer const & ()const
		{
			return *m_buffer;
		}

	private:
		uint64_t doComputeSize( VkDeviceSize count, VkDeviceSize & offset )const
		{
			offset *= sizeof( T );

			if ( count != WholeSize )
			{
				count *= sizeof( T );
			}
			else
			{
				offset = 0u;
			}

			auto const aligned = getAlignedSize( count
				, m_device.getProperties().limits.nonCoherentAtomSize );
			auto result = count  == WholeSize
				? WholeSize
				: ( count == m_buffer->getSize()
					? ( offset == VkDeviceSize( 0 )
						? WholeSize
						: aligned )
					: ( offset + count == m_buffer->getSize()
						? count
						: aligned ) );
			assert( result == WholeSize || offset + result <= m_buffer->getSize() );
			return result;
		}

	private:
		Device const & m_device;
		BufferBasePtr m_buffer;
	};
	/**
	*\brief
	*	Helper function tor create a Buffer< T >.
	*\param[in] device
	*	The logical device.
	*\param[in] count
	*	The buffer elements count.
	*\param[in] usage
	*	The buffer usage flags.
	*\param[in] flags
	*	The buffer memory flags.
	*\return
	*	The created buffer.
	*/
	template< typename T >
	BufferPtr< T > makeBuffer( Device const & device
		, VkDeviceSize count
		, VkBufferUsageFlags usage
		, QueueShare sharingMode = {} )
	{
		return std::make_unique< Buffer< T > >( device
			, count
			, usage
			, std::move( sharingMode ) );
	}
	/**
	*\brief
	*	Helper function tor create a Buffer< T >.
	*\param[in] device
	*	The logical device.
	*\param[in] count
	*	The buffer elements count.
	*\param[in] usage
	*	The buffer usage flags.
	*\param[in] flags
	*	The buffer memory flags.
	*\return
	*	The created buffer.
	*/
	template< typename T >
	BufferPtr< T > makeBuffer( Device const & device
		, std::string const & debugName
		, VkDeviceSize count
		, VkBufferUsageFlags usage
		, QueueShare sharingMode = {} )
	{
		return std::make_unique< Buffer< T > >( device
			, debugName
			, count
			, usage
			, std::move( sharingMode ) );
	}
}

#include "ashespp/Buffer/Buffer.inl"

#endif
