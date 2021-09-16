/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Miscellaneous/TestDeviceMemory.hpp"

#include "Core/TestDevice.hpp"
#include "Core/TestInstance.hpp"
#include "Core/TestPhysicalDevice.hpp"

#include <ashes/common/Exception.hpp>

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "ashestest_api.hpp"

namespace ashes::test
{
	//*********************************************************************************************

	namespace
	{
		VkMemoryPropertyFlags getMemoryProperties( uint32_t memoryTypeIndex )
		{
			assert( memoryTypeIndex < Instance::getMemoryProperties().memoryTypeCount
				&& "Wrong deduced memory type" );
			return Instance::getMemoryProperties().memoryTypes[memoryTypeIndex].propertyFlags;
		}
	}

	//*********************************************************************************************

	ObjectMemory::ObjectMemory( VkDevice pdevice
		, VkDeviceMemory pdeviceMemory
		, VkDeviceSize poffset
		, VkMemoryAllocateInfo pallocateInfo )
		: device{ pdevice }
		, deviceMemory{ pdeviceMemory }
		, offset{ poffset }
		, allocateInfo{ std::move( pallocateInfo ) }
	{
	}

	ObjectMemory::ObjectMemory( ObjectMemory && rhs )noexcept
		: device{ rhs.device }
		, deviceMemory{ rhs.deviceMemory }
		, offset{ rhs.offset }
		, allocateInfo{ std::move( rhs.allocateInfo ) }
	{
		rhs.device = nullptr;
		rhs.deviceMemory = nullptr;
		rhs.offset = 0;
	}

	ObjectMemory & ObjectMemory::operator=( ObjectMemory && rhs )noexcept
	{
		device = rhs.device;
		deviceMemory = rhs.deviceMemory;
		offset = rhs.offset;
		allocateInfo = std::move( rhs.allocateInfo );

		rhs.device = nullptr;
		rhs.deviceMemory = nullptr;
		rhs.offset = 0;

		return *this;
	}

	VkResult ObjectMemory::lock( void ** mapped )const
	{
		*mapped = get( deviceMemory )->m_data.data() + offset;
		return VK_SUCCESS;
	}

	void ObjectMemory::unlock()const
	{
	}

	void ObjectMemory::upload( uint8_t const * buffer
		, VkDeviceSize poffset
		, VkDeviceSize psize )const
	{
		auto maxOffset = std::max( offset, poffset );
		auto objectOffset = maxOffset - offset;
		auto copySize = psize - VkDeviceSize( std::abs( int64_t( poffset ) - int64_t( offset ) ) );

		if ( psize == WholeSize )
		{
			assert( poffset == 0ull );
			copySize = allocateInfo.allocationSize;
		}
		else if ( copySize > allocateInfo.allocationSize )
		{
			copySize = allocateInfo.allocationSize - objectOffset;
		}

		void * mapped{};

		if ( lock( &mapped ) == VK_SUCCESS )
		{
			std::memcpy( static_cast< uint8_t * >( mapped ) + objectOffset
				, buffer + maxOffset
				, copySize );
			unlock();
		}
	}

	void ObjectMemory::download( uint8_t * buffer
		, VkDeviceSize poffset
		, VkDeviceSize psize )const
	{
		auto maxOffset = std::max( offset, poffset );
		auto objectOffset = maxOffset - offset;
		auto copySize = psize - VkDeviceSize( std::abs( int64_t( poffset ) - int64_t( offset ) ) );

		if ( psize == WholeSize )
		{
			assert( poffset == 0ull );
			objectOffset = 0u;
			copySize = allocateInfo.allocationSize;
		}
		else if ( copySize > allocateInfo.allocationSize )
		{
			copySize = allocateInfo.allocationSize - objectOffset;
		}

		void * mapped{};

		if ( lock( &mapped ) == VK_SUCCESS )
		{
			std::memcpy( buffer + maxOffset
				, static_cast< uint8_t * >( mapped ) + objectOffset
				, copySize );
			unlock();
		}
	}

	//*********************************************************************************************

	class BufferDeviceMemory
	{
	public:
		BufferDeviceMemory( VkDevice device
			, VkDeviceMemory parent
			, VkDeviceSize offset
			, VkMemoryAllocateInfo allocateInfo
			, VkBufferUsageFlags )
			: memory{ device, parent, offset, std::move( allocateInfo ) }
		{
		}

	public:
		ObjectMemory memory;
	};
	
	//*********************************************************************************************

	class Texture1DDeviceMemory
	{
	public:
		Texture1DDeviceMemory( VkDevice device
			, VkDeviceMemory parent
			, VkDeviceSize offset
			, VkMemoryAllocateInfo allocateInfo
			, VkImageCreateInfo const & )
			: memory{ device, parent, offset, std::move( allocateInfo ) }
		{
		}

	public:
		ObjectMemory memory;
	};

	//*********************************************************************************************

	class Texture2DDeviceMemory
	{
	public:
		Texture2DDeviceMemory( VkDevice device
			, VkDeviceMemory parent
			, VkDeviceSize offset
			, VkMemoryAllocateInfo allocateInfo
			, VkImageCreateInfo const & )
			: memory{ device, parent, offset, std::move( allocateInfo ) }
		{
		}

	public:
		ObjectMemory memory;
	};
	
	//*********************************************************************************************

	class Texture3DDeviceMemory
	{
	public:
		Texture3DDeviceMemory( VkDevice device
			, VkDeviceMemory parent
			, VkDeviceSize offset
			, VkMemoryAllocateInfo allocateInfo
			, VkImageCreateInfo const & )
			: memory{ device, parent, offset, std::move( allocateInfo ) }
		{
		}

	public:
		ObjectMemory memory;
	};

	//*********************************************************************************************

	DeviceMemory::DeviceMemory( VkDevice device
		, VkMemoryAllocateInfo allocateInfo )
		: m_device{ device }
		, m_allocateInfo{ std::move( allocateInfo ) }
		, m_propertyFlags{ getMemoryProperties( m_allocateInfo.memoryTypeIndex ) }
	{
		thread_local uint8_t defaultInitValue = 10u;

		if ( ashes::checkFlag( m_propertyFlags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
		{
			m_data.resize( allocateInfo.allocationSize, defaultInitValue++ );
		}
	}

	DeviceMemory::~DeviceMemory()
	{
		if ( !m_objects.empty() )
		{
			onDestroy( get( this ) );
		}
	}

	VkResult DeviceMemory::bindToBuffer( VkBuffer buffer
		, VkDeviceSize memoryOffset
		, ObjectMemory *& objectMemory )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			BufferDeviceMemory impl
			{
				m_device,
				get( this ),
				memoryOffset,
				m_allocateInfo,
				get( buffer )->getUsage()
			};
			m_objects.emplace_back( std::make_unique< ObjectMemory >( std::move( impl.memory ) ) );
			objectMemory = m_objects.back().get();
			updateUpload( *objectMemory, 0ull, WholeSize );
			result = VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			std::cerr << exc.what() << std::endl;
			result = exc.getResult();
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
		}
		catch ( ... )
		{
			std::cerr << "Unknown error" << std::endl;
		}

		return result;
	}

	VkResult DeviceMemory::bindToImage1D( VkImage image
		, VkDeviceSize memoryOffset
		, ObjectMemory *& objectMemory )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			Texture1DDeviceMemory impl
			{
				m_device,
				get( this ),
				memoryOffset,
				m_allocateInfo,
				get( image )->getCreateInfo()
			};
			m_objects.emplace_back( std::make_unique< ObjectMemory >( std::move( impl.memory ) ) );
			objectMemory = m_objects.back().get();
			updateUpload( *objectMemory, 0ull, WholeSize );
			result = VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			std::cerr << exc.what() << std::endl;
			result = exc.getResult();
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
		}
		catch ( ... )
		{
			std::cerr << "Unknown error" << std::endl;
		}

		return result;
	}

	VkResult DeviceMemory::bindToImage2D( VkImage image
		, VkDeviceSize memoryOffset
		, ObjectMemory *& objectMemory )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			Texture2DDeviceMemory impl
			{
				m_device,
				get( this ),
				memoryOffset,
				m_allocateInfo,
				get( image )->getCreateInfo()
			};
			m_objects.emplace_back( std::make_unique< ObjectMemory >( std::move( impl.memory ) ) );
			objectMemory = m_objects.back().get();
			updateUpload( *objectMemory, 0ull, WholeSize );
			result = VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			std::cerr << exc.what() << std::endl;
			result = exc.getResult();
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
		}
		catch ( ... )
		{
			std::cerr << "Unknown error" << std::endl;
		}

		return result;
	}

	VkResult DeviceMemory::bindToImage3D( VkImage image
		, VkDeviceSize memoryOffset
		, ObjectMemory *& objectMemory )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			Texture3DDeviceMemory impl
			{
				m_device,
				get( this ),
				memoryOffset,
				m_allocateInfo,
				get( image )->getCreateInfo()
			};
			m_objects.emplace_back( std::make_unique< ObjectMemory >( std::move( impl.memory ) ) );
			objectMemory = m_objects.back().get();
			updateUpload( *objectMemory, 0ull, WholeSize );
			result = VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			std::cerr << exc.what() << std::endl;
			result = exc.getResult();
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
		}
		catch ( ... )
		{
			std::cerr << "Unknown error" << std::endl;
		}

		return result;
	}

	void DeviceMemory::updateUpload( ObjectMemory const & memory
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		if ( !m_data.empty() )
		{
			memory.upload( m_data.data()
				, ( ( size == WholeSize && offset == 0u )
					? 0u
					: offset + memory.offset )
				, size );
		}
	}

	void DeviceMemory::updateDownload( ObjectMemory const & memory
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		if ( !m_data.empty() )
		{
			memory.download( m_data.data()
				, ( ( size == WholeSize && offset == 0u )
					? 0u
					: offset + memory.offset )
				, size );
		}
	}

	void DeviceMemory::updateUpload( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		if ( !m_data.empty() && !m_objects.empty() )
		{
			auto paramOffset = offset;
			auto paramSize = size;

			for ( auto & object : m_objects )
			{
				auto objectOffset = std::max( paramOffset, object->offset ) - object->offset;
				auto objectSize = std::min( paramSize
					, ( object->allocateInfo.allocationSize > objectOffset
						? object->allocateInfo.allocationSize - objectOffset
						: 0u ) );
				objectOffset = object->offset;

				if ( objectSize > 0u
					&& paramOffset + paramSize > objectOffset )
				{
					object->upload( m_data.data()
						, offset
						, size );
				}
			}

			m_dirty = false;
		}
	}

	void DeviceMemory::updateDownload( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		if ( !m_data.empty() && !m_objects.empty() )
		{
			auto paramOffset = offset;
			auto paramSize = size;

			for ( auto & object : m_objects )
			{
				auto objectOffset = std::max( paramOffset, object->offset ) - object->offset;
				auto objectSize = std::min( paramSize
					, ( object->allocateInfo.allocationSize > objectOffset
						? object->allocateInfo.allocationSize - objectOffset
						: 0u ) );
				objectOffset = object->offset;

				if ( objectSize > 0u
					&& paramOffset + paramSize > objectOffset )
				{
					object->download( m_data.data()
						, offset
						, size );
				}
			}

			m_dirty = false;
		}
	}

	void DeviceMemory::updateData( VkDeviceMemory src
		, VkDeviceSize srcOffset
		, VkDeviceSize dstOffset
		, VkDeviceSize size )
	{
		if ( !m_data.empty()
			&& !get( src )->m_data.empty() )
		{
			std::memcpy( &m_data[dstOffset]
				, &get( src )->m_data[srcOffset]
				, size );
		}
	}

	VkResult DeviceMemory::lock( VkDeviceSize offset
		, VkDeviceSize size
		, VkMemoryMapFlags flags
		, void ** data )const
	{
		assert( !m_mapped && "VkDeviceMemory should not be mapped" );
		assert( !m_data.empty() && "VkDeviceMemory should be mappable" );
		size = (size == ~( 0ull )
			? m_allocateInfo.allocationSize
			: size);

		if ( m_data.size() <= offset 
			|| m_data.size() < ( offset + size ) )
		{
			return VK_ERROR_OUT_OF_DEVICE_MEMORY;
		}

		*data = m_data.data() + offset;
		m_mappedOffset = offset;
		m_mappedSize = size;
		m_mapped = *data != nullptr;
		m_dirty = true;
		return VK_SUCCESS;
	}

	VkResult DeviceMemory::flush( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		assert( m_mapped && "VkDeviceMemory should be mapped" );
		assert( !m_data.empty() && "VkDeviceMemory should be mappable" );
		size = ( size == ~( 0ull )
			? m_allocateInfo.allocationSize
			: size );

		if ( m_data.size() <= offset
			|| m_data.size() < ( offset + size ) )
		{
			return VK_ERROR_OUT_OF_DEVICE_MEMORY;
		}

		updateUpload( offset, size );
		return VK_SUCCESS;
	}

	VkResult DeviceMemory::invalidate( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		assert( m_mapped && "VkDeviceMemory should be mapped" );
		assert( !m_data.empty() && "VkDeviceMemory should be mappable" );
		size = ( size == ~( 0ull )
			? m_allocateInfo.allocationSize
			: size );

		if ( m_data.size() <= offset
			|| m_data.size() < ( offset + size ) )
		{
			return VK_ERROR_OUT_OF_DEVICE_MEMORY;
		}

		m_dirty = true;
		return VK_SUCCESS;
	}

	void DeviceMemory::unlock()const
	{
		assert( m_mapped && "VkDeviceMemory should be mapped" );
		assert( !m_data.empty() && "VkDeviceMemory should be mappable" );
		m_mapped = false;
		updateUpload( m_mappedOffset, m_mappedSize );
	}

	void DeviceMemory::upload( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		updateUpload( offset, size );
	}

	void DeviceMemory::download( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		updateDownload( offset, size );
	}

	//*********************************************************************************************
}
