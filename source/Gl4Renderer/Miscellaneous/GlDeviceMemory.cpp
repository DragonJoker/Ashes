/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Miscellaneous/GlDeviceMemory.hpp"

#include "Command/Commands/GlCopyBufferToImageCommand.hpp"
#include "Core/GlDevice.hpp"
#include "Core/GlInstance.hpp"
#include "Image/GlImage.hpp"

#include <Ashes/Miscellaneous/MemoryRequirements.hpp>

namespace gl_renderer
{
	//*********************************************************************************************

	ashes::MemoryPropertyFlags getFlags( uint32_t memoryTypeIndex )
	{
		assert( memoryTypeIndex < Instance::getMemoryProperties().memoryTypes.size()
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
			ImageMemory( Device const & device
				, VkMemoryAllocateInfo allocateInfo
				, Image const & texture
				, GLuint boundTarget
				, VkImageCreateInfo const & createInfo )
				: DeviceMemory::DeviceMemoryImpl{ device, std::move( allocateInfo ), texture.getInternal(), boundTarget }
				, m_texture{ &texture }
				, m_internal{ getInternalFormat( m_texture->getFormat() ) }
				, m_format{ getFormat( m_internal ) }
				, m_type{ getType( m_internal ) }
			{
				m_texture = &texture;
				auto context = m_device.getContext();
				glLogCall( context
					, glBindTexture
					, m_boundTarget
					, m_boundResource );

				switch ( m_boundTarget )
				{
				case gl_renderer::GL_TEXTURE_1D:
					doSetImage1D( context
						, createInfo.extent.width
						, createInfo );
					break;
				case gl_renderer::GL_TEXTURE_2D:
					doSetImage2D( context
						, createInfo.extent.width
						, createInfo.extent.height
						, createInfo );
					break;
				case gl_renderer::GL_TEXTURE_3D:
					doSetImage3D( context
						, createInfo.extent.width
						, createInfo.extent.height
						, createInfo.extent.depth
						, createInfo );
					break;
				case gl_renderer::GL_TEXTURE_1D_ARRAY:
					doSetImage2D( context
						, createInfo.extent.width
						, createInfo.extent.height
						, createInfo );
					break;
				case gl_renderer::GL_TEXTURE_2D_ARRAY:
					doSetImage3D( context
						, createInfo.extent.width
						, createInfo.extent.height
						, createInfo.arrayLayers
						, createInfo );
					break;
				case gl_renderer::GL_TEXTURE_2D_MULTISAMPLE:
					doSetImage2DMS( context
						, createInfo.extent.width
						, createInfo.extent.height
						, createInfo );
					break;
				case gl_renderer::GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
					doSetImage3DMS( context
						, createInfo.extent.width
						, createInfo.extent.height
						, createInfo.arrayLayers
						, createInfo );
					break;
				default:
					break;
				}

				int levels = 0;
				context->glGetTexParameteriv( m_boundTarget
					, GL_TEXTURE_IMMUTABLE_LEVELS
					, &levels );
				assert( levels == createInfo.mipLevels );
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
				if ( checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
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
						, GLbitfield( convert( m_flags ) ) );
					glLogCall( context
						, glBindBuffer
						, GL_BUFFER_TARGET_PIXEL_UNPACK
						, 0u );

					// Prepare update regions, layer by layer.
					uint32_t offset = 0;
					ashes::BufferImageCopy bufferCopyRegion = {};
					bufferCopyRegion.imageSubresource.aspectMask = getAspectMask( m_texture->getFormat() );
					bufferCopyRegion.imageSubresource.mipLevel = 0u;
					bufferCopyRegion.imageSubresource.layerCount = 1u;
					bufferCopyRegion.imageExtent = m_texture->getDimensions();
					bufferCopyRegion.levelSize = uint32_t( m_allocateInfo.allocationSize / m_texture->getLayerCount() );

					for ( uint32_t layer = 0; layer < m_texture->getLayerCount(); layer++ )
					{
						bufferCopyRegion.imageSubresource.baseArrayLayer = layer;
						bufferCopyRegion.bufferOffset = offset;
						m_updateRegions.push_back( bufferCopyRegion );
						offset += bufferCopyRegion.levelSize;
					}
				}
			}

			~ImageMemory()
			{
				if ( m_pbo != GL_INVALID_INDEX )
				{
					auto context = m_device.getContext();
					context->glDeleteBuffers( 1u, &m_pbo );
				}
			}

			uint8_t * lock( uint64_t offset
				, uint64_t size
				, ashes::MemoryMapFlags flags )const override
			{
				assert( checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local texture" );
				auto context = m_device.getContext();
				glLogCall( context
					, glBindBuffer
					, GL_BUFFER_TARGET_PIXEL_UNPACK
					, m_pbo );
				doSetupUpdateRegions( offset, size );
				auto result = glLogCall( context
					, glMapBufferRange
					, GL_BUFFER_TARGET_PIXEL_UNPACK
					, GLintptr( offset )
					, GLsizei( size == ashes::WholeSize ? m_allocateInfo.allocationSize : size )
					, m_mapFlags );
				assertDebugValue( m_isLocked, false );
				setDebugValue( m_isLocked, result != nullptr );
				return reinterpret_cast< uint8_t * >( result );
			}

			void flush( uint64_t offset
				, uint64_t size )const override
			{
				assert( checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local texture" );
				assertDebugValue( m_isLocked, true );
				auto context = m_device.getContext();
				glLogCall( context
					, glFlushMappedBufferRange
					, GL_BUFFER_TARGET_PIXEL_UNPACK
					, GLintptr( offset )
					, GLsizei( size == ashes::WholeSize ? m_allocateInfo.allocationSize : size ) );
			}

			void invalidate( uint64_t offset
				, uint64_t size )const override
			{
				assert( checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local texture" );
				assertDebugValue( m_isLocked, true );
				auto context = m_device.getContext();
				glLogCall( context
					, glInvalidateBufferSubData
					, GL_BUFFER_TARGET_PIXEL_UNPACK
					, GLintptr( offset )
					, GLsizei( size == ashes::WholeSize ? m_allocateInfo.allocationSize : size ) );
			}

			void unlock()const override
			{
				assert( checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local texture" );
				assertDebugValue( m_isLocked, true );

				auto context = m_device.getContext();
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

				if ( m_texture->getMipmapLevels() > 1
					&& !ashes::isCompressedFormat( m_texture->getFormat() ) )
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
			void doSetImage1D( ContextLock const & context
				, uint32_t width
				, VkImageCreateInfo const & createInfo )
			{
				glLogCall( context
					, glTexStorage1D
					, m_boundTarget
					, GLsizei( createInfo.mipLevels )
					, getInternalFormat( createInfo.format )
					, width );
			}

			void doSetImage2D( ContextLock const & context
				, uint32_t width
				, uint32_t height
				, VkImageCreateInfo const & createInfo )
			{
				glLogCall( context
					, glTexStorage2D
					, m_boundTarget
					, GLsizei( createInfo.mipLevels )
					, getInternalFormat( createInfo.format )
					, width
					, height );
			}

			void doSetImage3D( ContextLock const & context
				, uint32_t width
				, uint32_t height
				, uint32_t depth
				, VkImageCreateInfo const & createInfo )
			{
				glLogCall( context
					, glTexStorage3D
					, m_boundTarget
					, GLsizei( createInfo.mipLevels )
					, getInternalFormat( createInfo.format )
					, width
					, height
					, depth );
			}

			void doSetImage2DMS( ContextLock const & context
				, uint32_t width
				, uint32_t height
				, VkImageCreateInfo const & createInfo )
			{
				glLogCall( context
					, glTexStorage2DMultisample
					, m_boundTarget
					, GLsizei( createInfo.samples )
					, getInternalFormat( createInfo.format )
					, width
					, height
					, GL_TRUE );
			}

			void doSetImage3DMS( ContextLock const & context
				, uint32_t width
				, uint32_t height
				, uint32_t depth
				, VkImageCreateInfo const & createInfo )
			{
				glLogCall( context
					, glTexStorage3DMultisample
					, m_boundTarget
					, GLsizei( createInfo.samples )
					, getInternalFormat( createInfo.format )
					, width
					, height
					, depth
					, GL_TRUE );
			}

			void doSetupUpdateRegions( uint64_t offset
				, uint64_t size )const
			{
				assert( !m_updateRegions.empty() && "Can't update this texture." );
				auto layerSize = m_updateRegions[0].levelSize;
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
				, ashes::BufferImageCopy const & copyInfo )const
			{
				if ( ashes::isCompressedFormat( m_texture->getFormat() ) )
				{
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
							, copyInfo.levelSize
							, BufferOffset( copyInfo.bufferOffset ) );
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
							, copyInfo.levelSize
							, BufferOffset( copyInfo.bufferOffset ) );
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
							, copyInfo.levelSize
							, BufferOffset( copyInfo.bufferOffset ) );

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
							, copyInfo.levelSize
							, BufferOffset( copyInfo.bufferOffset ) );
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
							, copyInfo.levelSize
							, BufferOffset( copyInfo.bufferOffset ) );
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
							, BufferOffset( copyInfo.bufferOffset ) );
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
							, BufferOffset( copyInfo.bufferOffset ) );
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
							, BufferOffset( copyInfo.bufferOffset ) );
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
							, BufferOffset( copyInfo.bufferOffset ) );
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
							, BufferOffset( copyInfo.bufferOffset ) );
						break;
					}
				}
			}

		private:
			Image const * m_texture;
			GlInternal m_internal;
			GlFormat m_format;
			GlType m_type;
			std::vector< ashes::BufferImageCopy > m_updateRegions;
			GLuint m_pbo{ GL_INVALID_INDEX };
			mutable size_t m_beginRegion{ 0u };
			mutable size_t m_endRegion{ 0u };
		};

		//************************************************************************************************

		class BufferMemory
			: public DeviceMemory::DeviceMemoryImpl
		{
		public:
			BufferMemory( Device const & device
				, VkMemoryAllocateInfo allocateInfo
				, GLuint boundResource
				, GLuint boundTarget )
				: DeviceMemory::DeviceMemoryImpl{ device, std::move( allocateInfo ), boundResource, boundTarget }
			{
				auto context = m_device.getContext();
				glLogCall( context
					, glBindBuffer
					, m_boundTarget
					, m_boundResource );
				glLogCall( context
					, glBufferStorage
					, m_boundTarget
					, GLsizeiptr( m_allocateInfo.allocationSize )
					, nullptr
					, GLbitfield( convert( m_flags ) ) );
				glLogCall( context
					, glBindBuffer
					, m_boundTarget
					, 0u );
			}

			uint8_t * lock( uint64_t offset
				, uint64_t size
				, ashes::MemoryMapFlags flags )const override
			{
				assert( checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local buffer" );
				assertDebugValue( m_isLocked, false );
				auto context = m_device.getContext();
				glLogCall( context
					, glBindBuffer
					, GL_BUFFER_TARGET_COPY_WRITE
					, m_boundResource );
				auto result = glLogCall( context
					, glMapBufferRange
					, GL_BUFFER_TARGET_COPY_WRITE
					, GLintptr( offset )
					, GLsizei( size == ashes::WholeSize ? m_allocateInfo.allocationSize : size )
					, m_mapFlags );
				setDebugValue( m_isLocked, result != nullptr );
				return reinterpret_cast< uint8_t * >( result );
			}

			void flush( uint64_t offset
				, uint64_t size )const override
			{
				assert( checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local buffer" );
				assertDebugValue( m_isLocked, true );
				auto context = m_device.getContext();
				glLogCall( context
					, glFlushMappedBufferRange
					, GL_BUFFER_TARGET_COPY_WRITE
					, GLintptr( offset )
					, GLsizei( size == ashes::WholeSize ? m_allocateInfo.allocationSize : size ) );
			}

			void invalidate( uint64_t offset
				, uint64_t size )const override
			{
				assert( checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local buffer" );
				assertDebugValue( m_isLocked, true );
				auto context = m_device.getContext();
				glLogCall( context
					, glInvalidateBufferSubData
					, GL_BUFFER_TARGET_COPY_WRITE
					, GLintptr( offset )
					, GLsizei( size == ashes::WholeSize ? m_allocateInfo.allocationSize : size ) );
			}

			void unlock()const override
			{
				assert( checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
					&& "Unsupported action on a device local buffer" );
				assertDebugValue( m_isLocked, true );
				auto context = m_device.getContext();
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

	DeviceMemory::DeviceMemoryImpl::DeviceMemoryImpl( Device const & device
		, VkMemoryAllocateInfo allocateInfo
		, GLuint boundResource
		, GLuint boundTarget )
		: m_device{ device }
		, m_allocateInfo{ allocateInfo }
		, m_mapFlags{ 0u }
		, m_boundResource{ boundResource }
		, m_boundTarget{ boundTarget }
		, m_flags{ getFlags( m_allocateInfo.memoryTypeIndex ) }
	{
		if ( checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
		{
			m_mapFlags |= GL_MEMORY_MAP_READ_BIT | GL_MEMORY_MAP_WRITE_BIT | GL_MEMORY_MAP_FLUSH_EXPLICIT_BIT;
		}

		if ( checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ) )
		{
			m_mapFlags |= GL_MEMORY_MAP_COHERENT_BIT;
		}
	}

	//************************************************************************************************

	DeviceMemory::DeviceMemory( Device const & device
		, VkMemoryAllocateInfo allocateInfo )
		: ashes::DeviceMemory{ device, std::move( allocateInfo ) }
		, m_device{ device }
	{
	}

	DeviceMemory::~DeviceMemory()
	{
	}

	void DeviceMemory::bindToBuffer( GLuint resource, GLenum target )
	{
		assert( !m_impl && "Memory object was already bound to a resource object" );
		m_impl = std::make_unique< BufferMemory >( m_device
			, m_allocateInfo
			, resource
			, target );
	}

	void DeviceMemory::bindToImage( Image const & texture
		, GLenum target
		, VkImageCreateInfo const & createInfo )
	{
		assert( !m_impl && "Memory object was already bound to a resource object" );
		m_impl = std::make_unique< ImageMemory >( m_device
			, m_allocateInfo
			, texture
			, target
			, createInfo );
	}

	uint8_t * DeviceMemory::lock( uint64_t offset
		, uint64_t size
		, ashes::MemoryMapFlags flags )const
	{
		assert( m_impl && "Memory object was not bound to a resource object" );
		return m_impl->lock( offset, size, flags );
	}

	void DeviceMemory::flush( uint64_t offset
		, uint64_t size )const
	{
		assert( m_impl && "Memory object was not bound to a resource object" );
		m_impl->flush( offset, size );
	}

	void DeviceMemory::invalidate( uint64_t offset
		, uint64_t size )const
	{
		assert( m_impl && "Memory object was not bound to a resource object" );
		m_impl->invalidate( offset, size );
	}

	void DeviceMemory::unlock()const
	{
		assert( m_impl && "Memory object was not bound to a resource object" );
		m_impl->unlock();
	}

	//************************************************************************************************
}
