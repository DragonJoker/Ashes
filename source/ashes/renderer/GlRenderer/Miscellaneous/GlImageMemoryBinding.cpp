/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Miscellaneous/GlImageMemoryBinding.hpp"

#include "Image/GlImage.hpp"
#include "Miscellaneous/GlCallLogger.hpp"

#include "ashesgl_api.hpp"

#pragma warning( disable: 4127 )

namespace ashes::gl
{
	ImageMemoryBinding::ImageMemoryBinding( VkDeviceMemory parent
		, VkDevice device
		, VkImage image
		, VkDeviceSize memoryOffset )
		: DeviceMemoryBinding{ parent
			, device
			, get( image )->getTarget()
			, memoryOffset
			, get( image )->getMemoryRequirements()
			, image
			, get( image )->getInternal() }
		, m_texture{ get( image ) }
	{
		m_texture->setMemoryBinding( this );
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glBindTexture
			, m_texture->getTarget()
			, m_boundName );

		switch ( m_texture->getTarget() )
		{
		case GL_TEXTURE_1D:
			setImage1D( context );
			break;
		case GL_TEXTURE_2D:
			setImage2D( context, m_texture->getDimensions().height );
			break;
		case GL_TEXTURE_3D:
			setImage3D( context, m_texture->getDimensions().depth );
			break;
		case GL_TEXTURE_1D_ARRAY:
			setImage2D( context, m_texture->getArrayLayers() );
			break;
		case GL_TEXTURE_2D_ARRAY:
			setImage3D( context, m_texture->getArrayLayers() );
			break;
		case GL_TEXTURE_CUBE:
			if ( hasTextureStorage( m_device ) )
			{
				setImage2D( context, m_texture->getDimensions().height );
			}
			else
			{
				setImageCubeFace( context, 0 );
				setImageCubeFace( context, 1 );
				setImageCubeFace( context, 2 );
				setImageCubeFace( context, 3 );
				setImageCubeFace( context, 4 );
				setImageCubeFace( context, 5 );
			}
			break;
		case GL_TEXTURE_CUBE_ARRAY:
			setImage3D( context, m_texture->getArrayLayers() );
			break;
		case GL_TEXTURE_2D_MULTISAMPLE:
			setImage2DMS( context );
			break;
		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
			setImage3DMS( context, m_texture->getArrayLayers() );
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
			if ( levels != int( m_texture->getMipLevels() ) )
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
		bufferCopyRegion.imageSubresource.aspectMask = getAspectMask( m_texture->getFormatVk() );
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

	ImageMemoryBinding::~ImageMemoryBinding()
	{
		m_texture->setMemoryBinding( nullptr );
	}

	VkDeviceSize ImageMemoryBinding::getMipLevelOffset( uint32_t mipLevel )const
	{
		return getOffset()
			+ getTotalSize( m_texture->getDimensions()
				, m_texture->getFormatVk()
				, m_texture->getArrayLayers()
				, mipLevel
				, uint32_t( getMinimalSize( m_texture->getFormatVk() ) ) );
	}

	VkDeviceSize ImageMemoryBinding::getMipLevelLayerSize( uint32_t mipLevel )const
	{
		return getLevelsSize( VkExtent2D{ m_texture->getDimensions().width, m_texture->getDimensions().height }
			, m_texture->getFormatVk()
			, mipLevel
			, 1u
			, uint32_t( getMinimalSize( m_texture->getFormatVk() ) ) );
	}

	VkDeviceSize ImageMemoryBinding::getArrayLayerSize()const
	{
		return getLevelsSize( VkExtent2D{ m_texture->getDimensions().width, m_texture->getDimensions().height }
			, m_texture->getFormatVk()
			, 0u
			, m_texture->getMipLevels()
			, uint32_t( getMinimalSize( m_texture->getFormatVk() ) ) );
	}

	void ImageMemoryBinding::upload( ContextLock const & context
		, ByteArray const & data
		, BindingRange const & range )const
	{
		if ( !range )
		{
			return;
		}

		auto inter = m_range.intersect( range );

		if ( !inter )
		{
			return;
		}

		setupUpdateRegions( inter );
		glLogCall( context
			, glPixelStorei
			, GL_UNPACK_ALIGNMENT
			, 1 );
		glLogCall( context
			, glBindTexture
			, m_texture->getTarget()
			, m_boundName );

		for ( size_t i = m_beginRegion; i < m_endRegion; ++i )
		{
			updateRegion( context, m_updateRegions[i] );
		}

		if ( m_texture->getMipLevels() > 1
			&& !isCompressedFormat( m_texture->getFormatVk() ) )
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

	void ImageMemoryBinding::setImage1D( ContextLock const & context )
	{
		if ( false && isCompressedFormat( m_texture->getFormatVk() ) )
		{
			intptr_t offset = 0;
			auto width = m_texture->getDimensions().width;

			for ( uint32_t level = 0u; level < m_texture->getMipLevels(); ++level )
			{
				auto bufSize = ashes::getSize( VkExtent3D{ width, 1u, 1u }, m_texture->getFormatVk(), level );
				glLogCall( context
					, glCompressedTexImage1D
					, m_texture->getTarget()
					, GLint( level )
					, m_texture->getInternalFormat()
					, GLsizei( width )
					, 0
					, GLsizei( bufSize )
					, getBufferOffset( offset ) );
				width >>= 1u;
				offset += bufSize;
			}
		}
		else if ( hasTextureStorage( m_device ) )
		{
			glLogCall( context
				, glTexStorage1D
				, m_texture->getTarget()
				, GLsizei( m_texture->getMipLevels() )
				, m_texture->getInternalFormat()
				, GLsizei( m_texture->getDimensions().width ) );
		}
		else
		{
			intptr_t offset = 0;
			auto width = m_texture->getDimensions().width;

			for ( uint32_t level = 0u; level < m_texture->getMipLevels(); ++level )
			{
				auto bufSize = ashes::getSize( VkExtent3D{ width, 1u, 1u }, m_texture->getFormatVk(), level );
				glLogCall( context
					, glTexImage1D
					, m_texture->getTarget()
					, GLint( level )
					, m_texture->getInternalFormat()
					, GLsizei( width )
					, 0
					, m_texture->getUnpackFormat()
					, m_texture->getUnpackType()
					, getBufferOffset( offset ) );
				width >>= 1u;
				offset += bufSize;
			}
		}
	}

	void ImageMemoryBinding::setImage2D( ContextLock const & context
		, uint32_t height )
	{
		if ( false && isCompressedFormat( m_texture->getFormatVk() ) )
		{
			intptr_t offset = 0;
			auto width = m_texture->getDimensions().width;

			for ( uint32_t level = 0u; level < m_texture->getMipLevels(); ++level )
			{
				auto bufSize = ashes::getSize( VkExtent3D{ width, height, 1u }, m_texture->getFormatVk(), level );
				glLogCall( context
					, glCompressedTexImage2D
					, m_texture->getTarget()
					, GLint( level )
					, m_texture->getInternalFormat()
					, GLsizei( width )
					, GLsizei( height )
					, 0
					, GLsizei( bufSize )
					, getBufferOffset( offset ) );
				width >>= 1u;
				height >>= 1u;
				offset += bufSize;
			}
		}
		else if ( hasTextureStorage( m_device ) )
		{
			glLogCall( context
				, glTexStorage2D
				, m_texture->getTarget()
				, GLsizei( m_texture->getMipLevels() )
				, m_texture->getInternalFormat()
				, GLsizei( m_texture->getDimensions().width )
				, GLsizei( height ) );
		}
		else
		{
			intptr_t offset = 0;
			auto width = m_texture->getDimensions().width;

			for ( uint32_t level = 0u; level < m_texture->getMipLevels(); ++level )
			{
				auto bufSize = ashes::getSize( VkExtent3D{ width, height, 1u }, m_texture->getFormatVk(), level );
				glLogCall( context
					, glTexImage2D
					, m_texture->getTarget()
					, GLint( level )
					, m_texture->getInternalFormat()
					, GLsizei( m_texture->getDimensions().width )
					, GLsizei( height )
					, 0
					, m_texture->getUnpackFormat()
					, m_texture->getUnpackType()
					, getBufferOffset( offset ) );
				width >>= 1u;
				height >>= 1u;
				offset += bufSize;
			}
		}
	}

	void ImageMemoryBinding::setImageCubeFace( ContextLock const & context
		, int face )
	{
		assert( !hasTextureStorage( m_device ) );
		auto width = m_texture->getDimensions().width;
		auto height = m_texture->getDimensions().height;
		intptr_t offset = 0;

		for ( uint32_t level = 0u; level < m_texture->getMipLevels(); ++level )
		{
			auto bufSize = ashes::getSize( VkExtent3D{ width, height, 1u }, m_texture->getFormatVk(), level );

			if ( false && isCompressedFormat( m_texture->getFormatVk() ) )
			{
				glLogCall( context
					, glCompressedTexImage2D
					, m_texture->getTarget()
					, GLint( level )
					, m_texture->getInternalFormat()
					, GLsizei( width )
					, GLsizei( height )
					, 0
					, GLsizei( bufSize )
					, getBufferOffset( offset ) );
				offset += bufSize;
			}
			else
			{
				glLogCall( context
					, glTexImage2D
					, GlTextureType( GL_TEXTURE_CUBE_POSITIVE_X + face )
					, GLint( level )
					, m_texture->getInternalFormat()
					, GLsizei( m_texture->getDimensions().width )
					, GLsizei( m_texture->getDimensions().height )
					, 0
					, m_texture->getUnpackFormat()
					, m_texture->getUnpackType()
					, getBufferOffset( offset ) );
				offset += bufSize;
			}

			width >>= 1u;
			height >>= 1u;
		}
	}

	void ImageMemoryBinding::setImage3D( ContextLock const & context
		, uint32_t depth )
	{
		if ( false && isCompressedFormat( m_texture->getFormatVk() ) )
		{
			intptr_t offset = 0;
			auto width = m_texture->getDimensions().width;
			auto height = m_texture->getDimensions().height;

			for ( uint32_t level = 0u; level < m_texture->getMipLevels(); ++level )
			{
				auto bufSize = ashes::getSize( VkExtent3D{ width, height, depth }, m_texture->getFormatVk(), level );
				glLogCall( context
					, glCompressedTexImage3D
					, m_texture->getTarget()
					, GLint( level )
					, m_texture->getInternalFormat()
					, GLsizei( width )
					, GLsizei( height )
					, GLsizei( depth )
					, 0
					, GLsizei( bufSize )
					, getBufferOffset( offset ) );
				width >>= 1u;
				height >>= 1u;
				depth >>= 1u;
				offset += bufSize;
			}
		}
		else if ( hasTextureStorage( m_device ) )
		{
			glLogCall( context
				, glTexStorage3D
				, m_texture->getTarget()
				, GLsizei( m_texture->getMipLevels() )
				, m_texture->getInternalFormat()
				, GLsizei( m_texture->getDimensions().width )
				, GLsizei( m_texture->getDimensions().height )
				, GLsizei( depth ) );
		}
		else
		{
			intptr_t offset = 0;
			auto width = m_texture->getDimensions().width;
			auto height = m_texture->getDimensions().height;

			for ( uint32_t level = 0u; level < m_texture->getMipLevels(); ++level )
			{
				auto bufSize = ashes::getSize( VkExtent3D{ width, height, depth }, m_texture->getFormatVk(), level );
				glLogCall( context
					, glTexImage3D
					, m_texture->getTarget()
					, GLint( level )
					, m_texture->getInternalFormat()
					, GLsizei( m_texture->getDimensions().width )
					, GLsizei( m_texture->getDimensions().height )
					, GLsizei( depth )
					, 0
					, m_texture->getUnpackFormat()
					, m_texture->getUnpackType()
					, getBufferOffset( offset ) );
				width >>= 1u;
				height >>= 1u;
				depth >>= 1u;
				offset += bufSize;
			}
		}
	}

	void ImageMemoryBinding::setImage2DMS( ContextLock const & context )
	{
		if ( hasTextureStorage( m_device ) )
		{
			glLogCall( context
				, glTexStorage2DMultisample
				, m_texture->getTarget()
				, GLsizei( m_texture->getSamples() )
				, m_texture->getInternalFormat()
				, GLsizei( m_texture->getDimensions().width )
				, GLsizei( m_texture->getDimensions().height )
				, GL_TRUE );
		}
		else
		{
			glLogCall( context
				, glTexImage2DMultisample
				, m_texture->getTarget()
				, GLsizei( m_texture->getSamples() )
				, m_texture->getInternalFormat()
				, GLsizei( m_texture->getDimensions().width )
				, GLsizei( m_texture->getDimensions().height )
				, GL_TRUE );
		}
	}

	void ImageMemoryBinding::setImage3DMS( ContextLock const & context
		, uint32_t depth )
	{
		if ( hasTextureStorage( m_device ) )
		{
			glLogCall( context
				, glTexStorage3DMultisample
				, m_texture->getTarget()
				, GLsizei( m_texture->getSamples() )
				, m_texture->getInternalFormat()
				, GLsizei( m_texture->getDimensions().width )
				, GLsizei( m_texture->getDimensions().height )
				, GLsizei( depth )
				, GL_TRUE );
		}
		else
		{
			glLogCall( context
				, glTexImage3DMultisample
				, m_texture->getTarget()
				, GLsizei( m_texture->getSamples() )
				, m_texture->getInternalFormat()
				, GLsizei( m_texture->getDimensions().width )
				, GLsizei( m_texture->getDimensions().height )
				, GLsizei( depth )
				, GL_TRUE );
		}
	}

	void ImageMemoryBinding::setupUpdateRegions( BindingRange const & range )const
	{
		assert( !m_updateRegions.empty() && "Can't update this texture." );
		auto layerSize = ashes::getSize( m_updateRegions[0].imageExtent
			, m_texture->getFormatVk() );
		m_beginRegion = 0u;
		auto offset = range.getOffset();
		auto size = range.getSize();

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
			reportError( m_parent
				, VK_ERROR_VALIDATION_FAILED_EXT
				, "OpenGL"
				, "Invalid offset and/or size." );
			m_beginRegion = std::min( m_beginRegion, m_updateRegions.size() );
			m_endRegion = std::min( m_endRegion, m_updateRegions.size() - 1u );
		}
	}

	void ImageMemoryBinding::updateRegion( ContextLock const & context
		, VkBufferImageCopy const & copyInfo )const
	{
		if ( isCompressedFormat( m_texture->getFormatVk() ) )
		{
			auto layerSize = ashes::getSize( copyInfo.imageExtent
				, m_texture->getFormatVk()
				, copyInfo.imageSubresource.mipLevel );

			switch ( m_texture->getTarget() )
			{
			case GL_TEXTURE_1D:
				glLogCall( context
					, glCompressedTexSubImage1D
					, m_texture->getTarget()
					, GLint( copyInfo.imageSubresource.mipLevel )
					, copyInfo.imageOffset.x
					, GLsizei( copyInfo.imageExtent.width )
					, m_texture->getInternalFormat()
					, GLsizei( layerSize )
					, getBufferOffset( intptr_t( getOffset() + copyInfo.bufferOffset ) ) );
				break;

			case GL_TEXTURE_2D:
				glLogCall( context
					, glCompressedTexSubImage2D
					, m_texture->getTarget()
					, GLint( copyInfo.imageSubresource.mipLevel )
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, GLsizei( copyInfo.imageExtent.width )
					, GLsizei( copyInfo.imageExtent.height )
					, m_texture->getInternalFormat()
					, GLsizei( layerSize )
					, getBufferOffset( intptr_t( getOffset() + copyInfo.bufferOffset ) ) );
				break;

			case GL_TEXTURE_3D:
				glLogCall( context
					, glCompressedTexSubImage3D
					, m_texture->getTarget()
					, GLint( copyInfo.imageSubresource.mipLevel )
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, copyInfo.imageOffset.z
					, GLsizei( copyInfo.imageExtent.width )
					, GLsizei( copyInfo.imageExtent.height )
					, GLsizei( copyInfo.imageExtent.depth )
					, m_texture->getInternalFormat()
					, GLsizei( layerSize )
					, getBufferOffset( intptr_t( getOffset() + copyInfo.bufferOffset ) ) );
				break;

			case GL_TEXTURE_1D_ARRAY:
				glLogCall( context
					, glCompressedTexSubImage2D
					, m_texture->getTarget()
					, GLint( copyInfo.imageSubresource.mipLevel )
					, copyInfo.imageOffset.x
					, GLint( copyInfo.imageSubresource.baseArrayLayer )
					, GLsizei( copyInfo.imageExtent.width )
					, GLsizei( copyInfo.imageSubresource.layerCount )
					, m_texture->getInternalFormat()
					, GLsizei( layerSize )
					, getBufferOffset( intptr_t( getOffset() + copyInfo.bufferOffset ) ) );
				break;

			case GL_TEXTURE_2D_ARRAY:
				glLogCall( context
					, glCompressedTexSubImage3D
					, m_texture->getTarget()
					, GLint( copyInfo.imageSubresource.mipLevel )
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, GLint( copyInfo.imageSubresource.baseArrayLayer )
					, GLsizei( copyInfo.imageExtent.width )
					, GLsizei( copyInfo.imageExtent.height )
					, GLsizei( copyInfo.imageSubresource.layerCount )
					, m_texture->getInternalFormat()
					, GLsizei( layerSize )
					, getBufferOffset( intptr_t( getOffset() + copyInfo.bufferOffset ) ) );
				break;

			default:
				assert( false && "Unexpected GlTextureType" );
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
					, GLint( copyInfo.imageSubresource.mipLevel )
					, copyInfo.imageOffset.x
					, GLsizei( copyInfo.imageExtent.width )
					, m_texture->getUnpackFormat()
					, m_texture->getUnpackType()
					, getBufferOffset( intptr_t( getOffset() + copyInfo.bufferOffset ) ) );
				break;

			case GL_TEXTURE_2D:
				glLogCall( context
					, glTexSubImage2D
					, m_texture->getTarget()
					, GLint( copyInfo.imageSubresource.mipLevel )
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, GLsizei( copyInfo.imageExtent.width )
					, GLsizei( copyInfo.imageExtent.height )
					, m_texture->getUnpackFormat()
					, m_texture->getUnpackType()
					, getBufferOffset( intptr_t( getOffset() + copyInfo.bufferOffset ) ) );
				break;

			case GL_TEXTURE_3D:
				glLogCall( context
					, glTexSubImage3D
					, m_texture->getTarget()
					, GLint( copyInfo.imageSubresource.mipLevel )
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, copyInfo.imageOffset.z
					, GLsizei( copyInfo.imageExtent.width )
					, GLsizei( copyInfo.imageExtent.height )
					, GLsizei( copyInfo.imageExtent.depth )
					, m_texture->getUnpackFormat()
					, m_texture->getUnpackType()
					, getBufferOffset( intptr_t( getOffset() + copyInfo.bufferOffset ) ) );
				break;

			case GL_TEXTURE_1D_ARRAY:
				glLogCall( context
					, glTexSubImage2D
					, m_texture->getTarget()
					, GLint( copyInfo.imageSubresource.mipLevel )
					, copyInfo.imageOffset.x
					, GLint( copyInfo.imageSubresource.baseArrayLayer )
					, GLsizei( copyInfo.imageExtent.width )
					, GLsizei( copyInfo.imageSubresource.layerCount )
					, m_texture->getUnpackFormat()
					, m_texture->getUnpackType()
					, getBufferOffset( intptr_t( getOffset() + copyInfo.bufferOffset ) ) );
				break;

			case GL_TEXTURE_2D_ARRAY:
				glLogCall( context
					, glTexSubImage3D
					, m_texture->getTarget()
					, GLint( copyInfo.imageSubresource.mipLevel )
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, GLint( copyInfo.imageSubresource.baseArrayLayer )
					, GLsizei( copyInfo.imageExtent.width )
					, GLsizei( copyInfo.imageExtent.height )
					, GLsizei( copyInfo.imageSubresource.layerCount )
					, m_texture->getUnpackFormat()
					, m_texture->getUnpackType()
					, getBufferOffset( intptr_t( getOffset() + copyInfo.bufferOffset ) ) );
				break;

			default:
				assert( false && "Unexpected GlTextureType" );
				break;
			}
		}
	}
}
