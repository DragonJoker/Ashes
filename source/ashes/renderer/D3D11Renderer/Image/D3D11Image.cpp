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
	Image::Image( Image && rhs )noexcept
		: m_device{ rhs.m_device }
		, m_createInfo{ std::move( rhs.m_createInfo ) }
		, m_image{ rhs.m_image }
		, m_memoryRequirements{ rhs.m_memoryRequirements }
	{
		rhs.m_image.tex1D = nullptr;
	}

	Image & Image::operator=( Image && rhs )noexcept
	{
		m_image = rhs.m_image;
		m_memoryRequirements = rhs.m_memoryRequirements;
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
		doInitialiseMemoryRequirements();
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
		, VkImageTiling tiling )
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
		doInitialiseMemoryRequirements();
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
		auto physicalDevice = get( get( m_device )->getPhysicalDevice() );
		auto extent = ashes::getMinimalExtent3D( getFormat() );
		m_memoryRequirements.alignment = ashes::getSize( extent, getFormat() );
		m_memoryRequirements.size = getTotalSize( getDimensions(), getFormat(), getLayerCount(), getMipmapLevels(), uint32_t( m_memoryRequirements.alignment ) );
		m_memoryRequirements.memoryTypeBits = physicalDevice->getMemoryTypeBits( VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT );
		m_memoryRequirements.size = ashes::getAlignedSize( m_memoryRequirements.size, m_memoryRequirements.alignment );
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

		if ( !m_debugName.empty() )
		{
			m_objectMemory->resource->SetPrivateData( WKPDID_D3DDebugObjectName
				, UINT( m_debugName.size() )
				, m_debugName.c_str() );
		}

		m_image.resource = m_objectMemory->resource;
		return result;
	}

	bool Image::isMapped()const
	{
		assert( m_memory != nullptr );
		return get( m_memory )->isMapped();
	}
}
