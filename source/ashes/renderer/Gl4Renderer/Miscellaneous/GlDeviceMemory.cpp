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
			ImageMemory( VkDeviceMemory parent
				, VkDevice device
				, VkMemoryAllocateInfo allocateInfo
				, VkImage texture
				, VkDeviceSize memoryOffset
				, GLuint buffer )
				: DeviceMemory::DeviceMemoryImpl{ parent, device, std::move( allocateInfo ), get( texture )->getInternal(), get( texture )->getTarget(), memoryOffset, buffer }
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

			VkResult lock( ContextLock const & context
				, VkDeviceSize offset
				, VkDeviceSize size
				, void ** data )const override
			{
				assert( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local texture" );

				auto copySize = size == WholeSize
					? m_allocateInfo.allocationSize
					: size;
				glLogCall( context
					, glBindBuffer
					, GL_BUFFER_TARGET_PIXEL_UNPACK
					, m_buffer );
				doSetupUpdateRegions( offset, size );
				auto result = glLogCall( context
					, glMapBufferRange
					, GL_BUFFER_TARGET_PIXEL_UNPACK
					, GLintptr( offset )
					, GLsizei( copySize )
					, m_mapFlags );
				*data = result;
				return result
					? VK_SUCCESS
					: VK_ERROR_MEMORY_MAP_FAILED;
			}

			void unlock( ContextLock const & context )const override
			{
				assert( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local texture" );

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
			}

		private:
			void doSetImage1D( ContextLock const & context )
			{
				glLogCall( context
					, glTexStorage1D
					, m_boundTarget
					, GLsizei( m_texture->getMipLevels() )
					, m_internal
					, m_texture->getDimensions().width );
			}

			void doSetImage2D( ContextLock const & context )
			{
				glLogCall( context
					, glTexStorage2D
					, m_boundTarget
					, GLsizei( m_texture->getMipLevels() )
					, m_internal
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
					, m_internal
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
					, m_internal
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
					, m_internal
					, m_texture->getDimensions().width
					, m_texture->getDimensions().height
					, m_texture->getDimensions().depth
					, GL_TRUE );
			}

			void doSetupUpdateRegions( VkDeviceSize offset
				, VkDeviceSize size )const
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
							, GLsizei( layerSize )
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
							, GLsizei( layerSize )
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
							, GLsizei( layerSize )
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
							, GLsizei( layerSize )
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
							, GLsizei( layerSize )
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
			BufferMemory( VkDeviceMemory parent
				, VkDevice device
				, VkMemoryAllocateInfo allocateInfo
				, GlBufferTarget target
				, VkDeviceSize memoryOffset
				, GLuint buffer )
				: DeviceMemory::DeviceMemoryImpl{ parent, device, std::move( allocateInfo ), buffer, target, memoryOffset, buffer }
			{
				if ( !checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
				{
					auto context = get( m_device )->getContext();
					glLogCall( context
						, glGenBuffers
						, 1u
						, &m_boundResource );
					glLogCall( context
						, glBindBuffer
						, GlBufferTarget( m_boundTarget )
						, m_boundResource );
					glLogCall( context
						, glBufferStorage
						, GlBufferTarget( m_boundTarget )
						, GLsizeiptr( m_allocateInfo.allocationSize )
						, nullptr
						, convertMemoryPropertyFlags( m_flags ) );
					glLogCall( context
						, glBindBuffer
						, GlBufferTarget( m_boundTarget )
						, 0u );
				}
			}

			~BufferMemory()
			{
				if ( m_boundResource != m_buffer )
				{
					auto context = get( m_device )->getContext();
					glLogCall( context
						, glDeleteBuffers
						, 1u
						, &m_boundResource );
				}
			}

			VkResult lock( ContextLock const & context
				, VkDeviceSize offset
				, VkDeviceSize size
				, void ** data )const override
			{
				assert( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local buffer" );

				glLogCall( context
					, glBindBuffer
					, GlBufferTarget( m_boundTarget )
					, m_boundResource );
				auto result = glLogCall( context
					, glMapBufferRange
					, GlBufferTarget( m_boundTarget )
					, GLintptr( offset )
					, GLsizei( size == WholeSize ? m_allocateInfo.allocationSize : size )
					, m_mapFlags );
				*data = result;
				return result
					? VK_SUCCESS
					: VK_ERROR_MEMORY_MAP_FAILED;
			}

			void unlock( ContextLock const & context )const override
			{
				assert( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local buffer" );

				glLogCall( context
					, glUnmapBuffer
					, GlBufferTarget( m_boundTarget ) );
				glLogCall( context
					, glBindBuffer
					, GlBufferTarget( m_boundTarget )
					, 0u );
			}

		private:
			mutable GLenum m_copyTarget;
		};
	}

	//************************************************************************************************

	DeviceMemory::DeviceMemoryImpl::DeviceMemoryImpl( VkDeviceMemory parent
		, VkDevice device
		, VkMemoryAllocateInfo allocateInfo
		, GLuint boundResource
		, GLuint boundTarget
		, VkDeviceSize memoryOffset
		, GLuint buffer )
		: m_device{ device }
		, m_allocateInfo{ allocateInfo }
		, m_mapFlags{ 0u }
		, m_boundResource{ boundResource }
		, m_boundTarget{ boundTarget }
		, m_memoryOffset{ memoryOffset }
		, m_buffer{ buffer }
		, m_flags{ getFlags( m_allocateInfo.memoryTypeIndex ) }
	{
		if ( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
		{
			m_mapFlags |= GL_MEMORY_MAP_READ_BIT;
			m_mapFlags |= GL_MEMORY_MAP_WRITE_BIT;
			//m_mapFlags |= GL_MEMORY_MAP_PERSISTENT_BIT;

			//if ( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ) )
			//{
			//	m_mapFlags |= GL_MEMORY_MAP_COHERENT_BIT;
			//}
			//else
			//{
			//	m_mapFlags |= GL_MEMORY_MAP_FLUSH_EXPLICIT_BIT;
			//}
		}
	}

	void DeviceMemory::DeviceMemoryImpl::upload( ContextLock const & context
		, ByteArray const & data
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		if ( size == WholeSize )
		{
			assert( offset == 0ull );
			size = m_allocateInfo.allocationSize;
		}

		void * dst{ nullptr };

		if ( lock( context, offset, size, &dst ) == VK_SUCCESS )
		{
			std::memcpy( dst, data.data() + offset, size );
			unlock( context );
		}
	}

	void DeviceMemory::DeviceMemoryImpl::download( ContextLock const & context
		, ByteArray & data
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		if ( size == WholeSize )
		{
			assert( offset == 0ull );
			size = m_allocateInfo.allocationSize;
		}

		void * src{ nullptr };

		if ( lock( context, offset, size, &src ) == VK_SUCCESS )
		{
			std::memcpy( data.data() + offset, src, size );
			unlock( context );
		}
	}

	//************************************************************************************************

	DeviceMemory::DeviceMemory( VkDevice device
		, VkMemoryAllocateInfo allocateInfo )
		: m_device{ device }
		, m_allocateInfo{ std::move( allocateInfo ) }
		, m_flags{ getFlags( m_allocateInfo.memoryTypeIndex ) }
	{
		if ( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
		{
			m_mapFlags |= GL_MEMORY_MAP_READ_BIT;
			m_mapFlags |= GL_MEMORY_MAP_WRITE_BIT;
			//m_mapFlags |= GL_MEMORY_MAP_PERSISTENT_BIT;

			//if ( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ) )
			//{
			//	m_mapFlags |= GL_MEMORY_MAP_COHERENT_BIT;
			//}
			//else
			//{
			//	m_mapFlags |= GL_MEMORY_MAP_FLUSH_EXPLICIT_BIT;
			//}

			m_data.resize( allocateInfo.allocationSize );

			auto context = get( m_device )->getContext();
			glLogCall( context
				, glGenBuffers
				, 1u
				, &m_buffer );
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_COPY_WRITE
				, m_buffer );
			glLogCall( context
				, glBufferStorage
				, GL_BUFFER_TARGET_COPY_WRITE
				, GLsizeiptr( m_allocateInfo.allocationSize )
				, nullptr
				, convertMemoryPropertyFlags( m_flags ) );
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_COPY_WRITE
				, 0u );
			m_impl = std::make_unique< BufferMemory >( get( this )
				, m_device
				, m_allocateInfo
				, GL_BUFFER_TARGET_COPY_WRITE
				, 0u
				, m_buffer );
		}
	}

	DeviceMemory::~DeviceMemory()
	{
		onDestroy( m_buffer );
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glDeleteBuffers
			, 1u
			, &m_buffer );
	}

	VkResult DeviceMemory::bindToBuffer( VkBuffer buffer
		, VkDeviceSize memoryOffset )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			m_impl = std::make_unique< BufferMemory >( get( this )
				, m_device
				, m_allocateInfo
				, get( buffer )->getTarget()
				, memoryOffset
				, m_buffer );
			get( buffer )->setInternal( m_impl->getInternal() );
			get( buffer )->setMemory( get( this ) );
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
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			m_impl = std::make_unique< ImageMemory >( get( this )
				, m_device
				, m_allocateInfo
				, image
				, memoryOffset
				, m_buffer );
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

	void DeviceMemory::upload( ContextLock const & context
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		m_impl->upload( context, m_data, offset, size );
	}

	void DeviceMemory::download( ContextLock const & context
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		m_impl->download( context, m_data, offset, size );
	}

	VkResult DeviceMemory::lock( ContextLock const & context
		, VkDeviceSize offset
		, VkDeviceSize size
		, VkMemoryMapFlags flags
		, void ** data )const
	{
		assert( !m_mapped && "VkDeviceMemory should not be mapped" );
		*data = m_data.data() + offset;
		m_mappedOffset = offset;
		m_mappedSize = size == ~( 0ull )
			? m_allocateInfo.allocationSize
			: size;
		m_mapped = *data != nullptr;
		m_dirty = true;
		return VK_SUCCESS;
	}

	VkResult DeviceMemory::flush( ContextLock const & context
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		assert( m_mapped && "VkDeviceMemory should be mapped" );
		upload( context, offset, size );
		return VK_SUCCESS;
	}

	VkResult DeviceMemory::invalidate( ContextLock const & context
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		assert( m_mapped && "VkDeviceMemory should be mapped" );
		m_dirty = true;
		return VK_SUCCESS;
	}

	void DeviceMemory::unlock( ContextLock const & context )const
	{
		assert( m_mapped && "VkDeviceMemory should be mapped" );
		m_mapped = false;
		upload( context, m_mappedOffset, m_mappedSize );
	}

	//************************************************************************************************
}
