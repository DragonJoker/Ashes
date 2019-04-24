#include "Image/D3D11Image.hpp"

#include "Command/D3D11CommandBuffer.hpp"
#include "Command/D3D11Queue.hpp"
#include "Core/D3D11Device.hpp"
#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11Instance.hpp"
#include "Miscellaneous/D3D11DeviceMemory.hpp"
#include "Command/D3D11Queue.hpp"
#include "Image/D3D11ImageView.hpp"

#include <Ashes/Miscellaneous/Extent2D.hpp>
#include <Ashes/Sync/ImageMemoryBarrier.hpp>
#include <Ashes/Image/ImageSubresourceRange.hpp>

namespace ashes::d3d11
{
	Image::Image( Image && rhs )
		: VkImage{ std::move( rhs ) }
		, m_device{ rhs.m_device }
		, m_image{ rhs.m_image }
	{
		rhs.m_image.tex1D = nullptr;
	}

	Image & Image::operator=( Image && rhs )
	{
		VkImage::operator=( std::move( rhs ) );
		m_image = rhs.m_image;
		rhs.m_image.tex1D = nullptr;
		return *this;
	}

	Image::Image( VkDevice device
		, VkImageCreateInfo const & createInfo )
		: VkImage{ device, createInfo }
		, m_device{ device }
		, m_image{ nullptr }
	{
	}

	Image::Image( VkDevice device
		, VkFormat format
		, VkExtent2D const & dimensions
		, ID3D11Texture2D * image )
		: VkImage{ device
			, {
				0u,
				VK_IMAGE_TYPE_2D,
				format,
				VkExtent3D{ dimensions.width, dimensions.height, 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				( isDepthOrStencilFormat( format )
					? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
					: VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT ),
				VK_SHARING_MODE_EXCLUSIVE,
				{},
				VK_IMAGE_LAYOUT_UNDEFINED
			} }
		, m_device{ device }
		, m_image{ nullptr }
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
				{},
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
		result.size = ashes::getSize( getDimensions(), getFormat() );
		result.type = ashes::ResourceType::eImage;
		auto extent = ashes::getMinimalExtent3D( getFormat() );
		result.alignment = ashes::getSize( extent, getFormat() );
		result.memoryTypeBits = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			| ( ( checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_TRANSFER_DST_BIT )
				&& checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_TRANSFER_SRC_BIT ) )
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

	void Image::generateMipmaps( ashes::CommandBuffer & commandBuffer )const
	{
		static_cast< CommandBuffer & >( commandBuffer ).generateMips( *this );
	}

	void Image::doBindMemory()
	{
		switch ( getType() )
		{
		case VK_IMAGE_TYPE_1D:
			m_image.tex1D = static_cast< DeviceMemory & >( *m_storage ).bindToTexture1D( m_createInfo );
			break;

		case VK_IMAGE_TYPE_2D:
			m_image.tex2D = static_cast< DeviceMemory & >( *m_storage ).bindToTexture2D( m_createInfo );
			break;

		case VK_IMAGE_TYPE_3D:
			m_image.tex3D = static_cast< DeviceMemory & >( *m_storage ).bindToTexture3D( m_createInfo );
			break;
		}
	}
}
