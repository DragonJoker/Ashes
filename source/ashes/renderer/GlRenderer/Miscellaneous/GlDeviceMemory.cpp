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

	VkMemoryPropertyFlags getFlags( uint32_t memoryTypeIndex )
	{
		assert( memoryTypeIndex < Instance::getMemoryProperties().memoryTypeCount
			&& "Wrong deduced memory type" );
		return Instance::getMemoryProperties().memoryTypes[memoryTypeIndex].propertyFlags;
	}

	//************************************************************************************************

	namespace
	{
		struct BufferAlloc
		{
			GLuint name;
			GlBufferTarget target;
			GLsizeiptr size;
			GlBufferDataUsageFlags flags;
		};
		using BufferAllocCont = std::vector< BufferAlloc >;

		BufferAllocCont & getAllocatedBuffers()
		{
			static BufferAllocCont result;
			return result;
		}

		BufferAllocCont::iterator findBuffer( GLuint buffer )
		{
			return std::find_if( getAllocatedBuffers().begin()
				, getAllocatedBuffers().end()
				, [buffer]( BufferAlloc const & lookup )
				{
					return lookup.name == buffer;
				} );
		}

		GLint getBufferSize( ContextLock const & context
			, GLuint buffer )
		{
			GLint result = 0;
			GlBufferTarget target = GL_BUFFER_TARGET_COPY_WRITE;
			auto it = findBuffer( buffer );

			if ( it != getAllocatedBuffers().end() )
			{
				target = it->target;
			}

			context->glBindBuffer( target
				, buffer );
			context->glGetBufferParameteriv( target
				, 34660
				, &result );
			context->glBindBuffer( target
				, 0u );
			return result;
		}

		BufferAllocCont::iterator findBuffer( GLuint buffer
			, GLsizeiptr size )
		{
			return std::find_if( getAllocatedBuffers().begin()
				, getAllocatedBuffers().end()
				, [buffer, size]( BufferAlloc const & lookup )
				{
					return lookup.name == buffer
						&& lookup.size == size;
				} );
		}

		GLuint createBuffer( ContextLock const & context
			, GlBufferTarget target
			, GLsizeiptr size
			, GlBufferDataUsageFlags flags )
		{
			auto allocateBuffer = [&context]( GLuint result
				, GlBufferTarget target
				, GLsizeiptr size
				, GlBufferDataUsageFlags flags )
			{
				glLogCall( context
					, glBindBuffer
					, target
					, result );
				glLogCall( context
					, glBufferData
					, target
					, size
					, nullptr
					, flags );
				glLogCall( context
					, glBindBuffer
					, target
					, 0u );
				return result;
			};

			auto & cache = getAllocatedBuffers();
			GLuint result;
			glLogCall( context
				, glGenBuffers
				, 1u
				, &result );
			auto it = findBuffer( result );

			while ( it != cache.end() )
			{
				std::cerr << "Buffer " << result << " is being reused" << std::endl;
				allocateBuffer( it->name, it->target, it->size, it->flags );
				glLogCall( context
					, glGenBuffers
					, 1u
					, &result );
				it = findBuffer( result );
			}

			allocateBuffer( result, target, size, flags );
			GLint realSize = getBufferSize( context, result );
			assert( realSize >= size );
			getAllocatedBuffers().push_back( { result, target, GLsizeiptr( realSize ), flags } );
			return result;
		}

		void * mapBuffer( ContextLock const & context
			, GlBufferTarget target
			, VkDeviceSize offset
			, VkDeviceSize size
			, GlMemoryMapFlags mapFlags )
		{
			return glLogNonVoidCall( context
				, glMapBufferRange
				, target
				, GLintptr( offset )
				, GLsizei( size )
				, mapFlags );
		}

		void deleteBuffer( ContextLock const & context
			, GLuint buffer )
		{
			if ( buffer != GL_INVALID_INDEX )
			{
				GLint size = getBufferSize( context, buffer );

				if ( size != 0 )
				{
					auto it = findBuffer( buffer, size );

					if ( it != getAllocatedBuffers().end() )
					{
						getAllocatedBuffers().erase( it );

						glLogCall( context
							, glDeleteBuffers
							, 1u
							, &buffer );
					}
					else
					{
						std::cerr << "Couldn't find buffer " << buffer << " it has probably been reused" << std::endl;
					}
				}
				else
				{
					std::cerr << "Couldn't find buffer " << buffer << " it has probably been reused" << std::endl;
				}
			}
		}

		class ImageMemory
			: public DeviceMemory::DeviceMemoryImpl
		{
		public:
			ImageMemory( VkDeviceMemory parent
				, VkDevice device
				, VkMemoryAllocateInfo allocateInfo
				, VkImage texture
				, VkDeviceSize memoryOffset )
				: DeviceMemory::DeviceMemoryImpl
				{
					parent,
					device,
					std::move( allocateInfo ),
					get( texture )->getTarget(),
					memoryOffset,
					get( device )->getLimits().nonCoherentAtomSize,
				}
				, m_texture{ get( texture ) }
				, m_internal{ getInternalFormat( m_texture->getFormat() ) }
				, m_format{ getFormat( m_internal ) }
				, m_type{ getType( m_internal ) }
			{
				m_boundResource = get( texture )->getInternal();
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
				case GL_TEXTURE_CUBE:
					doSetImageCubeFace( context, 0 );
					doSetImageCubeFace( context, 1 );
					doSetImageCubeFace( context, 2 );
					doSetImageCubeFace( context, 3 );
					doSetImageCubeFace( context, 4 );
					doSetImageCubeFace( context, 5 );
					break;
				case GL_TEXTURE_CUBE_ARRAY:
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

				if ( hasTextureStorage( device ) )
				{
					int levels = 0;
					glLogCall( context
						, glGetTexParameteriv
						, m_boundTarget
						, GL_TEX_PARAMETER_IMMUTABLE_LEVELS
						, &levels );
					assert( levels == m_texture->getMipLevels() );
					int format = 0;
					glLogCall( context
						, glGetTexParameteriv
						, m_boundTarget
						, GL_TEX_PARAMETER_IMMUTABLE_FORMAT
						, &format );
					assert( format != 0 );
					glLogCall( context
						, glBindTexture
						, m_boundTarget
						, 0 );
				}
				else
				{
					glLogCall( context
						, glTexParameteri
						, m_boundTarget
						, GL_TEX_PARAMETER_MAX_LEVEL
						, std::max( 0, GLint( m_texture->getMipLevels() ) - 1 ) );
					glLogCall( context
						, glGenerateMipmap
						, m_boundTarget );
					glLogCall( context
						, glBindTexture
						, m_boundTarget
						, 0 );
				}

				if ( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
				{
					// Create the PBO
					m_buffer = createBuffer( context
						, GL_BUFFER_TARGET_COPY_WRITE
						, GLsizeiptr( m_allocateInfo.allocationSize )
						, getBufferDataUsageFlags( m_flags ) );

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
				auto result = mapBuffer( context
					, GL_BUFFER_TARGET_PIXEL_UNPACK
					, offset
					, copySize
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
					if ( context->hasMemoryBarrier() )
					{
						glLogCall( context
							, glMemoryBarrier
							, GL_MEMORY_BARRIER_TEXTURE_UPDATE );
					}

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
				if ( hasTextureStorage( m_device ) )
				{
					glLogCall( context
						, glTexStorage1D
						, m_boundTarget
						, GLsizei( m_texture->getMipLevels() )
						, m_internal
						, m_texture->getDimensions().width );
				}
				else
				{
					glLogCall( context
						, glTexImage1D
						, m_boundTarget
						, 0u
						, m_internal
						, m_texture->getDimensions().width
						, 0
						, m_format
						, m_type
						, nullptr );
				}
			}

			void doSetImage2D( ContextLock const & context )
			{
				if ( hasTextureStorage( m_device ) )
				{
					glLogCall( context
						, glTexStorage2D
						, m_boundTarget
						, GLsizei( m_texture->getMipLevels() )
						, m_internal
						, m_texture->getDimensions().width
						, m_texture->getDimensions().height );
				}
				else
				{
					glLogCall( context
						, glTexImage2D
						, m_boundTarget
						, 0u
						, m_internal
						, m_texture->getDimensions().width
						, m_texture->getDimensions().height
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
					, GL_TEXTURE_CUBE_POSITIVE_X + face
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
						, m_boundTarget
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
						, m_boundTarget
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
						, m_boundTarget
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
						, m_boundTarget
						, GLsizei( m_texture->getSamples() )
						, m_internal
						, m_texture->getDimensions().width
						, m_texture->getDimensions().height
						, GL_TRUE );
				}
			}

			void doSetImage3DMS( ContextLock const & context )
			{
				if ( hasTextureStorage( m_device ) )
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
				else
				{
					glLogCall( context
						, glTexImage3DMultisample
						, m_boundTarget
						, GLsizei( m_texture->getSamples() )
						, m_internal
						, m_texture->getDimensions().width
						, m_texture->getDimensions().height
						, m_texture->getDimensions().depth
						, GL_TRUE );
				}
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
				, VkDeviceSize memoryOffset )
				: DeviceMemory::DeviceMemoryImpl
				{
					parent,
					device,
					std::move( allocateInfo ),
					target,
					memoryOffset,
					get( device )->getLimits().nonCoherentAtomSize,
				}
			{
				auto context = get( m_device )->getContext();
				m_buffer = createBuffer( context
					, GlBufferTarget( m_boundTarget )
					, m_allocateInfo.allocationSize
					, getBufferDataUsageFlags( m_flags ) );
				m_boundResource = m_buffer;
			}

			~BufferMemory()
			{
			}

			VkResult lock( ContextLock const & context
				, VkDeviceSize offset
				, VkDeviceSize size
				, void ** data )const override
			{
				glLogCall( context
					, glBindBuffer
					, GlBufferTarget( m_boundTarget )
					, m_buffer );
				GLint bufferSize = 0;
				glLogCall( context
					, glGetBufferParameteriv
					, GlBufferTarget( m_boundTarget )
					, 34660
					, &bufferSize );

				if ( size + offset <= bufferSize )
				{
					auto result = mapBuffer( context
						, GlBufferTarget( m_boundTarget )
						, offset
						, size == WholeSize ? m_allocateInfo.allocationSize : size
						, m_mapFlags );
					*data = result;
					return result
						? VK_SUCCESS
						: VK_ERROR_MEMORY_MAP_FAILED;
				}

				return VK_ERROR_MEMORY_MAP_FAILED;
			}

			void unlock( ContextLock const & context )const override
			{
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
		, GLuint boundTarget
		, VkDeviceSize memoryOffset
		, VkDeviceSize align )
		: m_parent{ parent }
		, m_device{ device }
		, m_allocateInfo{ allocateInfo }
		, m_flags{ getFlags( m_allocateInfo.memoryTypeIndex ) }
		, m_mapFlags{ convertMemoryMapFlags( m_flags ) }
		, m_buffer{ GL_INVALID_INDEX }
		, m_boundTarget{ boundTarget }
		, m_memoryOffset{ memoryOffset }
		, m_align{ align }
	{
	}

	DeviceMemory::DeviceMemoryImpl::~DeviceMemoryImpl()
	{
		if ( m_buffer != GL_INVALID_INDEX )
		{
			auto context = get( m_device )->getContext();
			deleteBuffer( context, m_buffer );
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
		else
		{
			size = ashes::getAlignedSize( size, m_align );
			offset = ashes::getAlignedSize( offset, m_align );
			size = offset + size < m_allocateInfo.allocationSize
				? size
				: size = m_allocateInfo.allocationSize - offset;
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
		, m_mapFlags{ convertMemoryMapFlags( m_flags ) }
	{
		if ( ashes::checkFlag( m_flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
		{
			m_data.resize( allocateInfo.allocationSize );
		}
	}

	DeviceMemory::~DeviceMemory()
	{
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
				, memoryOffset );
			get( buffer )->setInternal( m_impl->getInternal(), memoryOffset );
			get( buffer )->setMemory( get( this ) );

			if ( !m_data.empty() )
			{
				auto context = get( m_device )->getContext();
				m_impl->upload( context
					, m_data
					, 0u
					, m_data.size() );
			}

			m_internal = m_impl->getBuffer();
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
				, memoryOffset );
			get( image )->setMemory( get( this ) );
			m_internal = m_impl->getBuffer();
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
		assert( m_impl && "VkDeviceMemory should be bound to a buffer or an image, at this point" );
		m_impl->upload( context, m_data, offset, size );
	}

	void DeviceMemory::download( ContextLock const & context
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		assert( m_impl && "VkDeviceMemory should be bound to a buffer or an image, at this point" );
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

		if ( m_impl )
		{
			upload( context, offset, size );
		}

		return VK_SUCCESS;
	}

	VkResult DeviceMemory::invalidate( ContextLock const & context
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
		assert( m_mapped && "VkDeviceMemory should be mapped" );
		m_dirty = true;

		if ( m_impl )
		{
			download( context, m_mappedOffset, m_mappedSize );
		}

		return VK_SUCCESS;
	}

	void DeviceMemory::unlock( ContextLock const & context )const
	{
		assert( m_mapped && "VkDeviceMemory should be mapped" );
		m_mapped = false;

		if ( m_impl )
		{
			upload( context, m_mappedOffset, m_mappedSize );
		}
	}

	//************************************************************************************************
}
