#include "Image/GlImage.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Image/GlImageView.hpp"
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
		GlTextureType convert( ashes::ImageType type
			, uint32_t layerCount
			, ashes::ImageCreateFlags flags
			, ashes::SampleCountFlag samples )
		{
			GlTextureType result;

			switch ( type )
			{
			case ashes::ImageType::e1D:
				if ( layerCount > 1 )
				{
					result = GL_TEXTURE_1D_ARRAY;
				}
				else
				{
					result = GL_TEXTURE_1D;
				}
				break;
			case ashes::ImageType::e2D:
				if ( layerCount > 1 )
				{
					if ( samples > ashes::SampleCountFlag::e1 )
					{
						result = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
					}
					else if ( checkFlag( flags, ashes::ImageCreateFlag::eCubeCompatible ) )
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
					if ( samples != ashes::SampleCountFlag::e1 )
					{
						result = GL_TEXTURE_2D_MULTISAMPLE;
					}
					else
					{
						result = GL_TEXTURE_2D;
					}
				}
				break;
			case ashes::ImageType::e3D:
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

	Image::Image( Device const & device
		, ashes::Format format
		, ashes::Extent2D const & dimensions )
		: ashes::Image{ device
			, 0u
			, ashes::ImageType::e2D
			, format
			, { dimensions.width, dimensions.height, 1u }
			, 1u
			, 1u }
		, m_device{ device }
		, m_createInfo{ 
			0u,
			ashes::ImageType::e2D,
			format,
			{ dimensions.width, dimensions.height, 1u },
			1u,
			1u,
			ashes::SampleCountFlag::e1,
			ashes::ImageTiling::eOptimal,
			0u
		}
	{
	}

	Image::Image( Device const & device
		, ashes::ImageCreateInfo const & createInfo )
		: ashes::Image{ device
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
		auto context = m_device.getContext();
		glLogCall( context
			, glGenTextures
			, 1
			, &m_texture );
	}

	Image::~Image()
	{
		m_storage.reset();
		auto context = m_device.getContext();
		glLogCall( context
			, glDeleteTextures
			, 1
			, &m_texture );
	}

	void Image::generateMipmaps( ashes::CommandBuffer & commandBuffer )const
	{
		static_cast< CommandBuffer & >( commandBuffer ).generateMipmaps( *this );
	}

	ashes::MemoryRequirements Image::getMemoryRequirements()const
	{
		ashes::MemoryRequirements result{};

		if ( !ashes::isCompressedFormat( getFormat() ) )
		{
			result.size = getDimensions().width
				* getDimensions().height
				* getDimensions().depth
				* getLayerCount()
				* ashes::getSize( getFormat() );
		}

		result.type = ashes::ResourceType::eImage;
		result.alignment = 1u;
		result.memoryTypeBits = ~result.memoryTypeBits;
		return result;
	}

	ashes::ImageViewPtr Image::createView( ashes::ImageViewCreateInfo const & createInfo )const
	{
		return std::make_shared< ImageView >( m_device
			, *this
			, createInfo );
	}

	void Image::doBindMemory()
	{
		static_cast< DeviceMemory & >( *m_storage ).bindToImage( *this, m_target, m_createInfo );
	}
}