#include "Image/GlTexture.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Image/GlTextureView.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"
#include "Sync/GlImageMemoryBarrier.hpp"

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
			, renderer::ImageCreateFlags flags
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
					else if ( checkFlag( flags, renderer::ImageCreateFlag::eCubeCompatible ) )
					{
						assert( ( layerCount % 6 ) == 0 );

						if ( layerCount > 6 )
						{
							result = GL_TEXTURE_CUBE_ARRAY;
						}
						else
						{
							result = GL_TEXTURE_CUBE;
						}
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
			, 0u
			, renderer::TextureType::e2D
			, format
			, { dimensions.width, dimensions.height, 1u }
			, 1u
			, 1u }
		, m_device{ device }
		, m_createInfo{ 
			0u,
			renderer::TextureType::e2D,
			format,
			{ dimensions.width, dimensions.height, 1u },
			1u,
			1u,
			renderer::SampleCountFlag::e1,
			renderer::ImageTiling::eOptimal,
			0u
		}
	{
	}

	Texture::Texture( Device const & device
		, renderer::ImageCreateInfo const & createInfo )
		: renderer::Texture{ device
			, createInfo.flags
			, createInfo.imageType
			, createInfo.format
			, createInfo.extent
			, createInfo.mipLevels
			, createInfo.arrayLayers }
		, m_device{ device }
		, m_target{ convert( createInfo.imageType, createInfo.arrayLayers, createInfo.flags, createInfo.samples ) }
		, m_createInfo{ createInfo }
	{
		glLogCall( m_device.getContext(), glGenTextures, 1, &m_texture );
	}

	Texture::~Texture()
	{
		m_storage.reset();
		glLogCall( m_device.getContext(), glDeleteTextures, 1, &m_texture );
	}

	void Texture::generateMipmaps( renderer::CommandBuffer & commandBuffer )const
	{
		static_cast< CommandBuffer & >( commandBuffer ).generateMipmaps( *this );
	}

	renderer::MemoryRequirements Texture::getMemoryRequirements()const
	{
		renderer::MemoryRequirements result{};

		if ( !renderer::isCompressedFormat( getFormat() ) )
		{
			result.size = getDimensions().width
				* getDimensions().height
				* getDimensions().depth
				* getLayerCount()
				* renderer::getSize( getFormat() );
		}

		result.type = renderer::ResourceType::eImage;
		result.alignment = 1u;
		result.memoryTypeBits = 0xFFFFFFFF;
		return result;
	}

	renderer::TextureViewPtr Texture::createView( renderer::ImageViewCreateInfo const & createInfo )const
	{
		return std::make_unique< TextureView >( m_device
			, *this
			, createInfo );
	}

	void Texture::doBindMemory()
	{
		static_cast< DeviceMemory & >( *m_storage ).bindToImage( *this, m_target, m_createInfo );
	}
}
