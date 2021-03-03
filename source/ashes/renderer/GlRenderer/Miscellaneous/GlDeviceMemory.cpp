/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Miscellaneous/GlDeviceMemory.hpp"

#include "Command/Commands/GlCopyBufferToImageCommand.hpp"
#include "Core/GlDevice.hpp"
#include "Core/GlInstance.hpp"
#include "Image/GlImage.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "Miscellaneous/GlBufferMemoryBinding.hpp"
#include "Miscellaneous/GlImageMemoryBinding.hpp"

#include "ashesgl_api.hpp"

#include <algorithm>
#include <cstring>
#include <map>

namespace ashes::gl
{
	//************************************************************************************************

	VkMemoryPropertyFlags getFlags( VkDevice device
		, uint32_t memoryTypeIndex )
	{
		auto & memProps = get( get( device )->getPhysicalDevice() )->getMemoryProperties();
		assert( memoryTypeIndex < memProps.memoryTypeCount
			&& "Wrong deduced memory type" );
		return memProps.memoryTypes[memoryTypeIndex].propertyFlags;
	}

	//************************************************************************************************

	namespace
	{
#if !defined( NDEBUG )

		static uint32_t constexpr ControlValueCount = 64u;
		static uint8_t constexpr ControlValue = 0xDC;

		void initControlValue( ByteArray & data )
		{
			auto it = data.begin() + ( data.size() - ControlValueCount );

			while ( it != data.end() )
			{
				*it = ControlValue;
				++it;
			}
		}

		void checkControlValue( ByteArray const & data )
		{
			auto it = data.begin() + ( data.size() - ControlValueCount );

			while ( it != data.end() )
			{
				assert( *it == ControlValue );
				++it;
			}
		}

#else

		static uint32_t constexpr ControlValueCount = 0u;

		void initControlValue( ByteArray const & data )
		{
		}

		void checkControlValue( ByteArray const & data )
		{
		}

#endif
	}

	DeviceMemory::DeviceMemory( VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, VkMemoryAllocateInfo allocateInfo )
		: m_device{ device }
		, m_allocateInfo{ std::move( allocateInfo ) }
		, m_flags{ getFlags( m_device, m_allocateInfo.memoryTypeIndex ) }
	{
		m_data.resize( allocateInfo.allocationSize + ControlValueCount );
		initControlValue( m_data );
		// Create the buffer effectively owning the data
		auto context = get( m_device )->getContext();
		m_internal = context->createBuffer( GL_BUFFER_TARGET_COPY_WRITE
			, GLsizeiptr( m_allocateInfo.allocationSize )
			, getBufferDataUsageFlags( m_flags ) );
	}

	DeviceMemory::~DeviceMemory()
	{
		auto context = get( m_device )->getContext();
		context->deleteBuffer( m_internal );
	}

	VkResult DeviceMemory::bindBuffer( VkBuffer buffer
		, VkDeviceSize memoryOffset )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			auto it = std::find_if( m_bindings.begin()
				, m_bindings.end()
				, [memoryOffset]( std::pair< VkDeviceSize, DeviceMemoryBindingPtr > const & lookup )
				{
					return memoryOffset == lookup.first;
				} );

			if ( it == m_bindings.end()
				|| it->second->getBound() != buffer )
			{
				auto & binding = *m_bindings.emplace_back( memoryOffset
					, std::make_unique< BufferMemoryBinding >( get( this )
						, m_device
						, buffer
						, memoryOffset ) ).second;
				get( buffer )->setInternal( getInternal() );
				binding.map( m_mappedOffset, m_mappedSize );
				it = m_bindings.begin() + ( m_bindings.size() - 1 );
			}

			auto & binding = *it->second;

			if ( !m_data.empty() )
			{
				auto context = get( m_device )->getContext();
				binding.upload( context
					, m_data
					, 0u
					, m_data.size() );
			}

			result = VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			result = exc.getResult();
		}
		catch ( ... )
		{
		}

		return result;
	}

	VkResult DeviceMemory::bindImage( VkImage image
		, VkDeviceSize memoryOffset )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			auto it = std::find_if( m_bindings.begin()
				, m_bindings.end()
				, [memoryOffset]( std::pair< VkDeviceSize, DeviceMemoryBindingPtr > const & lookup )
				{
					return memoryOffset == lookup.first;
				} );

			if ( it == m_bindings.end() )
			{
				auto & binding = *m_bindings.emplace_back( memoryOffset
					, std::make_unique< ImageMemoryBinding >( get( this )
						, m_device
						, image
						, memoryOffset ) ).second;
				binding.map( m_mappedOffset, m_mappedSize );
			}
			else
			{
				assert( it->second->getBound() == image );
			}

			result = VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			result = exc.getResult();
		}
		catch ( ... )
		{
		}

		return result;
	}

	void DeviceMemory::unbindBuffer( VkBuffer buffer )
	{
		auto it = m_bindings.begin();

		while ( it != m_bindings.end() )
		{
			if ( it->second->getBound() == buffer )
			{
				it = m_bindings.erase( it );
			}
			else
			{
				++it;
			}
		}
	}

	void DeviceMemory::unbindImage( VkImage image )
	{
		auto it = m_bindings.begin();

		while ( it != m_bindings.end() )
		{
			if ( it->second->getBound() == image )
			{
				it = m_bindings.erase( it );
			}
			else
			{
				++it;
			}
		}
	}

	void DeviceMemory::upload( ContextLock const & context
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		assert( !m_data.empty() );

		if ( size == WholeSize )
		{
			size = getSize();
			offset = 0;
		}

		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_SHADER_STORAGE
			, getInternal() );
		glLogCall( context
			, glBufferSubData
			, GL_BUFFER_TARGET_SHADER_STORAGE
			, GLintptr( offset )
			, GLsizei( size )
			, m_data.data() + offset );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_SHADER_STORAGE
			, 0u );
	}

	void DeviceMemory::download( ContextLock const & context
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		assert( !m_data.empty() );

		if ( size == WholeSize )
		{
			size = getSize();
			offset = 0;
		}

		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_SHADER_STORAGE
			, getInternal() );
		glLogCall( context
			, glGetBufferSubData
			, GL_BUFFER_TARGET_SHADER_STORAGE
			, GLintptr( offset )
			, GLsizei( size )
			, m_data.data() + offset );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_SHADER_STORAGE
			, 0u );
	}

	VkResult DeviceMemory::lock( ContextLock const & context
		, VkDeviceSize offset
		, VkDeviceSize size
		, VkMemoryMapFlags flags
		, void ** data )const
	{
		if ( size == WholeSize )
		{
			offset = 0u;
			size = m_allocateInfo.allocationSize;
		}

		m_mappedOffset = offset;
		m_mappedSize = size;;

		for ( auto & [key, binding] : m_bindings )
		{
			binding->map( offset, size );
		}

		*data = m_data.data() + offset;
		m_dirty = true;
		return VK_SUCCESS;
	}

	VkResult DeviceMemory::flush( ContextLock const & context
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		if ( size == WholeSize )
		{
			offset = 0u;
			size = m_allocateInfo.allocationSize;
		}

		checkControlValue( m_data );
		upload( context, offset, size );
		return VK_SUCCESS;
	}

	VkResult DeviceMemory::invalidate( ContextLock const & context
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		if ( size == WholeSize )
		{
			offset = 0u;
			size = m_allocateInfo.allocationSize;
		}

		m_dirty = true;
		checkControlValue( m_data );
		download( context, m_mappedOffset, m_mappedSize );
		return VK_SUCCESS;
	}

	void DeviceMemory::unlock( ContextLock const & context )const
	{
		for ( auto & [key, binding] : m_bindings )
		{
			binding->unmap();
		}

		m_mappedOffset = 0ull;
		m_mappedSize = 0ull;;
	}

	//************************************************************************************************
}
