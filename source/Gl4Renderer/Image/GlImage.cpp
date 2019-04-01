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
			VkImageCreateInfo
			{
				VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				nullptr,
				0u,
				VK_IMAGE_TYPE_2D,
				format,
				VkExtent3D{ dimensions.width, dimensions.height, 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT
			}
		}
	{
	}

	Image::Image( Device const & device
		, Image const & image )
		: m_device{ device }
		, m_target{ image.m_target }
		, m_texture{ image.m_texture }
		, m_ownTexture{ false }
		, createInfo{ image.createInfo }
	{
	}

	Image::Image( Device const & device
		, VkImageCreateInfo const & createInfo )
		: m_device{ device }
		, m_target{ convert( createInfo.imageType, createInfo.arrayLayers, createInfo.samples ) }
		, m_ownTexture{ true }
		, createInfo{ createInfo }
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

	VkMemoryRequirements Image::getMemoryRequirements()const
	{
		VkMemoryRequirements result{};
		result.size = getSize( createInfo.extent, createInfo.format );
		auto extent = getMinimalExtent3D( createInfo.format );
		result.alignment = getSize( extent, createInfo.format );
		result.memoryTypeBits = VK_MEMORY_HEAP_DEVICE_LOCAL_BIT
			| ( ( checkFlag( createInfo.usage, ashes::ImageUsageFlag::eTransferDst )
				&& checkFlag( createInfo.usage, ashes::ImageUsageFlag::eTransferSrc ) )
				? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
				: VK_MEMORY_HEAP_DEVICE_LOCAL_BIT );
		return result;
	}

	void Image::bindMemory( DeviceMemoryPtr memory )const
	{
		assert( !m_storage && "A resource can only be bound once to a device memory object." );
		m_storage = std::move( memory );
		static_cast< DeviceMemory & >( *m_storage ).bindToImage( *this, m_target, createInfo );
	}

	void Image::generateMipmaps( CommandBuffer & commandBuffer )const
	{
		static_cast< CommandBuffer & >( commandBuffer ).generateMipmaps( *this );
	}

	ImageViewPtr Image::createView( VkImageViewCreateInfo const & createInfo )const
	{
		return std::make_unique< ImageView >( m_device
			, *this
			, createInfo );
	}
}
