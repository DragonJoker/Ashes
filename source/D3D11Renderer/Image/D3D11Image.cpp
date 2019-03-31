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

namespace d3d11_renderer
{
	Image::Image( Image && rhs )
		: ashes::Image{ std::move( rhs ) }
		, m_device{ rhs.m_device }
		, m_image{ rhs.m_image }
		, m_createInfo{ std::move( rhs.m_createInfo ) }
	{
		rhs.m_image.tex1D = nullptr;
	}

	Image & Image::operator=( Image && rhs )
	{
		ashes::Image::operator=( std::move( rhs ) );
		m_image = rhs.m_image;
		rhs.m_image.tex1D = nullptr;
		return *this;
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
		, m_createInfo{ createInfo }
		, m_image{ nullptr }
	{
	}

	Image::Image( Device const & device
		, ashes::Format format
		, ashes::Extent2D const & dimensions
		, ID3D11Texture2D * image )
		: ashes::Image{ device
			, 0u
			, ashes::ImageType::e2D
			, format
			, ashes::Extent3D{ dimensions.width, dimensions.height, 1u }
			, 1u 
			, 1u }
		, m_device{ device }
		, m_image{ nullptr }
		, m_createInfo
		{
			0u,
			ashes::ImageType::e2D,
			format,
			ashes::Extent3D{ dimensions.width, dimensions.height, 1u },
			1u,
			1u,
			ashes::SampleCountFlag::e1,
			ashes::ImageTiling::eOptimal,
			( isDepthOrStencilFormat( format )
				? ashes::ImageUsageFlag::eDepthStencilAttachment
				: ashes::ImageUsageFlag::eColourAttachment ),
			ashes::SharingMode::eExclusive,
			{},
			ashes::ImageLayout::eUndefined
		}
	{
		m_image.tex2D = image;
	}

	Image::Image( Device const & device
		, ashes::Format format
		, ashes::Extent2D const & dimensions
		, ashes::ImageUsageFlags usageFlags
		, ashes::ImageTiling tiling
		, ashes::MemoryPropertyFlags memoryFlags )
		: Image{ device
			, {
				0u,
				ashes::ImageType::e2D,
				format,
				ashes::Extent3D{ dimensions.width, dimensions.height, 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				tiling,
				usageFlags,
				ashes::SharingMode::eExclusive,
				{},
				ashes::ImageLayout::eUndefined
			} }
	{
	}

	Image::~Image()
	{
		safeRelease( m_image.tex1D );
	}

	ashes::MemoryRequirements Image::getMemoryRequirements()const
	{
		ashes::MemoryRequirements result{};
		result.size = ashes::getSize( getDimensions(), getFormat() );
		result.type = ashes::ResourceType::eImage;
		result.alignment = 1u;
		result.memoryTypeBits = ashes::MemoryPropertyFlag::eDeviceLocal
			| ( ( checkFlag( m_createInfo.usage, ashes::ImageUsageFlag::eTransferDst )
				&& checkFlag( m_createInfo.usage, ashes::ImageUsageFlag::eTransferSrc ) )
				? ashes::MemoryPropertyFlag::eHostVisible
				: ashes::MemoryPropertyFlag::eDeviceLocal );
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
		case ashes::ImageType::e1D:
			m_image.tex1D = static_cast< DeviceMemory & >( *m_storage ).bindToTexture1D( m_createInfo );
			break;

		case ashes::ImageType::e2D:
			m_image.tex2D = static_cast< DeviceMemory & >( *m_storage ).bindToTexture2D( m_createInfo );
			break;

		case ashes::ImageType::e3D:
			m_image.tex3D = static_cast< DeviceMemory & >( *m_storage ).bindToTexture3D( m_createInfo );
			break;
		}
	}
}
