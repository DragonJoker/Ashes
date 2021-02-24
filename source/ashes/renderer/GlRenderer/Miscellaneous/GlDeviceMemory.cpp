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

	namespace
	{
		//************************************************************************************************

		class ImageMemoryBinding
			: public DeviceMemory::DeviceMemoryBinding
		{
		public:
			ImageMemoryBinding( VkDeviceMemory parent
				, VkDevice device
				, VkImage image
				, VkDeviceSize memoryOffset )
				: DeviceMemory::DeviceMemoryBinding{ parent
					, device
					, get( image )->getTarget()
					, memoryOffset
					, get( image )->getMemoryRequirements()
					, image
					, get( image )->getInternal() }
				, m_texture{ get( image ) }
				, m_internal{ getInternalFormat( m_texture->getFormat() ) }
				, m_format{ getFormat( m_internal ) }
				, m_type{ getType( m_internal ) }
			{
				auto context = get( m_device )->getContext();
				glLogCall( context
					, glBindTexture
					, m_texture->getTarget()
					, m_boundName );

				switch ( m_texture->getTarget() )
				{
				case GL_TEXTURE_1D:
					doSetImage1D( context );
					break;
				case GL_TEXTURE_2D:
					doSetImage2D( context, m_texture->getDimensions().height );
					break;
				case GL_TEXTURE_3D:
					doSetImage3D( context, m_texture->getDimensions().depth );
					break;
				case GL_TEXTURE_1D_ARRAY:
					doSetImage2D( context, m_texture->getArrayLayers() );
					break;
				case GL_TEXTURE_2D_ARRAY:
					doSetImage3D( context, m_texture->getArrayLayers() );
					break;
				case GL_TEXTURE_CUBE:
					if ( hasTextureStorage( m_device ) )
					{
						doSetImage2D( context, m_texture->getDimensions().height );
					}
					else
					{
						doSetImageCubeFace( context, 0 );
						doSetImageCubeFace( context, 1 );
						doSetImageCubeFace( context, 2 );
						doSetImageCubeFace( context, 3 );
						doSetImageCubeFace( context, 4 );
						doSetImageCubeFace( context, 5 );
					}
					break;
				case GL_TEXTURE_CUBE_ARRAY:
					doSetImage3D( context, m_texture->getArrayLayers() );
					break;
				case GL_TEXTURE_2D_MULTISAMPLE:
					doSetImage2DMS( context );
					break;
				case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
					doSetImage3DMS( context, m_texture->getArrayLayers() );
					break;
				default:
					break;
				}

				if ( hasTextureStorage( device ) )
				{
					int levels = 0;
					glLogCall( context
						, glGetTexParameteriv
						, m_texture->getTarget()
						, GL_TEX_PARAMETER_IMMUTABLE_LEVELS
						, &levels );
					if ( levels != m_texture->getMipLevels() )
					{
						reportWarning( parent
							, VK_SUCCESS
							, "Image memory"
							, "Image mip level doesn't match requested one." );
					}

					int format = 0;
					glLogCall( context
						, glGetTexParameteriv
						, m_texture->getTarget()
						, GL_TEX_PARAMETER_IMMUTABLE_FORMAT
						, &format );
					if ( format == 0 )
					{
						reportWarning( parent
							, VK_SUCCESS
							, "Image memory"
							, "Image format isn't immutable." );
					}
				}
				else
				{
					glLogCall( context
						, glTexParameteri
						, m_texture->getTarget()
						, GL_TEX_PARAMETER_MAX_LEVEL
						, std::max( 0, GLint( m_texture->getMipLevels() ) - 1 ) );
					glLogCall( context
						, glGenerateMipmap
						, m_texture->getTarget() );
				}

				glLogCall( context
					, glBindTexture
					, m_texture->getTarget()
					, 0 );

				// Prepare update regions, layer by layer.
				uint32_t offset = 0u;
				VkBufferImageCopy bufferCopyRegion = {};
				bufferCopyRegion.imageSubresource.aspectMask = getAspectMask( m_texture->getFormat() );
				bufferCopyRegion.imageSubresource.mipLevel = 0u;
				bufferCopyRegion.imageSubresource.layerCount = 1u;
				bufferCopyRegion.imageExtent = m_texture->getDimensions();
				auto levelSize = uint32_t( getSize() / m_texture->getArrayLayers() );

				for ( uint32_t layer = 0; layer < m_texture->getArrayLayers(); layer++ )
				{
					bufferCopyRegion.imageSubresource.baseArrayLayer = layer;
					bufferCopyRegion.bufferOffset = offset;
					m_updateRegions.push_back( bufferCopyRegion );
					offset += levelSize;
				}
			}

			~ImageMemoryBinding()
			{
				m_texture->setMemory( nullptr );
			}

			void upload( ContextLock const & context
				, ByteArray const & data
				, VkDeviceSize offset
				, VkDeviceSize size )const override
			{
				doSetupUpdateRegions( offset, size );

				for ( size_t i = m_beginRegion; i < m_endRegion; ++i )
				{
					updateRegion( context, m_updateRegions[i] );
				}

				glLogCall( context
					, glBindTexture
					, m_texture->getTarget()
					, m_boundName );

				if ( m_texture->getMipLevels() > 1
					&& !isCompressedFormat( m_texture->getFormat() ) )
				{
					if ( context->hasMemoryBarrier() )
					{
						glLogCall( context
							, glMemoryBarrier
							, GL_MEMORY_BARRIER_TEXTURE_UPDATE );
					}

					glLogCall( context
						, glGenerateMipmap
						, m_texture->getTarget() );
				}

				glLogCall( context
					, glBindTexture
					, m_texture->getTarget()
					, 0u );
			}

		private:
			void doSetImage1D( ContextLock const & context )
			{
				if ( hasTextureStorage( m_device ) )
				{
					glLogCall( context
						, glTexStorage1D
						, m_texture->getTarget()
						, GLsizei( m_texture->getMipLevels() )
						, m_internal
						, m_texture->getDimensions().width );
				}
				else
				{
					glLogCall( context
						, glTexImage1D
						, m_texture->getTarget()
						, 0u
						, m_internal
						, m_texture->getDimensions().width
						, 0
						, m_format
						, m_type
						, nullptr );
				}
			}

			void doSetImage2D( ContextLock const & context
				, uint32_t height )
			{
				if ( hasTextureStorage( m_device ) )
				{
					glLogCall( context
						, glTexStorage2D
						, m_texture->getTarget()
						, GLsizei( m_texture->getMipLevels() )
						, m_internal
						, m_texture->getDimensions().width
						, height );
				}
				else
				{
					glLogCall( context
						, glTexImage2D
						, m_texture->getTarget()
						, 0u
						, m_internal
						, m_texture->getDimensions().width
						, height
						, 0
						, m_format
						, m_type
						, nullptr );
				}
			}

			void doSetImageCubeFace( ContextLock const & context
				, int face )
			{
				assert( !hasTextureStorage( m_device ) );
				glLogCall( context
					, glTexImage2D
					, GlTextureType( GL_TEXTURE_CUBE_POSITIVE_X + face )
					, 0u
					, m_internal
					, m_texture->getDimensions().width
					, m_texture->getDimensions().height
					, 0
					, m_format
					, m_type
					, nullptr );
			}

			void doSetImage3D( ContextLock const & context
				, uint32_t depth )
			{
				if ( hasTextureStorage( m_device ) )
				{
					glLogCall( context
						, glTexStorage3D
						, m_texture->getTarget()
						, GLsizei( m_texture->getMipLevels() )
						, m_internal
						, m_texture->getDimensions().width
						, m_texture->getDimensions().height
						, depth );
				}
				else
				{
					glLogCall( context
						, glTexImage3D
						, m_texture->getTarget()
						, 0u
						, m_internal
						, m_texture->getDimensions().width
						, m_texture->getDimensions().height
						, depth
						, 0
						, m_format
						, m_type
						, nullptr );
				}
			}

			void doSetImage2DMS( ContextLock const & context )
			{
				if ( hasTextureStorage( m_device ) )
				{
					glLogCall( context
						, glTexStorage2DMultisample
						, m_texture->getTarget()
						, GLsizei( m_texture->getSamples() )
						, m_internal
						, m_texture->getDimensions().width
						, m_texture->getDimensions().height
						, GL_TRUE );
				}
				else
				{
					glLogCall( context
						, glTexImage2DMultisample
						, m_texture->getTarget()
						, GLsizei( m_texture->getSamples() )
						, m_internal
						, m_texture->getDimensions().width
						, m_texture->getDimensions().height
						, GL_TRUE );
				}
			}

			void doSetImage3DMS( ContextLock const & context
				, uint32_t depth )
			{
				if ( hasTextureStorage( m_device ) )
				{
					glLogCall( context
						, glTexStorage3DMultisample
						, m_texture->getTarget()
						, GLsizei( m_texture->getSamples() )
						, m_internal
						, m_texture->getDimensions().width
						, m_texture->getDimensions().height
						, depth
						, GL_TRUE );
				}
				else
				{
					glLogCall( context
						, glTexImage3DMultisample
						, m_texture->getTarget()
						, GLsizei( m_texture->getSamples() )
						, m_internal
						, m_texture->getDimensions().width
						, m_texture->getDimensions().height
						, depth
						, GL_TRUE );
				}
			}

			void doSetupUpdateRegions( VkDeviceSize offset
				, VkDeviceSize size )const
			{
				assert( !m_updateRegions.empty() && "Can't update this texture." );
				auto layerSize = ashes::getSize( m_updateRegions[0].imageExtent
					, m_texture->getFormat() );
				m_beginRegion = 0u;

				while ( offset >= layerSize )
				{
					++m_beginRegion;
					offset -= layerSize;
				}

				m_endRegion = m_beginRegion + 1u;

				while ( size > layerSize )
				{
					++m_endRegion;
					size -= layerSize;
				}

				if ( m_beginRegion >= m_updateRegions.size()
					|| m_endRegion > m_updateRegions.size() )
				{
					get( m_device )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
						, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT
						, uint64_t( get( m_parent ) )
						, 0u
						, VK_ERROR_VALIDATION_FAILED_EXT
						, "OpenGL"
						, "Invalid offset and/or size." );
				}
			}

			void updateRegion( ContextLock const & context
				, VkBufferImageCopy const & copyInfo )const
			{
				if ( isCompressedFormat( m_texture->getFormat() ) )
				{
					auto layerSize = ashes::getSize( copyInfo.imageExtent
						, m_texture->getFormat()
						, copyInfo.imageSubresource.mipLevel );

					switch ( m_texture->getTarget() )
					{
					case GL_TEXTURE_1D:
						glLogCall( context
							, glCompressedTexSubImage1D
							, m_texture->getTarget()
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageExtent.width
							, m_internal
							, GLsizei( layerSize )
							, getBufferOffset( m_memoryOffset + copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_2D:
						glLogCall( context
							, glCompressedTexSubImage2D
							, m_texture->getTarget()
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageOffset.y
							, copyInfo.imageExtent.width
							, copyInfo.imageExtent.height
							, m_internal
							, GLsizei( layerSize )
							, getBufferOffset( m_memoryOffset + copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_3D:
						glLogCall( context
							, glCompressedTexSubImage3D
							, m_texture->getTarget()
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageOffset.y
							, copyInfo.imageOffset.z
							, copyInfo.imageExtent.width
							, copyInfo.imageExtent.height
							, copyInfo.imageExtent.depth
							, m_internal
							, GLsizei( layerSize )
							, getBufferOffset( m_memoryOffset + copyInfo.bufferOffset ) );

					case GL_TEXTURE_1D_ARRAY:
						glLogCall( context
							, glCompressedTexSubImage2D
							, m_texture->getTarget()
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageSubresource.baseArrayLayer
							, copyInfo.imageExtent.width
							, copyInfo.imageSubresource.layerCount
							, m_internal
							, GLsizei( layerSize )
							, getBufferOffset( m_memoryOffset + copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_2D_ARRAY:
						glLogCall( context
							, glCompressedTexSubImage3D
							, m_texture->getTarget()
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageOffset.y
							, copyInfo.imageSubresource.baseArrayLayer
							, copyInfo.imageExtent.width
							, copyInfo.imageExtent.height
							, copyInfo.imageSubresource.layerCount
							, m_internal
							, GLsizei( layerSize )
							, getBufferOffset( m_memoryOffset + copyInfo.bufferOffset ) );
						break;
					}
				}
				else
				{
					switch ( m_texture->getTarget() )
					{
					case GL_TEXTURE_1D:
						glLogCall( context
							, glTexSubImage1D
							, m_texture->getTarget()
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageExtent.width
							, m_format
							, m_type
							, getBufferOffset( m_memoryOffset + copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_2D:
						glLogCall( context
							, glTexSubImage2D
							, m_texture->getTarget()
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageOffset.y
							, copyInfo.imageExtent.width
							, copyInfo.imageExtent.height
							, m_format
							, m_type
							, getBufferOffset( m_memoryOffset + copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_3D:
						glLogCall( context
							, glTexSubImage3D
							, m_texture->getTarget()
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageOffset.y
							, copyInfo.imageOffset.z
							, copyInfo.imageExtent.width
							, copyInfo.imageExtent.height
							, copyInfo.imageExtent.depth
							, m_format
							, m_type
							, getBufferOffset( m_memoryOffset + copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_1D_ARRAY:
						glLogCall( context
							, glTexSubImage2D
							, m_texture->getTarget()
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageSubresource.baseArrayLayer
							, copyInfo.imageExtent.width
							, copyInfo.imageSubresource.layerCount
							, m_format
							, m_type
							, getBufferOffset( m_memoryOffset + copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_2D_ARRAY:
						glLogCall( context
							, glTexSubImage3D
							, m_texture->getTarget()
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageOffset.y
							, copyInfo.imageSubresource.baseArrayLayer
							, copyInfo.imageExtent.width
							, copyInfo.imageExtent.height
							, copyInfo.imageSubresource.layerCount
							, m_format
							, m_type
							, getBufferOffset( m_memoryOffset + copyInfo.bufferOffset ) );
						break;
					}
				}
			}

		private:
			Image * m_texture;
			GlInternal m_internal;
			GlFormat m_format;
			GlType m_type;
			std::vector< VkBufferImageCopy > m_updateRegions;
			mutable size_t m_beginRegion{ 0u };
			mutable size_t m_endRegion{ 0u };
		};

		//************************************************************************************************

		class BufferMemoryBinding
			: public DeviceMemory::DeviceMemoryBinding
		{
		public:
			BufferMemoryBinding( VkDeviceMemory parent
				, VkDevice device
				, VkBuffer buffer
				, VkDeviceSize memoryOffset )
				: DeviceMemory::DeviceMemoryBinding{ parent
					, device
					, get( buffer )->getTarget()
					, memoryOffset
					, get( buffer )->getMemoryRequirements()
					, buffer
					, get( parent )->getInternal() }
				, m_buffer{ get( buffer ) }
			{
			}

			~BufferMemoryBinding()
			{
				m_buffer->setMemory( nullptr );
			}

		private:
			Buffer * m_buffer;
			mutable GLenum m_copyTarget;
		};
	}

	//************************************************************************************************

	DeviceMemory::DeviceMemoryBinding::DeviceMemoryBinding( VkDeviceMemory parent
		, VkDevice device
		, GLuint boundTarget
		, VkDeviceSize memoryOffset
		, VkMemoryRequirements requirements
		, void * bound
		, GLuint boundName )
		: m_parent{ parent }
		, m_device{ device }
		, m_boundTarget{ boundTarget }
		, m_memoryOffset{ memoryOffset }
		, m_requirements{ std::move( requirements ) }
		, m_bound{ bound }
		, m_boundName{ boundName }
	{
	}

	DeviceMemory::DeviceMemoryBinding::~DeviceMemoryBinding()
	{
	}

	void DeviceMemory::DeviceMemoryBinding::upload( ContextLock const & context
		, ByteArray const & data
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
	}

	//************************************************************************************************

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
			auto & binding = *m_bindings.emplace_back( memoryOffset
				, std::make_unique< BufferMemoryBinding >( get( this )
					, m_device
					, buffer
					, memoryOffset ) ).second;
			get( buffer )->setInternal( getInternal(), memoryOffset );
			get( buffer )->setMemory( get( this ) );

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
			auto & binding = *m_bindings.emplace_back( memoryOffset
				, std::make_unique< ImageMemoryBinding >( get( this )
					, m_device
					, image
					, memoryOffset ) ).second;
			get( image )->setMemory( get( this ) );
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
		auto copySize = size == WholeSize
			? getSize()
			: size;
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_COPY_WRITE
			, getInternal() );
		auto result = glLogNonVoidCall( context
			, glMapBufferRange
			, GL_BUFFER_TARGET_COPY_WRITE
			, GLintptr( offset )
			, GLsizei( copySize )
			, GL_MEMORY_MAP_WRITE_BIT );
		assert( result != nullptr );

		if ( result )
		{
			std::memcpy( result, m_data.data() + offset, copySize );
			glLogCall( context
				, glUnmapBuffer
				, GL_BUFFER_TARGET_COPY_WRITE );
		}

		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_COPY_WRITE
			, 0u );
	}

	void DeviceMemory::download( ContextLock const & context
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		assert( !m_data.empty() );
		auto copySize = size == WholeSize
			? getSize()
			: size;
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_COPY_READ
			, getInternal() );
		auto result = glLogNonVoidCall( context
			, glMapBufferRange
			, GL_BUFFER_TARGET_COPY_READ
			, GLintptr( offset )
			, GLsizei( copySize )
			, GL_MEMORY_MAP_READ_BIT );
		assert( result != nullptr );

		if ( result )
		{
			std::memcpy( m_data.data() + offset, result, copySize );
			glLogCall( context
				, glUnmapBuffer
				, GL_BUFFER_TARGET_COPY_READ );
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_COPY_READ
				, 0u );
		}

		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_COPY_READ
			, 0u );
	}

	VkResult DeviceMemory::lock( ContextLock const & context
		, VkDeviceSize offset
		, VkDeviceSize size
		, VkMemoryMapFlags flags
		, void ** data )const
	{
		assert( ( !m_mapped || ( size == ~( 0ull ) ? ( m_mappedSize == m_allocateInfo.allocationSize ) : ( m_mappedSize == size ) ) ) 
			&& "VkDeviceMemory should not be mapped" );

		if ( size == WholeSize )
		{
			m_mappedOffset = 0u;
			m_mappedSize = m_allocateInfo.allocationSize;
		}
		else
		{
			m_mappedOffset = offset;
			m_mappedSize = size;
		}

		*data = m_data.data() + offset;
		m_mapped = *data != nullptr;
		assert( m_mapped );
		assert( m_mappedOffset + m_mappedSize < m_data.size() );
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
		download( context, offset, size );
		return VK_SUCCESS;
	}

	void DeviceMemory::unlock( ContextLock const & context )const
	{
		if ( m_mapped )
		{
			m_mapped = false;
			checkControlValue( m_data );
			upload( context, m_mappedOffset, m_mappedSize );
		}
	}

	//************************************************************************************************
}
