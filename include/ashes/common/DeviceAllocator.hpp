/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_common_DeviceAllocator_HPP___
#define ___Ashes_common_DeviceAllocator_HPP___
#pragma once

#include "BuddyAllocator.hpp"

#include <vulkan/vulkan.h>

#include <memory>

namespace ashes
{
	class DeviceAllocatorBase
	{
	public:
		DeviceAllocatorBase()
			: m_callbacks{ this
				, &DeviceAllocatorBase::allocate
				, &DeviceAllocatorBase::reallocate
				, &DeviceAllocatorBase::deallocate
				, &DeviceAllocatorBase::onInternalAllocation
				, &DeviceAllocatorBase::onInternalFree }
		{
		}

		virtual ~DeviceAllocatorBase()noexcept = default;

		inline operator VkAllocationCallbacks const & ()const
		{
			return m_callbacks;
		}

		inline VkAllocationCallbacks const * getAllocationCallbacks()const
		{
			return &m_callbacks;
		}

		inline VkAllocationCallbacks const * getAllocationCallbacks()
		{
			return &m_callbacks;
		}

	private:
		virtual void * allocate( size_t size
			, size_t alignment
			, VkSystemAllocationScope allocationScope ) = 0;
		virtual void * reallocate( void * pOriginal
			, size_t size
			, size_t alignment
			, VkSystemAllocationScope allocationScope ) = 0;
		virtual void deallocate( void * pMemory ) = 0;
		virtual void onInternalAllocation( size_t size
			, VkInternalAllocationType allocationType
			, VkSystemAllocationScope allocationScope ) = 0;
		virtual void onInternalFree( size_t size
			, VkInternalAllocationType allocationType
			, VkSystemAllocationScope allocationScope ) = 0;

		static inline void * allocate( void * pUserData
			, size_t size
			, size_t alignment
			, VkSystemAllocationScope allocationScope )
		{
			return reinterpret_cast< DeviceAllocatorBase * >( pUserData )->allocate( size
				, alignment
				, allocationScope );
		}

		static inline void * reallocate( void * pUserData
			, void * pOriginal
			, size_t size
			, size_t alignment
			, VkSystemAllocationScope allocationScope )
		{
			return reinterpret_cast< DeviceAllocatorBase * >( pUserData )->reallocate( pOriginal
				, size
				, alignment
				, allocationScope );
		}

		static inline void deallocate( void * pUserData
			, void * pMemory )
		{
			return reinterpret_cast< DeviceAllocatorBase * >( pUserData )->deallocate( pMemory );
		}

		static inline void onInternalAllocation( void * pUserData
			, size_t size
			, VkInternalAllocationType allocationType
			, VkSystemAllocationScope allocationScope )
		{
			return reinterpret_cast< DeviceAllocatorBase * >( pUserData )->onInternalAllocation( size
				, allocationType
				, allocationScope );
		}

		static inline void onInternalFree( void * pUserData
			, size_t size
			, VkInternalAllocationType allocationType
			, VkSystemAllocationScope allocationScope )
		{
			return reinterpret_cast< DeviceAllocatorBase * >( pUserData )->onInternalFree( size
				, allocationType
				, allocationScope );
		}

	private:
		VkAllocationCallbacks m_callbacks;
	};

	using DeviceAllocatorPtr = std::unique_ptr< DeviceAllocatorBase >;
		
	template< typename TraitsT >
	class DeviceAllocatorT
		: public DeviceAllocatorBase
	{
		static constexpr uint32_t MinBlockSize = 16u;
		static constexpr uint32_t LevelCount = 24u;

	public:
		DeviceAllocatorT()
			: m_allocator{ LevelCount, MinBlockSize }
		{
		}

		~DeviceAllocatorT()noexcept = default;

	private:
		inline void * allocate( size_t size
			, size_t /*alignment*/
			, VkSystemAllocationScope )override
		{
			return reinterpret_cast< void * >( m_allocator.allocate( size ) );
		}

		inline void * reallocate( void * pOriginal
			, size_t size
			, size_t /*alignment*/
			, VkSystemAllocationScope )override
		{
			m_allocator.deallocate( reinterpret_cast< uint8_t * >( pOriginal ) );
			return reinterpret_cast< void * >( m_allocator.allocate( size ) );
		}

		inline void deallocate( void * pMemory )override
		{
			m_allocator.deallocate( reinterpret_cast< uint8_t * >( pMemory ) );
		}

		inline void onInternalAllocation( size_t
			, VkInternalAllocationType
			, VkSystemAllocationScope )override
		{
		}

		inline void onInternalFree( size_t
			, VkInternalAllocationType
			, VkSystemAllocationScope )override
		{
		}

	private:
		BuddyAllocatorT< TraitsT > m_allocator;
	};

	template< typename TraitsT >
	DeviceAllocatorPtr makeAllocator()
	{
		return std::make_unique< DeviceAllocatorT< TraitsT > >();
	}
}

#endif
