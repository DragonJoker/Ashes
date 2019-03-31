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
		: Image
		{
			device,
			ashes::ImageCreateInfo
			{
				0u,
				ashes::ImageType::e2D,
				format,
				ashes::Extent3D{ dimensions.width, dimensions.height, 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eColourAttachment | ashes::ImageUsageFlag::eTransferDst | ashes::ImageUsageFlag::eTransferSrc
			}
		}
	{
	}

	Image::Image( Device const & device
		, Image const & image )
		: ashes::Image{ device
			, image.m_createInfo.flags
			, image.m_createInfo.imageType
			, image.m_createInfo.format
			, image.m_createInfo.extent
			, image.m_createInfo.mipLevels
			, image.m_createInfo.arrayLayers }
		, m_device{ device }
		, m_target{ image.m_target }
		, m_createInfo{ image.m_createInfo }
		, m_texture{ image.m_texture }
		, m_ownTexture{ false }
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
		, m_target{ convert( createInfo.imageType, createInfo.arrayLayers, createInfo.samples ) }
		, m_createInfo{ createInfo }
		, m_ownTexture{ true }
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

		if ( m_ownTexture )
		{
			auto context = m_device.getContext();
			glLogCall( context
				, glDeleteTextures
				, 1
				, &m_texture );
		}
	}

	ashes::MemoryRequirements Image::getMemoryRequirements()const
	{
		ashes::MemoryRequirements result{};
		result.size = ashes::getSize( getDimensions(), getFormat() );
		result.type = ashes::ResourceType::eImage;
		auto extent = ashes::getMinimalExtent3D( getFormat() );
		result.alignment = ashes::getSize( extent, getFormat() );
		result.memoryTypeBits = ashes::MemoryPropertyFlag::eDeviceLocal
			| ( ( checkFlag( m_createInfo.usage, ashes::ImageUsageFlag::eTransferDst )
				&& checkFlag( m_createInfo.usage, ashes::ImageUsageFlag::eTransferSrc ) )
				? ashes::MemoryPropertyFlag::eHostVisible
				: ashes::MemoryPropertyFlag::eDeviceLocal );
		return result;
	}

	void Image::generateMipmaps( ashes::CommandBuffer & commandBuffer )const
	{
		static_cast< CommandBuffer & >( commandBuffer ).generateMipmaps( *this );
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
