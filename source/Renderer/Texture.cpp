#include "Texture.hpp"

#include "CommandBuffer.hpp"
#include "ImageMemoryBarrier.hpp"
#include "RenderingResources.hpp"
#include "StagingBuffer.hpp"

#include <VkLib/Queue.hpp>

namespace renderer
{
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
		, CommandBuffer const & commandBuffer )
	{
		assert( ( !m_nonOwnedTexture || m_ownedTexture ) && "Can't set the image of a non owned texture" );
		setImage( format, size );
		stagingBuffer.copyTextureData( commandBuffer
			, data
			, *this );
	}

	void Texture::setImage( utils::PixelFormat format
		, IVec2 const & size )
	{
		m_format = format;
		m_size = size;
		m_ownedTexture = m_device.getDevice().createImage( convert( m_format )
			, m_size.x
			, m_size.y
			, convert( MemoryPropertyFlag::eDeviceLocal ) );
		m_nonOwnedTexture = m_ownedTexture.get();
	}

	void Texture::generateMipmaps()const noexcept
	{
		//m_texture->bind( 0 );
		//m_texture->generateMipmaps();
		//m_texture->unbind( 0 );
	}

	void Texture::bindAsShaderInput( CommandBuffer const & cb
		, uint32_t unit )const
	{
		assert( m_nonOwnedTexture );
		auto & device = m_device;
		auto & commandBuffer = cb.getCommandBuffer();

		if ( commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) )
		{
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
				, m_nonOwnedTexture->makeShaderInputResource() );
			auto res = vk::checkError( commandBuffer.end() );

			if ( !res )
			{
				throw std::runtime_error{ "Texture binding failed: " + vk::getLastError() };
			}

			res = vk::checkError( m_device.getDevice().getGraphicsQueue().submit( { commandBuffer }
				, {}
				, {}
				, {}
			, nullptr ) );

			if ( !res )
			{
				throw std::runtime_error{ "Texture binding failed: " + vk::getLastError() };
			}

			m_device.getDevice().waitIdle();
		}
	}

	void Texture::bindAsShaderOutput( CommandBuffer const & cb
		, uint32_t unit )const
	{
		assert( m_nonOwnedTexture );
		auto & device = m_device;
		auto & commandBuffer = cb.getCommandBuffer();

		if ( commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) )
		{
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
				, m_nonOwnedTexture->makeColourAttachment() );
			auto res = vk::checkError( commandBuffer.end() );

			if ( !res )
			{
				throw std::runtime_error{ "Texture binding failed: " + vk::getLastError() };
			}

			res = vk::checkError( m_device.getDevice().getGraphicsQueue().submit( { commandBuffer }
				, {}
				, {}
				, {}
			, nullptr ) );

			if ( !res )
			{
				throw std::runtime_error{ "Texture binding failed: " + vk::getLastError() };
			}

			m_device.getDevice().waitIdle();
		}
	}

	ImageMemoryBarrier Texture::makeTransferDestination()const
	{
		assert( m_nonOwnedTexture );
		return ImageMemoryBarrier{ m_nonOwnedTexture->makeTransferDestination() };
	}

	ImageMemoryBarrier Texture::makeShaderInputResource()const
	{
		assert( m_nonOwnedTexture );
		return ImageMemoryBarrier{ m_nonOwnedTexture->makeShaderInputResource() };
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
