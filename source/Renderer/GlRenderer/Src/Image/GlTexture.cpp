#include "Image/GlTexture.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Sync/GlImageMemoryBarrier.hpp"
#include "Core/GlRenderingResources.hpp"
#include "Image/GlTextureView.hpp"

namespace gl_renderer
{
	namespace
	{
		renderer::ImageAspectFlags getAspectFlag( renderer::PixelFormat format )
		{
			switch ( format )
			{
			case renderer::PixelFormat::eL8:
			case renderer::PixelFormat::eL8A8:
			case renderer::PixelFormat::eR8G8B8:
			case renderer::PixelFormat::eRGB565:
			case renderer::PixelFormat::eR8G8B8A8:
			case renderer::PixelFormat::eB8G8R8A8:
			case renderer::PixelFormat::eRGBA5551:
			case renderer::PixelFormat::eRGBA4444:
				return renderer::ImageAspectFlag::eColour;

			case renderer::PixelFormat::eD16:
			case renderer::PixelFormat::eD32F:
				return renderer::ImageAspectFlag::eDepth;

			case renderer::PixelFormat::eD24S8:
				return renderer::ImageAspectFlag::eDepth | renderer::ImageAspectFlag::eStencil;

			case renderer::PixelFormat::eS8:
				return renderer::ImageAspectFlag::eStencil;

			default:
				assert( false && "Unsupported pixel format." );
				return renderer::ImageAspectFlag::eColour;
			}
		}
	}

	Texture::Texture( Device const & device )
		: renderer::Texture{ device }
		, m_device{ device }
	{
		glLogCall( gl::GenTextures, 1, &m_texture );
	}

	Texture::~Texture()
	{
		glLogCall( gl::DeleteTextures, 1, &m_texture );
	}

	void Texture::doSetImage1D( renderer::ImageUsageFlags usageFlags
		, renderer::ImageTiling tiling
		, renderer::MemoryPropertyFlags memoryFlags )
	{
		if ( m_layerCount )
		{
			m_target = GL_TEXTURE_1D_ARRAY;
		}
		else
		{
			m_target = GL_TEXTURE_1D;
		}

		glLogCall( gl::BindTexture, m_target, m_texture );

		if ( m_layerCount )
		{
			glLogCall( gl::TexStorage2D
				, m_target
				, 1
				, gl_renderer::getInternal( m_format )
				, m_size[0]
				, m_layerCount );
		}
		else
		{
			glLogCall( gl::TexStorage1D
				, m_target
				, 1
				, gl_renderer::getInternal( m_format )
				, m_size[0] );
		}

		glLogCall( gl::BindTexture, m_target, 0 );
	}

	void Texture::doSetImage2D( renderer::ImageUsageFlags usageFlags
		, renderer::ImageTiling tiling
		, renderer::MemoryPropertyFlags memoryFlags )
	{
		if ( m_layerCount )
		{
			if ( m_samples > renderer::SampleCountFlag::e1 )
			{
				m_target = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
			}
			else
			{
				m_target = GL_TEXTURE_2D_ARRAY;
			}
		}
		else if ( m_samples != renderer::SampleCountFlag::e1 )
		{
			m_target = GL_TEXTURE_2D_MULTISAMPLE;
		}
		else
		{
			m_target = GL_TEXTURE_2D;
		}

		glLogCall( gl::BindTexture, m_target, m_texture );

		if ( m_layerCount )
		{
			if ( m_samples > renderer::SampleCountFlag::e1 )
			{
				glLogCall( gl::TexStorage3DMultisample
					, m_target
					, GLsizei( m_samples )
					, gl_renderer::getInternal( m_format )
					, m_size[0]
					, m_size[1]
					, m_layerCount
					, GL_TRUE );
			}
			else
			{
				glLogCall( gl::TexStorage3D
					, m_target
					, 1
					, gl_renderer::getInternal( m_format )
					, m_size[0]
					, m_size[1]
					, m_layerCount );
			}
		}
		else if ( m_samples != renderer::SampleCountFlag::e1 )
		{
			glLogCall( gl::TexStorage2DMultisample
				, m_target
				, GLsizei( m_samples )
				, gl_renderer::getInternal( m_format )
				, m_size[0]
				, m_size[1]
				, GL_TRUE );
		}
		else
		{
			glLogCall( gl::TexStorage2D
				, m_target
				, 1
				, gl_renderer::getInternal( m_format )
				, m_size[0]
				, m_size[1] );
		}

		glLogCall( gl::BindTexture, m_target, 0 );
	}

	void Texture::doSetImage3D( renderer::ImageUsageFlags usageFlags
		, renderer::ImageTiling tiling
		, renderer::MemoryPropertyFlags memoryFlags )
	{
		m_target = GL_TEXTURE_3D;
		glLogCall( gl::BindTexture, m_target, m_texture );
		glLogCall( gl::TexStorage3D
			, m_target
			, 1
			, gl_renderer::getInternal( m_format )
			, m_size[0]
			, m_size[1]
			, m_size[2] );
		glLogCall( gl::BindTexture, m_target, 0 );
	}

	void Texture::generateMipmaps()const
	{
		glLogCall( gl::BindTexture, m_target, m_texture );
		glLogCall( gl::GenerateMipmap, m_target );
		glLogCall( gl::BindTexture, m_target, 0 );
	}

	renderer::ImageMemoryBarrier Texture::makeGeneralLayout( renderer::ImageSubresourceRange const & range
		, renderer::AccessFlags accessFlags )const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::eGeneral
			, ~( 0u )
			, accessFlags
			, range );
	}

	renderer::ImageMemoryBarrier Texture::makeTransferDestination( renderer::ImageSubresourceRange const & range )const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::eTransferDstOptimal
			, ~( 0u )
			, renderer::AccessFlag::eTransferWrite
			, range );
	}

	renderer::ImageMemoryBarrier Texture::makeTransferSource( renderer::ImageSubresourceRange const & range )const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::eTransferSrcOptimal
			, ~( 0u )
			, renderer::AccessFlag::eShaderRead
			, range );
	}

	renderer::ImageMemoryBarrier Texture::makeShaderInputResource( renderer::ImageSubresourceRange const & range )const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::eShaderReadOnlyOptimal
			, ~( 0u )
			, renderer::AccessFlag::eTransferRead
			, range );
	}

	renderer::ImageMemoryBarrier Texture::makeDepthStencilReadOnly( renderer::ImageSubresourceRange const & range )const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::eDepthStencilReadOnlyOptimal
			, ~( 0u )
			, renderer::AccessFlag::eShaderRead
			, range );
	}

	renderer::ImageMemoryBarrier Texture::makeColourAttachment( renderer::ImageSubresourceRange const & range )const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::eColourAttachmentOptimal
			, ~( 0u )
			, renderer::AccessFlag::eColourAttachmentWrite
			, range );
	}

	renderer::ImageMemoryBarrier Texture::makeDepthStencilAttachment( renderer::ImageSubresourceRange const & range )const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::eDepthStencilAttachmentOptimal
			, ~( 0u )
			, renderer::AccessFlag::eColourAttachmentWrite
			, range );
	}

	renderer::ImageMemoryBarrier Texture::makePresentSource( renderer::ImageSubresourceRange const & range )const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::ePresentSrc
			, ~( 0u )
			, renderer::AccessFlag::eMemoryRead
			, range );
	}

	renderer::ImageMemoryBarrier Texture::doMakeLayoutTransition( renderer::ImageLayout layout
		, uint32_t queueFamily
		, renderer::AccessFlags dstAccessMask
		, renderer::ImageSubresourceRange const & range )const
	{
		return renderer::ImageMemoryBarrier
		{
			0u,                                      // srcAccessMask
			dstAccessMask,                           // dstAccessMask
			renderer::ImageLayout::eUndefined,       // oldLayout
			layout,                                  // newLayout
			~( 0u ),                                 // srcQueueFamilyIndex
			queueFamily,                             // dstQueueFamilyIndex
			*this,                                   // image
			range                                    // subresourceRange
		};
	}
}
