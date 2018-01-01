#include "VkTexture.hpp"

#include "VkCommandBuffer.hpp"
#include "VkDevice.hpp"
#include "VkImageMemoryBarrier.hpp"
#include "VkImageSubresourceRange.hpp"
#include "VkRenderingResources.hpp"
#include "VkStagingBuffer.hpp"

#include <VkLib/Queue.hpp>

namespace vk_renderer
{
	namespace
	{
		renderer::ImageMemoryBarrier doConvert( Texture const & texture
			, VkImageMemoryBarrier const & barrier )
		{
			return renderer::ImageMemoryBarrier
			{
				convertAccessFlags( barrier.srcAccessMask ),
				convertAccessFlags( barrier.dstAccessMask ),
				convertImageLayout( barrier.oldLayout ),
				convertImageLayout( barrier.newLayout ),
				barrier.srcQueueFamilyIndex,
				barrier.dstQueueFamilyIndex,
				texture,
				convert( barrier.subresourceRange )
			};
		}
	}

	Texture::Texture( renderer::Device const & device )
		: renderer::Texture{ device }
	{
	}

	Texture::Texture( renderer::Device const & device
		, vk::Image const & image )
		: renderer::Texture{ device }
		, m_nonOwnedTexture{ &image }
	{
	}

	void Texture::setImage( utils::PixelFormat format
		, utils::IVec2 const & size
		, renderer::ImageUsageFlags usageFlags
		, renderer::ImageTiling tiling )
	{
		m_format = format;
		m_size = size;
		m_ownedTexture = static_cast< Device const & >( m_device ).getDevice().createImage( convert( m_format )
			, m_size.x
			, m_size.y
			, convert( renderer::MemoryPropertyFlag::eDeviceLocal )
			, convert( usageFlags )
			, convert( tiling ) );
		m_nonOwnedTexture = m_ownedTexture.get();
	}

	void Texture::generateMipmaps()const
	{
		//m_texture->bind( 0 );
		//m_texture->generateMipmaps();
		//m_texture->unbind( 0 );
	}

	void Texture::bindAsShaderInput( renderer::CommandBuffer const & commandBuffer
		, uint32_t unit )const
	{
		assert( m_nonOwnedTexture );
		static_cast< CommandBuffer const & >( commandBuffer ).getCommandBuffer().memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
			, m_nonOwnedTexture->makeShaderInputResource() );
	}

	void Texture::bindAsShaderOutput( renderer::CommandBuffer const & commandBuffer
		, uint32_t unit )const
	{
		assert( m_nonOwnedTexture );
		static_cast< CommandBuffer const & >( commandBuffer ).getCommandBuffer().memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
			, m_nonOwnedTexture->makeColourAttachment() );
	}

	renderer::ImageMemoryBarrier Texture::makeGeneralLayout( renderer::AccessFlags accessFlags )const
	{
		assert( m_nonOwnedTexture );
		return doConvert( *this, m_nonOwnedTexture->makeGeneralLayout( convert( accessFlags ) ) );
	}

	renderer::ImageMemoryBarrier Texture::makeTransferDestination()const
	{
		assert( m_nonOwnedTexture );
		return doConvert( *this, m_nonOwnedTexture->makeTransferDestination() );
	}

	renderer::ImageMemoryBarrier Texture::makeTransferSource()const
	{
		assert( m_nonOwnedTexture );
		return doConvert( *this, m_nonOwnedTexture->makeTransferSource() );
	}

	renderer::ImageMemoryBarrier Texture::makeShaderInputResource()const
	{
		assert( m_nonOwnedTexture );
		return doConvert( *this, m_nonOwnedTexture->makeShaderInputResource() );
	}

	renderer::ImageMemoryBarrier Texture::makeDepthStencilReadOnly()const
	{
		assert( m_nonOwnedTexture );
		return doConvert( *this, m_nonOwnedTexture->makeDepthStencilReadOnly() );
	}

	renderer::ImageMemoryBarrier Texture::makeColourAttachment()const
	{
		assert( m_nonOwnedTexture );
		return doConvert( *this, m_nonOwnedTexture->makeColourAttachment() );
	}

	renderer::ImageMemoryBarrier Texture::makeDepthStencilAttachment()const
	{
		assert( m_nonOwnedTexture );
		return doConvert( *this, m_nonOwnedTexture->makeDepthStencilAttachment() );
	}

	renderer::ImageMemoryBarrier Texture::makePresentSource()const
	{
		assert( m_nonOwnedTexture );
		return doConvert( *this, m_nonOwnedTexture->makePresentSource() );
	}
}
