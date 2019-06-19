#include "Image/D3D11Image.hpp"

#include "Command/D3D11CommandBuffer.hpp"
#include "Command/D3D11Queue.hpp"
#include "Core/D3D11Device.hpp"
#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11Instance.hpp"
#include "Miscellaneous/D3D11DeviceMemory.hpp"
#include "Command/D3D11Queue.hpp"
#include "Image/D3D11ImageView.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	Image::Image( Image && rhs )
		: m_device{ rhs.m_device }
		, m_createInfo{ std::move( rhs.m_createInfo ) }
		, m_image{ rhs.m_image }
	{
		rhs.m_image.tex1D = nullptr;
	}

	Image & Image::operator=( Image && rhs )
	{
		m_image = rhs.m_image;
		m_createInfo = std::move( rhs.m_createInfo );
		rhs.m_image.tex1D = nullptr;
		return *this;
	}

	Image::Image( VkDevice device
		, VkImageCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
		, m_image{ nullptr }
	{
	}

	Image::Image( VkDevice device
		, VkFormat format
		, VkExtent2D const & dimensions
		, ID3D11Texture2D * image )
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
		m_image.tex2D = image;
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

	Image::~Image()
	{
		safeRelease( m_image.tex1D );
	}

	VkMemoryRequirements Image::getMemoryRequirements()const
	{
		VkMemoryRequirements result{};
		result.size = getTotalSize( getDimensions(), getFormat(), getLayerCount(), getMipmapLevels() );
		auto extent = ashes::getMinimalExtent3D( getFormat() );
		result.alignment = ashes::getSize( extent, getFormat() );
		result.memoryTypeBits = ( checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_TRANSFER_DST_BIT )
				|| checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_TRANSFER_SRC_BIT ) )
			? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			: VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		return result;
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
		}

		m_image.resource = m_objectMemory->resource;
		return result;
	}

	bool Image::isMapped()const
	{
		assert( m_memory != VK_NULL_HANDLE );
		return get( m_memory )->isMapped();
	}
}
