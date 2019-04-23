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
		GlTextureType convert( VkImageType type
			, uint32_t layerCount
			, VkImageCreateFlags flags
			, VkSampleCountFlagBits samples )
		{
			GlTextureType result;

			switch ( type )
			{
			case VK_IMAGE_TYPE_1D:
				if ( layerCount > 1 )
				{
					result = GL_TEXTURE_1D_ARRAY;
				}
				else
				{
					result = GL_TEXTURE_1D;
				}
				break;
			case VK_IMAGE_TYPE_2D:
				if ( layerCount > 1 )
				{
					if ( samples > VK_SAMPLE_COUNT_1_BIT )
					{
						result = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
					}
					else if ( checkFlag( flags, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT ) )
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
					if ( samples != VK_SAMPLE_COUNT_1_BIT )
					{
						result = GL_TEXTURE_2D_MULTISAMPLE;
					}
					else
					{
						result = GL_TEXTURE_2D;
					}
				}
				break;
			case VK_IMAGE_TYPE_3D:
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
		, VkFormat format
		, VkExtent2D const & dimensions )
		: Image
		{
			device,
			ashes::ImageCreateInfo
			{
				0u,
				VK_IMAGE_TYPE_2D,
				format,
				VkExtent3D{ dimensions.width, dimensions.height, 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				ashes::ImageUsageFlag::eColourAttachment | ashes::ImageUsageFlag::eTransferDst | ashes::ImageUsageFlag::eTransferSrc
			}
		}
	{
	}

	Image::Image( Device const & device
		, Image const & image )
		: ashes::Image{ device, image.m_createInfo }
		, m_device{ device }
		, m_target{ image.m_target }
		, m_texture{ image.m_texture }
		, m_ownTexture{ false }
	{
	}

	Image::Image( Device const & device
		, ashes::ImageCreateInfo const & createInfo )
		: ashes::Image{ device, createInfo }
		, m_device{ device }
		, m_target{ convert( createInfo.imageType, createInfo.arrayLayers, createInfo.flags, createInfo.samples ) }
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

	void Image::generateMipmaps( ashes::CommandBuffer & commandBuffer )const
	{
		static_cast< CommandBuffer & >( commandBuffer ).generateMipmaps( *this );
	}

	ashes::MemoryRequirements Image::getMemoryRequirements()const
	{
		ashes::MemoryRequirements result{};
		result.size = ashes::getSize( getDimensions(), getFormat() );
		result.type = ashes::ResourceType::eImage;
		auto extent = ashes::getMinimalExtent3D( getFormat() );
		result.alignment = ashes::getSize( extent, getFormat() );
		result.memoryTypeBits = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			| ( ( checkFlag( m_createInfo.usage, ashes::ImageUsageFlag::eTransferDst )
				&& checkFlag( m_createInfo.usage, ashes::ImageUsageFlag::eTransferSrc ) )
				? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
				: VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
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
