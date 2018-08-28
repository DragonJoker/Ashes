/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#include "Miscellaneous/GlDeviceMemory.hpp"

#include "Command/Commands/GlCopyBufferToImageCommand.hpp"
#include "Core/GlDevice.hpp"
#include "Image/GlTexture.hpp"

#include <Miscellaneous/MemoryRequirements.hpp>

namespace gl_renderer
{
	//************************************************************************************************

	namespace
	{
		class ImageMemory
			: public DeviceMemory::DeviceMemoryImpl
		{
		public:
			ImageMemory( Device const & device
				, renderer::MemoryRequirements const & requirements
				, renderer::MemoryPropertyFlags flags
				, Texture const & texture
				, GLuint boundTarget
				, renderer::ImageCreateInfo const & createInfo )
				: DeviceMemory::DeviceMemoryImpl{ device, requirements, flags, texture.getImage(), boundTarget }
				, m_texture{ &texture }
				, m_internal{ getInternal( m_texture->getFormat() ) }
				, m_format{ getFormat( m_internal ) }
				, m_type{ getType( m_internal ) }
			{
				m_texture = &texture;
				glLogCall( m_device.getContext(), glBindTexture, m_boundTarget, m_boundResource );

				switch ( m_boundTarget )
				{
				case gl_renderer::GL_TEXTURE_1D:
					doSetImage1D( createInfo.extent.width
						, createInfo );
					break;
				case gl_renderer::GL_TEXTURE_2D:
					doSetImage2D( createInfo.extent.width
						, createInfo.extent.height
						, createInfo );
					break;
				case gl_renderer::GL_TEXTURE_3D:
					doSetImage3D( createInfo.extent.width
						, createInfo.extent.height
						, createInfo.extent.depth
						, createInfo );
					break;
				case gl_renderer::GL_TEXTURE_1D_ARRAY:
					doSetImage2D( createInfo.extent.width
						, createInfo.extent.height
						, createInfo );
					break;
				case gl_renderer::GL_TEXTURE_2D_ARRAY:
					doSetImage3D( createInfo.extent.width
						, createInfo.extent.height
						, createInfo.arrayLayers
						, createInfo );
					break;
				case gl_renderer::GL_TEXTURE_CUBE:
					doSetImageCubeFace( createInfo.extent.width
						, createInfo.extent.height
						, 0
						, createInfo );
					doSetImageCubeFace( createInfo.extent.width
						, createInfo.extent.height
						, 1
						, createInfo );
					doSetImageCubeFace( createInfo.extent.width
						, createInfo.extent.height
						, 2
						, createInfo );
					doSetImageCubeFace( createInfo.extent.width
						, createInfo.extent.height
						, 3
						, createInfo );
					doSetImageCubeFace( createInfo.extent.width
						, createInfo.extent.height
						, 4
						, createInfo );
					doSetImageCubeFace( createInfo.extent.width
						, createInfo.extent.height
						, 5
						, createInfo );
					break;
				case gl_renderer::GL_TEXTURE_CUBE_ARRAY:
					doSetImage3D( createInfo.extent.width
						, createInfo.extent.height
						, createInfo.arrayLayers
						, createInfo );
					break;
				case gl_renderer::GL_TEXTURE_2D_MULTISAMPLE:
					doSetImage2DMS( createInfo.extent.width
						, createInfo.extent.height
						, createInfo );
					break;
				case gl_renderer::GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
					doSetImage3DMS( createInfo.extent.width
						, createInfo.extent.height
						, createInfo.arrayLayers
						, createInfo );
					break;
				default:
					break;
				}

				glLogCall( m_device.getContext(), glGenerateMipmap, m_boundTarget );
				glLogCall( m_device.getContext(), glBindTexture, m_boundTarget, 0 );

				// If the texture is visible to the host, we'll need a PBO to map it to RAM.
				if ( checkFlag( flags, renderer::MemoryPropertyFlag::eHostVisible ) )
				{
					glLogCall( m_device.getContext(), glGenBuffers, 1u, &m_pbo );
					// Initialise Upload PBO.
					glLogCall( m_device.getContext(), glBindBuffer, GL_BUFFER_TARGET_PIXEL_UNPACK, m_pbo );
					glLogCall( m_device.getContext(), glBufferData, m_pbo, GLsizeiptr( m_requirements.size ), nullptr, GLbitfield( convert( flags ) ) );
					glLogCall( m_device.getContext(), glBindBuffer, GL_BUFFER_TARGET_PIXEL_UNPACK, 0u );

					// Prepare update regions, layer by layer.
					uint32_t offset = 0;
					renderer::BufferImageCopy bufferCopyRegion = {};
					bufferCopyRegion.imageSubresource.aspectMask = getAspectMask( m_texture->getFormat() );
					bufferCopyRegion.imageSubresource.mipLevel = 0u;
					bufferCopyRegion.imageSubresource.layerCount = 1u;
					bufferCopyRegion.imageExtent = m_texture->getDimensions();
					bufferCopyRegion.levelSize = uint32_t( m_requirements.size / m_texture->getLayerCount() );

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
					m_device.getContext().glDeleteBuffers( 1u, &m_pbo );
				}
			}

			uint8_t * lock( uint32_t offset
				, uint32_t size
				, renderer::MemoryMapFlags flags )const override
			{
				assert( checkFlag( m_flags, renderer::MemoryPropertyFlag::eHostVisible ) && "Unsupported action on a device local texture" );
				glLogCall( m_device.getContext(), glBindBuffer, GL_BUFFER_TARGET_PIXEL_UNPACK, m_pbo );
				doSetupUpdateRegions( offset, size );
				auto result = glLogCall( m_device.getContext(), glMapBufferRange, GL_BUFFER_TARGET_PIXEL_UNPACK, offset, size, m_mapFlags );
				assertDebugValue( m_isLocked, false );
				setDebugValue( m_isLocked, result != nullptr );
				return reinterpret_cast< uint8_t * >( result );
			}

			void flush( uint32_t offset
				, uint32_t size )const override
			{
				assert( checkFlag( m_flags, renderer::MemoryPropertyFlag::eHostVisible ) && "Unsupported action on a device local texture" );
				assertDebugValue( m_isLocked, true );
				glLogCall( m_device.getContext(), glFlushMappedBufferRange, GL_BUFFER_TARGET_PIXEL_UNPACK, offset, size );
			}

			void invalidate( uint32_t offset
				, uint32_t size )const override
			{
				assert( checkFlag( m_flags, renderer::MemoryPropertyFlag::eHostVisible ) && "Unsupported action on a device local texture" );
				assertDebugValue( m_isLocked, true );
			}

			void unlock()const override
			{
				assert( checkFlag( m_flags, renderer::MemoryPropertyFlag::eHostVisible ) && "Unsupported action on a device local texture" );
				assertDebugValue( m_isLocked, true );

				glLogCall( m_device.getContext(), glBindTexture, m_boundTarget, m_boundResource );
				glLogCall( m_device.getContext(), glUnmapBuffer, GL_BUFFER_TARGET_PIXEL_UNPACK );

				for( size_t i = m_beginRegion; i < m_endRegion; ++i )
				{
					updateRegion( m_updateRegions[i] );
				}

				glLogCall( m_device.getContext(), glBindBuffer, GL_BUFFER_TARGET_PIXEL_UNPACK, 0u );

				if ( m_texture->getMipmapLevels() > 1
					&& !renderer::isCompressedFormat( m_texture->getFormat() ) )
				{
					if ( m_device.getContext().glMemoryBarrier_ARB )
					{
						glLogCall( m_device.getContext(), glMemoryBarrier_ARB, GL_MEMORY_BARRIER_TEXTURE_UPDATE );
					}

					glLogCall( m_device.getContext(), glGenerateMipmap, m_boundTarget );
				}

				glLogCall( m_device.getContext(), glBindTexture, m_boundTarget, 0u );
				setDebugValue( m_isLocked, false );
			}

		private:
			void doSetImage1D( uint32_t width
				, renderer::ImageCreateInfo const & createInfo )
			{
				glLogCall( m_device.getContext(), glTexImage1D
					, m_boundTarget
					, 0u
					, m_internal
					, width
					, 0
					, m_format
					, m_type
					, nullptr );
			}

			void doSetImage2D( uint32_t width
				, uint32_t height
				, renderer::ImageCreateInfo const & createInfo )
			{
				glLogCall( m_device.getContext(), glTexImage2D
					, m_boundTarget
					, 0u
					, m_internal
					, width
					, height
					, 0
					, m_format
					, m_type
					, nullptr );
			}

			void doSetImageCubeFace( uint32_t width
				, uint32_t height
				, int face
				, renderer::ImageCreateInfo const & createInfo )
			{
				glLogCall( m_device.getContext(), glTexImage2D
					, GL_TEXTURE_CUBE_POSITIVE_X + face
					, 0u
					, m_internal
					, width
					, height
					, 0
					, m_format
					, m_type
					, nullptr );
			}

			void doSetImage3D( uint32_t width
				, uint32_t height
				, uint32_t depth
				, renderer::ImageCreateInfo const & createInfo )
			{
				glLogCall( m_device.getContext(), glTexImage3D
					, m_boundTarget
					, 0u
					, m_internal
					, width
					, height
					, depth
					, 0
					, m_format
					, m_type
					, nullptr );
			}

			void doSetImage2DMS( uint32_t width
				, uint32_t height
				, renderer::ImageCreateInfo const & createInfo )
			{
				glLogCall( m_device.getContext(), glTexImage2DMultisample
					, m_boundTarget
					, GLsizei( createInfo.samples )
					, m_internal
					, width
					, height
					, GL_TRUE );
			}

			void doSetImage3DMS( uint32_t width
				, uint32_t height
				, uint32_t depth
				, renderer::ImageCreateInfo const & createInfo )
			{
				glLogCall( m_device.getContext(), glTexImage3DMultisample
					, m_boundTarget
					, GLsizei( createInfo.samples )
					, m_internal
					, width
					, height
					, depth
					, GL_TRUE );
			}

			void doSetupUpdateRegions( uint32_t offset
				, uint32_t size )const
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

			void updateRegion( renderer::BufferImageCopy const & copyInfo )const
			{
				if ( renderer::isCompressedFormat( m_texture->getFormat() ) )
				{
					switch ( m_boundTarget )
					{
					case GL_TEXTURE_1D:
						glLogCall( m_device.getContext(), glCompressedTexSubImage1D
							, m_boundTarget
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageExtent.width
							, m_internal
							, copyInfo.levelSize
							, BufferOffset( copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_2D:
						glLogCall( m_device.getContext(), glCompressedTexSubImage2D
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
						glLogCall( m_device.getContext(), glCompressedTexSubImage3D
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
						glLogCall( m_device.getContext(), glCompressedTexSubImage2D
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
						glLogCall( m_device.getContext(), glCompressedTexSubImage3D
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
						glLogCall( m_device.getContext(), glTexSubImage1D
							, m_boundTarget
							, copyInfo.imageSubresource.mipLevel
							, copyInfo.imageOffset.x
							, copyInfo.imageExtent.width
							, m_format
							, m_type
							, BufferOffset( copyInfo.bufferOffset ) );
						break;

					case GL_TEXTURE_2D:
						glLogCall( m_device.getContext(), glTexSubImage2D
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
						glLogCall( m_device.getContext(), glTexSubImage3D
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
						glLogCall( m_device.getContext(), glTexSubImage2D
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
						glLogCall( m_device.getContext(), glTexSubImage3D
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
			Texture const * m_texture;
			GlInternal m_internal;
			GlFormat m_format;
			GlType m_type;
			std::vector< renderer::BufferImageCopy > m_updateRegions;
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
				, renderer::MemoryRequirements const & requirements
				, renderer::MemoryPropertyFlags flags
				, GLuint boundResource
				, GLuint boundTarget )
				: DeviceMemory::DeviceMemoryImpl{ device, requirements, flags, boundResource, boundTarget }
			{
				glLogCall( m_device.getContext(), glBindBuffer, m_boundTarget, m_boundResource );
				glLogCall( m_device.getContext(), glBufferData, m_boundTarget, GLsizeiptr( m_requirements.size ), nullptr, GLbitfield( convert( flags ) ) );
				glLogCall( m_device.getContext(), glBindBuffer, m_boundTarget, 0u );
			}

			uint8_t * lock( uint32_t offset
				, uint32_t size
				, renderer::MemoryMapFlags flags )const override
			{
				assert( checkFlag( m_flags, renderer::MemoryPropertyFlag::eHostVisible ) && "Unsupported action on a device local buffer" );
				assertDebugValue( m_isLocked, false );
				glLogCall( m_device.getContext(), glBindBuffer, GL_BUFFER_TARGET_COPY_WRITE, m_boundResource );
				auto result = glLogCall( m_device.getContext(), glMapBufferRange, GL_BUFFER_TARGET_COPY_WRITE, offset, size, m_mapFlags );
				setDebugValue( m_isLocked, result != nullptr );
				return reinterpret_cast< uint8_t * >( result );
			}

			void flush( uint32_t offset
				, uint32_t size )const override
			{
				assert( checkFlag( m_flags, renderer::MemoryPropertyFlag::eHostVisible ) && "Unsupported action on a device local buffer" );
				assertDebugValue( m_isLocked, true );
				glLogCall( m_device.getContext(), glFlushMappedBufferRange, GL_BUFFER_TARGET_COPY_WRITE, offset, size );
			}

			void invalidate( uint32_t offset
				, uint32_t size )const override
			{
				assert( checkFlag( m_flags, renderer::MemoryPropertyFlag::eHostVisible ) && "Unsupported action on a device local buffer" );
				assertDebugValue( m_isLocked, true );
			}

			void unlock()const override
			{
				assert( checkFlag( m_flags, renderer::MemoryPropertyFlag::eHostVisible ) && "Unsupported action on a device local buffer" );
				assertDebugValue( m_isLocked, true );
				glLogCall( m_device.getContext(), glUnmapBuffer, GL_BUFFER_TARGET_COPY_WRITE );
				glLogCall( m_device.getContext(), glBindBuffer, GL_BUFFER_TARGET_COPY_WRITE, 0u );
				setDebugValue( m_isLocked, false );
			}

		private:
			mutable GLenum m_copyTarget;
		};
	}

	//************************************************************************************************

	DeviceMemory::DeviceMemoryImpl::DeviceMemoryImpl( Device const & device
		, renderer::MemoryRequirements const & requirements
		, renderer::MemoryPropertyFlags flags
		, GLuint boundResource
		, GLuint boundTarget )
		: m_device{ device }
		, m_requirements{ requirements }
		, m_mapFlags{ 0u }
		, m_boundResource{ boundResource }
		, m_boundTarget{ boundTarget }
		, m_flags{ flags }
	{
		if ( checkFlag( flags, renderer::MemoryPropertyFlag::eHostVisible ) )
		{
			m_mapFlags |= GL_MEMORY_MAP_READ_BIT | GL_MEMORY_MAP_WRITE_BIT | GL_MEMORY_MAP_FLUSH_EXPLICIT_BIT;
		}
	}

	//************************************************************************************************

	DeviceMemory::DeviceMemory( Device const & device
		, renderer::MemoryRequirements const & requirements
		, renderer::MemoryPropertyFlags flags )
		: renderer::DeviceMemory{ device, flags }
		, m_device{ device }
		, m_requirements{ requirements }
	{
	}

	DeviceMemory::~DeviceMemory()
	{
	}

	void DeviceMemory::bindToBuffer( GLuint resource, GLenum target )
	{
		assert( !m_impl && "Memory object was already bound to a resource object" );
		m_impl = std::make_unique< BufferMemory >( m_device, m_requirements, m_flags, resource, target );
	}

	void DeviceMemory::bindToImage( Texture const & texture
		, GLenum target
		, renderer::ImageCreateInfo const & createInfo )
	{
		assert( !m_impl && "Memory object was already bound to a resource object" );
		m_impl = std::make_unique< ImageMemory >( m_device, m_requirements, m_flags, texture, target, createInfo );
	}

	uint8_t * DeviceMemory::lock( uint32_t offset
		, uint32_t size
		, renderer::MemoryMapFlags flags )const
	{
		assert( m_impl && "Memory object was not bound to a resource object" );
		return m_impl->lock( offset, size, flags );
	}

	void DeviceMemory::flush( uint32_t offset
		, uint32_t size )const
	{
		assert( m_impl && "Memory object was not bound to a resource object" );
		m_impl->flush( offset, size );
	}

	void DeviceMemory::invalidate( uint32_t offset
		, uint32_t size )const
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
