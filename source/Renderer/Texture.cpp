#include "Texture.hpp"

#include "CommandBuffer.hpp"
#include "ImageMemoryBarrier.hpp"
#include "RenderingResources.hpp"
#include "StagingBuffer.hpp"

#include <VkLib/Queue.hpp>

namespace renderer
{
	Texture::Texture( RenderingResources const & resources )
		: m_resources{ resources }
	{
	}

	void Texture::setImage( PixelFormat format
		, IVec2 const & size
		, ByteArray const & data )
	{
		setImage( format, size );
		m_resources.getStagingBuffer().copyTextureData( data, *this );
	}

	void Texture::setImage( PixelFormat format
		, IVec2 const & size )
	{
		m_format = format;
		m_size = size;
		m_texture = m_resources.getDevice().createImage( convert( m_format )
			, m_size.x
			, m_size.y
			, convert( MemoryPropertyFlag::eDeviceLocal ) );
	}

	void Texture::generateMipmaps()const noexcept
	{
		//m_texture->bind( 0 );
		//m_texture->generateMipmaps();
		//m_texture->unbind( 0 );
	}

	void Texture::bindAsShaderInput( uint32_t unit )const
	{
		assert( m_texture );
		auto & device = m_resources.getDevice();
		auto & commandBuffer = m_resources.getCommandBuffer().getCommandBuffer();

		if ( commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) )
		{
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
				, m_texture->makeShaderInputResource() );
			auto res = vk::checkError( commandBuffer.end() );

			if ( !res )
			{
				throw std::runtime_error{ "Texture binding failed: " + vk::getLastError() };
			}

			res = vk::checkError( device.getGraphicsQueue().submit( { commandBuffer }
				, {}
				, {}
				, {}
			, nullptr ) );

			if ( !res )
			{
				throw std::runtime_error{ "Texture binding failed: " + vk::getLastError() };
			}

			device.waitIdle();
		}
	}

	void Texture::bindAsShaderOutput( uint32_t unit )const
	{
		assert( m_texture );
		auto & device = m_resources.getDevice();
		auto & commandBuffer = m_resources.getCommandBuffer().getCommandBuffer();

		if ( commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) )
		{
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
				, m_texture->makeColourAttachment() );
			auto res = vk::checkError( commandBuffer.end() );

			if ( !res )
			{
				throw std::runtime_error{ "Texture binding failed: " + vk::getLastError() };
			}

			res = vk::checkError( device.getGraphicsQueue().submit( { commandBuffer }
				, {}
				, {}
				, {}
			, nullptr ) );

			if ( !res )
			{
				throw std::runtime_error{ "Texture binding failed: " + vk::getLastError() };
			}

			device.waitIdle();
		}
	}
	ImageMemoryBarrier Texture::makeTransferDestination()const
	{
		return ImageMemoryBarrier{ m_texture->makeTransferDestination() };
	}

	ImageMemoryBarrier Texture::makeShaderInputResource()const
	{
		return ImageMemoryBarrier{ m_texture->makeShaderInputResource() };
	}

	ImageMemoryBarrier Texture::makeColourAttachment()const
	{
		return ImageMemoryBarrier{ m_texture->makeColourAttachment() };
	}

	ImageMemoryBarrier Texture::makeDrawDestination()const
	{
		return ImageMemoryBarrier{ m_texture->makeDrawDestination() };
	}

	ImageMemoryBarrier Texture::makePresentSource()const
	{
		return ImageMemoryBarrier{ m_texture->makePresentSource() };
	}
}
