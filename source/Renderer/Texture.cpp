#include "Texture.hpp"

#include "CommandBuffer.hpp"
#include "ImageMemoryBarrier.hpp"
#include "RenderingResources.hpp"
#include "StagingBuffer.hpp"

#include <VkLib/Queue.hpp>

namespace renderer
{
	namespace
	{
		Texture::Mapped convert( vk::Image::Mapped const & mapped )
		{
			return Texture::Mapped
			{
				mapped.data,
				mapped.size,
				mapped.rowPitch,
				mapped.arrayPitch,
				mapped.depthPitch,
			};
		}
	}

	Texture::Texture( Device const & device )
		: m_device{ device }
	{
	}

	Texture::Texture( Device const & device
		, vk::Image const & image )
		: m_device{ device }
		, m_nonOwnedTexture{ &image }
	{
	}

	void Texture::setImage( utils::PixelFormat format
		, IVec2 const & size
		, ByteArray const & data
		, StagingBuffer const & stagingBuffer
		, CommandBuffer const & commandBuffer
		, ImageUsageFlags usageFlags
		, ImageTiling tiling )
	{
		assert( ( !m_nonOwnedTexture || m_ownedTexture ) && "Can't set the image of a non owned texture" );
		setImage( format
			, size
			, usageFlags
			, tiling );
		stagingBuffer.copyTextureData( commandBuffer
			, data
			, *this );
	}

	void Texture::setImage( utils::PixelFormat format
		, IVec2 const & size
		, ImageUsageFlags usageFlags
		, ImageTiling tiling )
	{
		m_format = format;
		m_size = size;
		m_ownedTexture = m_device.getDevice().createImage( convert( m_format )
			, m_size.x
			, m_size.y
			, convert( MemoryPropertyFlag::eDeviceLocal )
			, convert( usageFlags )
			, convert( tiling ) );
		m_nonOwnedTexture = m_ownedTexture.get();
	}

	void Texture::generateMipmaps()const noexcept
	{
		//m_texture->bind( 0 );
		//m_texture->generateMipmaps();
		//m_texture->unbind( 0 );
	}

	Texture::Mapped Texture::lock( uint32_t offset
		, uint32_t size
		, VkMemoryMapFlags flags )const
	{
		return convert( m_nonOwnedTexture->lock( offset
			, size
			, flags ) );
	}

	void Texture::unlock( uint32_t size
		, bool modified )const
	{
	}

	void Texture::bindAsShaderInput( CommandBuffer const & commandBuffer
		, uint32_t unit )const
	{
		assert( m_nonOwnedTexture );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
			, m_nonOwnedTexture->makeShaderInputResource() );
	}

	void Texture::bindAsShaderOutput( CommandBuffer const & commandBuffer
		, uint32_t unit )const
	{
		assert( m_nonOwnedTexture );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
			, m_nonOwnedTexture->makeColourAttachment() );
	}

	ImageMemoryBarrier Texture::makeGeneralLayout( AccessFlags accessFlags )const
	{
		assert( m_nonOwnedTexture );
		return ImageMemoryBarrier{ m_nonOwnedTexture->makeGeneralLayout( convert( accessFlags ) ) };
	}

	ImageMemoryBarrier Texture::makeTransferDestination()const
	{
		assert( m_nonOwnedTexture );
		return ImageMemoryBarrier{ m_nonOwnedTexture->makeTransferDestination() };
	}

	ImageMemoryBarrier Texture::makeTransferSource()const
	{
		assert( m_nonOwnedTexture );
		return ImageMemoryBarrier{ m_nonOwnedTexture->makeTransferSource() };
	}

	ImageMemoryBarrier Texture::makeShaderInputResource()const
	{
		assert( m_nonOwnedTexture );
		return ImageMemoryBarrier{ m_nonOwnedTexture->makeShaderInputResource() };
	}

	ImageMemoryBarrier Texture::makeDepthStencilReadOnly()const
	{
		assert( m_nonOwnedTexture );
		return ImageMemoryBarrier{ m_nonOwnedTexture->makeDepthStencilReadOnly() };
	}

	ImageMemoryBarrier Texture::makeColourAttachment()const
	{
		assert( m_nonOwnedTexture );
		return ImageMemoryBarrier{ m_nonOwnedTexture->makeColourAttachment() };
	}

	ImageMemoryBarrier Texture::makeDrawDestination()const
	{
		assert( m_nonOwnedTexture );
		return ImageMemoryBarrier{ m_nonOwnedTexture->makeDrawDestination() };
	}

	ImageMemoryBarrier Texture::makePresentSource()const
	{
		assert( m_nonOwnedTexture );
		return ImageMemoryBarrier{ m_nonOwnedTexture->makePresentSource() };
	}
}
