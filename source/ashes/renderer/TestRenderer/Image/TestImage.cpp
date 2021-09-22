#include "Image/TestImage.hpp"

#include "Command/TestCommandBuffer.hpp"
#include "Command/TestQueue.hpp"
#include "Core/TestDevice.hpp"
#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestInstance.hpp"
#include "Miscellaneous/TestDeviceMemory.hpp"
#include "Command/TestQueue.hpp"
#include "Image/TestImageView.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	Image::Image( Image && rhs )noexcept
		: m_device{ rhs.m_device }
		, m_createInfo{ std::move( rhs.m_createInfo ) }
	{
	}

	Image & Image::operator=( Image && rhs )noexcept
	{
		m_createInfo = std::move( rhs.m_createInfo );
		return *this;
	}

	Image::Image( VkDevice device
		, VkImageCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
	}

	Image::Image( VkDevice device
		, VkFormat format
		, VkExtent2D const & dimensions )
		: Image{ device
			, {
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
				VkImageUsageFlags( isDepthOrStencilFormat( format )
					? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
					: VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT ),
				VK_SHARING_MODE_EXCLUSIVE,
				0u,
				nullptr,
				VK_IMAGE_LAYOUT_UNDEFINED
			} }
	{
	}

	Image::Image( VkDevice device
		, VkFormat format
		, VkExtent2D const & dimensions
		, VkImageUsageFlags usageFlags
		, VkImageTiling tiling
		, VkMemoryPropertyFlags memoryFlags )
		: Image{ device
			, {
				VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				nullptr,
				0u,
				VK_IMAGE_TYPE_2D,
				format,
				VkExtent3D{ dimensions.width, dimensions.height, 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				tiling,
				usageFlags,
				VK_SHARING_MODE_EXCLUSIVE,
				0u,
				nullptr,
				VK_IMAGE_LAYOUT_UNDEFINED
			} }
	{
	}

	VkMemoryRequirements Image::getMemoryRequirements()const
	{
		VkMemoryRequirements result{};
		auto extent = ashes::getMinimalExtent3D( getFormat() );
		result.alignment = ashes::getSize( extent, getFormat() );
		result.size = getTotalSize( getDimensions(), getFormat(), getLayerCount(), getMipmapLevels(), uint32_t( result.alignment ) );
		result.memoryTypeBits = uint32_t( ( checkFlag( getUsage(), VK_IMAGE_USAGE_TRANSFER_DST_BIT )
				|| checkFlag( getUsage(), VK_IMAGE_USAGE_TRANSFER_SRC_BIT ) )
			? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			: VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		return result;
	}

	std::vector< VkSparseImageMemoryRequirements > Image::getSparseImageMemoryRequirements()const
	{
		return {};
	}

	void Image::generateMipmaps( VkCommandBuffer commandBuffer )const
	{
		get( commandBuffer )->generateMipmaps( get( this ) );
	}

	VkResult Image::bindMemory( VkDeviceMemory memory
		, VkDeviceSize memoryOffset )
	{
		m_memory = memory;
		m_memoryOffset = memoryOffset;
		VkResult result = VK_SUCCESS;

		switch ( getType() )
		{
		case VK_IMAGE_TYPE_1D:
			result = get( m_memory )->bindToImage1D( get( this )
				, m_memoryOffset
				, m_objectMemory );
			break;
		case VK_IMAGE_TYPE_2D:
			result = get( m_memory )->bindToImage2D( get( this )
				, m_memoryOffset
				, m_objectMemory );
			break;
		case VK_IMAGE_TYPE_3D:
			result = get( m_memory )->bindToImage3D( get( this )
				, m_memoryOffset
				, m_objectMemory );
			break;
		default:
			break;
		}

		return result;
	}

	bool Image::isMapped()const
	{
		assert( m_memory != nullptr );
		return get( m_memory )->isMapped();
	}
}
