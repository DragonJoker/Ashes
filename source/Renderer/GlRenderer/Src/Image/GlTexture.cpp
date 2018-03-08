#include "Image/GlTexture.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Sync/GlImageMemoryBarrier.hpp"
#include "Core/GlRenderingResources.hpp"
#include "Image/GlTextureView.hpp"

#ifdef max
#	undef max
#	undef min
#endif

namespace gl_renderer
{
	namespace
	{
		GlTextureType convert( renderer::TextureType type
			, uint32_t layerCount
			, renderer::SampleCountFlag samples )
		{
			GlTextureType result;

			switch ( type )
			{
			case renderer::TextureType::e1D:
				if ( layerCount > 1 )
				{
					result = GL_TEXTURE_1D_ARRAY;
				}
				else
				{
					result = GL_TEXTURE_1D;
				}
				break;
			case renderer::TextureType::e2D:
				if ( layerCount > 1 )
				{
					if ( samples > renderer::SampleCountFlag::e1 )
					{
						result = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
					}
					else
					{
						result = GL_TEXTURE_2D_ARRAY;
					}
				}
				else
				{
					if ( samples != renderer::SampleCountFlag::e1 )
					{
						result = GL_TEXTURE_2D_MULTISAMPLE;
					}
					else
					{
						result = GL_TEXTURE_2D;
					}
				}
				break;
			case renderer::TextureType::e3D:
				result = GL_TEXTURE_3D;
				break;
			default:
				assert( false );
				result = GL_TEXTURE_2D;
				break;
			}

			return result;
		}
	}

	Texture::Texture( Device const & device
		, renderer::Format format
		, renderer::Extent2D const & dimensions )
		: renderer::Texture{ device
			, renderer::TextureType::e2D
			, format
			, { dimensions.width, dimensions.height, 1u }
			, 1u
			, 1u }
		, m_device{ device }
		, m_samples{ renderer::SampleCountFlag::e1 }
	{
	}

	Texture::Texture( Device const & device
		, renderer::ImageCreateInfo const & createInfo
		, renderer::MemoryPropertyFlags memoryFlags )
		: renderer::Texture{ device
			, createInfo.imageType
			, createInfo.format
			, createInfo.extent
			, createInfo.mipLevels
			, createInfo.arrayLayers }
		, m_device{ device }
		, m_target{ convert( createInfo.imageType, createInfo.arrayLayers, createInfo.samples ) }
		, m_samples{ createInfo.samples }
	{
		glLogCall( gl::GenTextures, 1, &m_texture );
		glLogCall( gl::BindTexture, m_target, m_texture );

		switch ( m_target )
		{
		case gl_renderer::GL_TEXTURE_1D:
			doSetImage1D( createInfo.extent.width );
			break;
		case gl_renderer::GL_TEXTURE_2D:
			doSetImage2D( createInfo.extent.width
				, createInfo.extent.height );
			break;
		case gl_renderer::GL_TEXTURE_3D:
			doSetImage3D( createInfo.extent.width
				, createInfo.extent.height
				, createInfo.extent.depth );
			break;
		case gl_renderer::GL_TEXTURE_1D_ARRAY:
			doSetImage2D( createInfo.extent.width
				, createInfo.arrayLayers );
			break;
		case gl_renderer::GL_TEXTURE_2D_ARRAY:
			doSetImage3D( createInfo.extent.width
				, createInfo.extent.height
				, createInfo.arrayLayers );
			break;
		case gl_renderer::GL_TEXTURE_2D_MULTISAMPLE:
			doSetImage2DMS( createInfo.extent.width
				, createInfo.extent.height
				, createInfo.samples );
			break;
		case gl_renderer::GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
			doSetImage3DMS( createInfo.extent.width
				, createInfo.extent.height
				, createInfo.arrayLayers
				, createInfo.samples );
			break;
		default:
			break;
		}

		int levels = 0;
		gl::GetTexParameteriv( m_target, GL_TEXTURE_IMMUTABLE_LEVELS, &levels );
		assert( levels == createInfo.mipLevels );
		int format = 0;
		gl::GetTexParameteriv( m_target, GL_TEXTURE_IMMUTABLE_FORMAT, &format );
		assert( format != 0 );
		glLogCall( gl::BindTexture, m_target, 0 );
	}

	Texture::~Texture()
	{
		glLogCall( gl::DeleteTextures, 1, &m_texture );
	}

	renderer::TextureViewPtr Texture::createView( renderer::ImageViewCreateInfo const & createInfo )const
	{
		return std::make_unique< TextureView >( m_device
			, *this
			, createInfo );
	}

	void Texture::generateMipmaps()const
	{
		glLogCall( gl::BindTexture, m_target, m_texture );
		glLogCall( gl::GenerateMipmap, m_target );
		glLogCall( gl::BindTexture, m_target, 0 );
	}

	void Texture::doSetImage1D( uint32_t width )
	{
		glLogCall( gl::TexStorage1D
			, m_target
			, GLsizei( getMipmapLevels() )
			, gl_renderer::getInternal( getFormat() )
			, width );
	}

	void Texture::doSetImage2D( uint32_t width
		, uint32_t height )
	{
		glLogCall( gl::TexStorage2D
			, m_target
			, GLsizei( getMipmapLevels() )
			, gl_renderer::getInternal( getFormat() )
			, width
			, height );
	}

	void Texture::doSetImage3D( uint32_t width
		, uint32_t height
		, uint32_t depth )
	{
		glLogCall( gl::TexStorage3D
			, m_target
			, GLsizei( getMipmapLevels() )
			, gl_renderer::getInternal( getFormat() )
			, width
			, height
			, depth );
	}

	void Texture::doSetImage2DMS( uint32_t width
		, uint32_t height
		, renderer::SampleCountFlag samples )
	{
		glLogCall( gl::TexStorage2DMultisample
			, m_target
			, GLsizei( samples )
			, gl_renderer::getInternal( getFormat() )
			, width
			, height
			, GL_TRUE );
	}

	void Texture::doSetImage3DMS( uint32_t width
		, uint32_t height
		, uint32_t depth
		, renderer::SampleCountFlag samples )
	{
		glLogCall( gl::TexStorage3DMultisample
			, m_target
			, GLsizei( samples )
			, gl_renderer::getInternal( getFormat() )
			, width
			, height
			, depth
			, GL_TRUE );
	}
}
