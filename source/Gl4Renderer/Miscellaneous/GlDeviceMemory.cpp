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

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	//*********************************************************************************************

	VkMemoryPropertyFlags getFlags( uint32_t memoryTypeIndex )
	{
		assert( memoryTypeIndex < Instance::getMemoryProperties().memoryTypeCount
			&& "Wrong deduced memory type" );
		return Instance::getMemoryProperties().memoryTypes[memoryTypeIndex].propertyFlags;
	}

	//************************************************************************************************

	namespace
	{
		class ImageMemory
			: public DeviceMemory::DeviceMemoryImpl
		{
		public:
			ImageMemory( VkDevice device
				, VkMemoryAllocateInfo allocateInfo
				, VkImage texture
				, VkDeviceSize memoryOffset )
				: DeviceMemory::DeviceMemoryImpl{ device, std::move( allocateInfo ), get( texture )->getInternal(), get( texture )->getTarget(), memoryOffset }
				, m_texture{ get( texture ) }
				, m_internal{ getInternalFormat( m_texture->getFormat() ) }
				, m_format{ getFormat( m_internal ) }
				, m_type{ getType( m_internal ) }
			{
				auto context = get( m_device )->getContext();
				glLogCall( context
					, glBindTexture
					, m_boundTarget
					, m_boundResource );

				switch ( m_boundTarget )
				{
				case GL_TEXTURE_1D:
					doSetImage1D( context );
					break;
				case GL_TEXTURE_2D:
					doSetImage2D( context );
					break;
				case GL_TEXTURE_3D:
					doSetImage3D( context, m_texture->getDimensions().depth );
					break;
				case GL_TEXTURE_1D_ARRAY:
					doSetImage2D( context );
					break;
				case GL_TEXTURE_2D_ARRAY:
					doSetImage3D( context, m_texture->getArrayLayers() );
					break;
				case GL_TEXTURE_2D_MULTISAMPLE:
					doSetImage2DMS( context );
					break;
				case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
					doSetImage3DMS( context );
					break;
				default:
					break;
				}

				int levels = 0;
				context->glGetTexParameteriv( m_boundTarget
					, GL_TEXTURE_IMMUTABLE_LEVELS
					, &levels );
				assert( levels == m_texture->getMipLevels() );
				int format = 0;
				context->glGetTexParameteriv( m_boundTarget
					, GL_TEXTURE_IMMUTABLE_FORMAT
					, &format );
				assert( format != 0 );
				glLogCall( context
					, glBindTexture
					, m_boundTarget
					, 0 );

				// If the texture is visible to the host, we'll need a PBO to map it to RAM.
				if ( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
				{
					glLogCall( context
						, glGenBuffers
						, 1u
						, &m_pbo );
					// Initialise Upload PBO.
					glLogCall( context
						, glBindBuffer
						, GL_BUFFER_TARGET_PIXEL_UNPACK
						, m_pbo );
					glLogCall( context
						, glBufferStorage
						, m_pbo
						, GLsizeiptr( m_allocateInfo.allocationSize )
						, nullptr
						, GLbitfield( convertMemoryPropertyFlags( m_flags ) ) );
					glLogCall( context
						, glBindBuffer
						, GL_BUFFER_TARGET_PIXEL_UNPACK
						, 0u );

					// Prepare update regions, layer by layer.
					uint32_t offset = 0;
					VkBufferImageCopy bufferCopyRegion = {};
					bufferCopyRegion.imageSubresource.aspectMask = getAspectMask( m_texture->getFormat() );
					bufferCopyRegion.imageSubresource.mipLevel = 0u;
					bufferCopyRegion.imageSubresource.layerCount = 1u;
					bufferCopyRegion.imageExtent = m_texture->getDimensions();
					auto levelSize = uint32_t( m_allocateInfo.allocationSize / m_texture->getArrayLayers() );

					for ( uint32_t layer = 0; layer < m_texture->getArrayLayers(); layer++ )
					{
						bufferCopyRegion.imageSubresource.baseArrayLayer = layer;
						bufferCopyRegion.bufferOffset = offset;
						m_updateRegions.push_back( bufferCopyRegion );
						offset += levelSize;
					}
				}
			}

			~ImageMemory()
			{
				if ( m_pbo != GL_INVALID_INDEX )
				{
					auto context = get( m_device )->getContext();
					context->glDeleteBuffers( 1u, &m_pbo );
				}
			}

			VkResult lock( VkDeviceSize offset
				, VkDeviceSize size
				, VkMemoryMapFlags flags
				, void ** data )const override
			{
				assert( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local texture" );

				auto context = get( m_device )->getContext();
				glLogCall( context
					, glBindBuffer
					, GL_BUFFER_TARGET_PIXEL_UNPACK
					, m_pbo );
				doSetupUpdateRegions( offset, size );
				auto result = glLogCall( context
					, glMapBufferRange
					, GL_BUFFER_TARGET_PIXEL_UNPACK
					, GLintptr( offset )
					, GLsizei( size == WholeSize ? m_allocateInfo.allocationSize : size )
					, m_mapFlags );
				assertDebugValue( m_isLocked, false );
				setDebugValue( m_isLocked, result != nullptr );
				*data = result;
				return result
					? VK_SUCCESS
					: VK_ERROR_MEMORY_MAP_FAILED;
			}

			VkResult flush( VkDeviceSize offset
				, VkDeviceSize size )const override
			{
				assert( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local texture" );
				assertDebugValue( m_isLocked, true );

				auto context = get( m_device )->getContext();
				glLogCall( context
					, glFlushMappedBufferRange
					, GL_BUFFER_TARGET_PIXEL_UNPACK
					, GLintptr( offset )
					, GLsizei( size == WholeSize ? m_allocateInfo.allocationSize : size ) );
				return VK_SUCCESS;
			}

			VkResult invalidate( VkDeviceSize offset
				, VkDeviceSize size )const override
			{
				assert( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local texture" );
				assertDebugValue( m_isLocked, true );

				auto context = get( m_device )->getContext();
				glLogCall( context
					, glInvalidateBufferSubData
					, GL_BUFFER_TARGET_PIXEL_UNPACK
					, GLintptr( offset )
					, GLsizei( size == WholeSize ? m_allocateInfo.allocationSize : size ) );
				return VK_SUCCESS;
			}

			void unlock()const override
			{
				assert( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local texture" );
				assertDebugValue( m_isLocked, true );

				auto context = get( m_device )->getContext();
				glLogCall( context
					, glBindTexture
					, m_boundTarget
					, m_boundResource );
				glLogCall( context
					, glUnmapBuffer
					, GL_BUFFER_TARGET_PIXEL_UNPACK );

				for( size_t i = m_beginRegion; i < m_endRegion; ++i )
				{
					updateRegion( context, m_updateRegions[i] );
				}

				glLogCall( context
					, glBindBuffer
					, GL_BUFFER_TARGET_PIXEL_UNPACK
					, 0u );

				if ( m_texture->getMipLevels() > 1
					&& !isCompressedFormat( m_texture->getFormat() ) )
				{
					glLogCall( context
						, glMemoryBarrier
						, GL_MEMORY_BARRIER_TEXTURE_UPDATE );
					glLogCall( context
						, glGenerateMipmap
						, m_boundTarget );
				}

				glLogCall( context
					, glBindTexture
					, m_boundTarget
					, 0u );
				setDebugValue( m_isLocked, false );
			}

		private:
			void doSetImage1D( ContextLock const & context )
			{
				glLogCall( context
					, glTexStorage1D
					, m_boundTarget
					, GLsizei( m_texture->getMipLevels() )
					, getInternalFormat( m_texture->getFormat() )
					, m_texture->getDimensions().width );
			}

			void doSetImage2D( ContextLock const & context )
			{
				glLogCall( context
					, glTexStorage2D
					, m_boundTarget
					, GLsizei( m_texture->getMipLevels() )
					, getInternalFormat( m_texture->getFormat() )
					, m_texture->getDimensions().width
					, m_texture->getDimensions().height );
			}

			void doSetImage3D( ContextLock const & context
				, uint32_t depth )
			{
				glLogCall( context
					, glTexStorage3D
					, m_boundTarget
					, GLsizei( m_texture->getMipLevels() )
					, getInternalFormat( m_texture->getFormat() )
					, m_texture->getDimensions().width
					, m_texture->getDimensions().height
					, depth );
			}

			void doSetImage2DMS( ContextLock const & context )
			{
				glLogCall( context
					, glTexStorage2DMultisample
					, m_boundTarget
					, GLsizei( m_texture->getSamples() )
					, getInternalFormat( m_texture->getFormat() )
					, m_texture->getDimensions().width
					, m_texture->getDimensions().height
					, GL_TRUE );
			}

			void doSetImage3DMS( ContextLock const & context )
			{
				glLogCall( context
					, glTexStorage3DMultisample
					, m_boundTarget
					, GLsizei( m_texture->getSamples() )
					, getInternalFormat( m_texture->getFormat() )
					, m_texture->getDimensions().width
					, m_texture->getDimensions().height
					, m_texture->getDimensions().depth
					, GL_TRUE );
			}

			void doSetupUpdateRegions( uint64_t offset
				, uint64_t size )const
			{
				assert( !m_updateRegions.empty() && "Can't update this texture." );
				auto layerSize = getSize( m_updateRegions[0].imageExtent
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
					throw std::runtime_error{ "Invalid offset and/or size." };
				}
			}

			void updateRegion( ContextLock const & context
				, VkBufferImageCopy const & copyInfo )const
			{
				if ( isCompressedFormat( m_texture->getFormat() ) )
				{
					auto layerSize = getSize( copyInfo.imageExtent
						, m_texture->getFormat()
						, copyInfo.imageSubresource.mipLevel );

					switch ( m_boundTarget )
					{
					case GL_TEXTURE_1D:
						glLogCall( context
							, glCompressedTexSubImage1D
							, m_boundTarget
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageExtent.width
							, m_internal
							, layerSize
							, getBufferOffset( copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_2D:
						glLogCall( context
							, glCompressedTexSubImage2D
							, m_boundTarget
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageOffset.y
							, copyInfo.imageExtent.width
							, copyInfo.imageExtent.height
							, m_internal
							, layerSize
							, getBufferOffset( copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_3D:
						glLogCall( context
							, glCompressedTexSubImage3D
							, m_boundTarget
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageOffset.y
							, copyInfo.imageOffset.z
							, copyInfo.imageExtent.width
							, copyInfo.imageExtent.height
							, copyInfo.imageExtent.depth
							, m_internal
							, layerSize
							, getBufferOffset( copyInfo.bufferOffset ) );

					case GL_TEXTURE_1D_ARRAY:
						glLogCall( context
							, glCompressedTexSubImage2D
							, m_boundTarget
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageSubresource.baseArrayLayer
							, copyInfo.imageExtent.width
							, copyInfo.imageSubresource.layerCount
							, m_internal
							, layerSize
							, getBufferOffset( copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_2D_ARRAY:
						glLogCall( context
							, glCompressedTexSubImage3D
							, m_boundTarget
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageOffset.y
							, copyInfo.imageSubresource.baseArrayLayer
							, copyInfo.imageExtent.width
							, copyInfo.imageExtent.height
							, copyInfo.imageSubresource.layerCount
							, m_internal
							, layerSize
							, getBufferOffset( copyInfo.bufferOffset ) );
						break;
					}
				}
				else
				{
					switch ( m_boundTarget )
					{
					case GL_TEXTURE_1D:
						glLogCall( context
							, glTexSubImage1D
							, m_boundTarget
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageExtent.width
							, m_format
							, m_type
							, getBufferOffset( copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_2D:
						glLogCall( context
							, glTexSubImage2D
							, m_boundTarget
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageOffset.y
							, copyInfo.imageExtent.width
							, copyInfo.imageExtent.height
							, m_format
							, m_type
							, getBufferOffset( copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_3D:
						glLogCall( context
							, glTexSubImage3D
							, m_boundTarget
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageOffset.y
							, copyInfo.imageOffset.z
							, copyInfo.imageExtent.width
							, copyInfo.imageExtent.height
							, copyInfo.imageExtent.depth
							, m_format
							, m_type
							, getBufferOffset( copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_1D_ARRAY:
						glLogCall( context
							, glTexSubImage2D
							, m_boundTarget
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageSubresource.baseArrayLayer
							, copyInfo.imageExtent.width
							, copyInfo.imageSubresource.layerCount
							, m_format
							, m_type
							, getBufferOffset( copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_2D_ARRAY:
						glLogCall( context
							, glTexSubImage3D
							, m_boundTarget
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageOffset.y
							, copyInfo.imageSubresource.baseArrayLayer
							, copyInfo.imageExtent.width
							, copyInfo.imageExtent.height
							, copyInfo.imageSubresource.layerCount
							, m_format
							, m_type
							, getBufferOffset( copyInfo.bufferOffset ) );
						break;
					}
				}
			}

		private:
			Image const * m_texture;
			GlInternal m_internal;
			GlFormat m_format;
			GlType m_type;
			std::vector< VkBufferImageCopy > m_updateRegions;
			GLuint m_pbo{ GL_INVALID_INDEX };
			mutable size_t m_beginRegion{ 0u };
			mutable size_t m_endRegion{ 0u };
		};

		//************************************************************************************************

		class BufferMemory
			: public DeviceMemory::DeviceMemoryImpl
		{
		public:
			BufferMemory( VkDevice device
				, VkMemoryAllocateInfo allocateInfo
				, VkBuffer buffer
				, VkDeviceSize memoryOffset )
				: DeviceMemory::DeviceMemoryImpl{ device, std::move( allocateInfo ), get( buffer)->getInternal(), get( buffer )->getTarget(), memoryOffset }
			{
				auto context = get( m_device )->getContext();
				glLogCall( context
					, glBindBuffer
					, m_boundTarget
					, m_boundResource );
				glLogCall( context
					, glBufferStorage
					, m_boundTarget
					, GLsizeiptr( m_allocateInfo.allocationSize )
					, nullptr
					, GLbitfield( convertMemoryPropertyFlags( m_flags ) ) );
				glLogCall( context
					, glBindBuffer
					, m_boundTarget
					, 0u );
			}

			VkResult lock( uint64_t offset
				, uint64_t size
				, VkMemoryMapFlags flags
				, void ** data )const override
			{
				assert( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local buffer" );
				assertDebugValue( m_isLocked, false );
				auto context = get( m_device )->getContext();
				glLogCall( context
					, glBindBuffer
					, GL_BUFFER_TARGET_COPY_WRITE
					, m_boundResource );
				auto result = glLogCall( context
					, glMapBufferRange
					, GL_BUFFER_TARGET_COPY_WRITE
					, GLintptr( offset )
					, GLsizei( size == WholeSize ? m_allocateInfo.allocationSize : size )
					, m_mapFlags );
				setDebugValue( m_isLocked, result != nullptr );
				*data = result;
				return result
					? VK_SUCCESS
					: VK_ERROR_MEMORY_MAP_FAILED;
			}

			VkResult flush( uint64_t offset
				, uint64_t size )const override
			{
				assert( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local buffer" );
				assertDebugValue( m_isLocked, true );
				auto context = get( m_device )->getContext();
				glLogCall( context
					, glFlushMappedBufferRange
					, GL_BUFFER_TARGET_COPY_WRITE
					, GLintptr( offset )
					, GLsizei( size == WholeSize ? m_allocateInfo.allocationSize : size ) );
				return VK_SUCCESS;
			}

			VkResult invalidate( uint64_t offset
				, uint64_t size )const override
			{
				assert( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local buffer" );
				assertDebugValue( m_isLocked, true );
				auto context = get( m_device )->getContext();
				glLogCall( context
					, glInvalidateBufferSubData
					, GL_BUFFER_TARGET_COPY_WRITE
					, GLintptr( offset )
					, GLsizei( size == WholeSize ? m_allocateInfo.allocationSize : size ) );
				return VK_SUCCESS;
			}

			void unlock()const override
			{
				assert( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local buffer" );
				assertDebugValue( m_isLocked, true );
				auto context = get( m_device )->getContext();
				glLogCall( context
					, glUnmapBuffer
					, GL_BUFFER_TARGET_COPY_WRITE );
				glLogCall( context
					, glBindBuffer
					, GL_BUFFER_TARGET_COPY_WRITE
					, 0u );
				setDebugValue( m_isLocked, false );
			}

		private:
			mutable GLenum m_copyTarget;
		};
	}

	//************************************************************************************************

	DeviceMemory::DeviceMemoryImpl::DeviceMemoryImpl( VkDevice device
		, VkMemoryAllocateInfo allocateInfo
		, GLuint boundResource
		, GLuint boundTarget
		, VkDeviceSize memoryOffset )
		: m_device{ device }
		, m_allocateInfo{ allocateInfo }
		, m_mapFlags{ 0u }
		, m_boundResource{ boundResource }
		, m_boundTarget{ boundTarget }
		, m_memoryOffset{ memoryOffset }
		, m_flags{ getFlags( m_allocateInfo.memoryTypeIndex ) }
	{
		if ( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
		{
			m_mapFlags |= GL_MEMORY_MAP_READ_BIT | GL_MEMORY_MAP_WRITE_BIT | GL_MEMORY_MAP_FLUSH_EXPLICIT_BIT;
		}

		if ( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ) )
		{
			m_mapFlags |= GL_MEMORY_MAP_COHERENT_BIT;
		}
	}

	//************************************************************************************************

	DeviceMemory::DeviceMemory( VkDevice device
		, VkMemoryAllocateInfo allocateInfo )
		: m_allocateInfo{ std::move( allocateInfo ) }
		, m_device{ device }
	{
	}

	DeviceMemory::~DeviceMemory()
	{
	}

	VkResult DeviceMemory::bindToBuffer( VkBuffer buffer
		, VkDeviceSize memoryOffset )
	{
		assert( !m_impl && "Memory object was already bound to a resource object" );
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			m_impl = std::make_unique< BufferMemory >( m_device
				, m_allocateInfo
				, buffer
				, memoryOffset );
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

	VkResult DeviceMemory::bindToImage( VkImage image
		, VkDeviceSize memoryOffset )
	{
		assert( !m_impl && "Memory object was already bound to a resource object" );
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			m_impl = std::make_unique< ImageMemory >( m_device
				, m_allocateInfo
				, image
				, memoryOffset );
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

	VkResult DeviceMemory::lock( uint64_t offset
		, uint64_t size
		, VkMemoryMapFlags flags
		, void ** data )const
	{
		assert( m_impl && "Memory object was not bound to a resource object" );
		return m_impl->lock( offset, size, flags, data );
	}

	VkResult DeviceMemory::flush( uint64_t offset
		, uint64_t size )const
	{
		assert( m_impl && "Memory object was not bound to a resource object" );
		return m_impl->flush( offset, size );
	}

	VkResult DeviceMemory::invalidate( uint64_t offset
		, uint64_t size )const
	{
		assert( m_impl && "Memory object was not bound to a resource object" );
		return m_impl->invalidate( offset, size );
	}

	void DeviceMemory::unlock()const
	{
		assert( m_impl && "Memory object was not bound to a resource object" );
		m_impl->unlock();
	}

	//************************************************************************************************
}
