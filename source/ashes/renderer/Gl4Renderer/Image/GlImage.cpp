#include "Image/GlImage.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"

#include "ashesgl4_api.hpp"

#ifdef max
#	undef max
#	undef min
#endif

namespace ashes::gl4
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

	Image::Image( VkDevice device
		, VkFormat format
		, VkExtent2D const & dimensions
		, bool swapchainImage )
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
			},
			swapchainImage
		}
	{
	}

	Image::Image( VkDevice device
		, VkImageCreateInfo createInfo
		, bool swapchainImage )
		: m_flags{ createInfo.flags }
		, m_imageType{ createInfo.imageType }
		, m_format{ createInfo.format }
		, m_extent{ createInfo.extent }
		, m_mipLevels{ createInfo.mipLevels }
		, m_arrayLayers{ createInfo.arrayLayers }
		, m_samples{ createInfo.samples }
		, m_tiling{ createInfo.tiling }
		, m_usage{ createInfo.usage }
		, m_sharingMode{ createInfo.sharingMode }
		, m_queueFamilyIndices{ createInfo.pQueueFamilyIndices, createInfo.pQueueFamilyIndices + createInfo.queueFamilyIndexCount }
		, m_device{ device }
		, m_target{ convert( getType(), getArrayLayers(), getSamples() ) }
		, m_swapchainImage{ swapchainImage }
	{
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glGenTextures
			, 1
			, &m_internal );
		glLogCall( context
			, glBindTexture
			, m_target
			, m_internal );
		glLogCall( context
			, glBindTexture
			, m_target
			, 0 );
		doInitialiseMemoryRequirements();
	}

	Image::~Image()
	{
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glDeleteTextures
			, 1
			, &m_internal );
	}

	VkMemoryRequirements Image::getMemoryRequirements()const
	{
		return m_memoryRequirements;
	}

	std::vector< VkSparseImageMemoryRequirements > Image::getSparseImageMemoryRequirements()const
	{
		return {};
	}

	void Image::doInitialiseMemoryRequirements()
	{
		m_memoryRequirements.size = getTotalSize( getDimensions(), getFormat(), getArrayLayers(), getMipLevels() );
		auto extent = ashes::getMinimalExtent3D( getFormat() );
		m_memoryRequirements.alignment = getSize( extent, getFormat() );
		m_memoryRequirements.memoryTypeBits = ( checkFlag( getUsage(), VK_IMAGE_USAGE_TRANSFER_DST_BIT )
			|| checkFlag( getUsage(), VK_IMAGE_USAGE_TRANSFER_SRC_BIT ) )
			? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			: VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		m_memoryRequirements.size = ashes::getAlignedSize( m_memoryRequirements.size, m_memoryRequirements.alignment );
	}
}
