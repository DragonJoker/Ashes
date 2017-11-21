#include "Texture.hpp"

#include "RenderingResources.hpp"

#include <VkLib/Queue.hpp>

namespace renderer
{
	Texture::Texture( RenderingResources const & resources )
		: m_resources{ resources }
	{
	}

	void Texture::image( PixelFormat format
		, IVec2 const & size
		, ByteArray const & data
		, WrapMode wrapS
		, WrapMode wrapT
		, Filter minFilter
		, Filter magFilter )
	{
		m_format = format;
		m_size = size;
		m_texture = m_resources.getDevice().createImage( convert( m_format )
			, m_size.x
			, m_size.y
			, convert( MemoryPropertyFlag::eDeviceLocal ) );
		m_sampler = m_resources.getDevice().createSampler( convert( minFilter )
			, convert( magFilter )
			, convert( MipmapMode::eNearest )
			, convert( wrapS )
			, convert( wrapT )
			, convert( WrapMode::eRepeat ) );
		m_resources.copyTextureData( data, *m_texture );
	}

	void Texture::generateMipmaps()const noexcept
	{
		//m_texture->bind( 0 );
		//m_texture->generateMipmaps();
		//m_texture->unbind( 0 );
	}

	void Texture::bindAsShaderInput( uint32_t unit )const
	{
		assert( m_texture && m_sampler );
		auto & device = m_resources.getDevice();
		auto & commandBuffer = m_resources.getCommandBuffer();

		if ( commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) )
		{
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
				, m_texture->makeShaderInputResource() );
			auto res = commandBuffer.end();

			if ( !vk::checkError( res ) )
			{
				throw std::runtime_error{ "Texture binding failed: " + vk::getLastError() };
			}

			res = device.getGraphicsQueue().submit( { commandBuffer }
				, {}
				, {}
				, {}
			, nullptr );

			if ( !vk::checkError( res ) )
			{
				throw std::runtime_error{ "Texture binding failed: " + vk::getLastError() };
			}

			device.waitIdle();
		}
	}

	void Texture::bindAsShaderOutput( uint32_t unit )const
	{
		assert( m_texture && m_sampler );
		auto & device = m_resources.getDevice();
		auto & commandBuffer = m_resources.getCommandBuffer();

		if ( commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ) )
		{
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
				, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
				, m_texture->makeColourAttachment() );
			auto res = commandBuffer.end();

			if ( !vk::checkError( res ) )
			{
				throw std::runtime_error{ "Texture binding failed: " + vk::getLastError() };
			}

			res = device.getGraphicsQueue().submit( { commandBuffer }
				, {}
				, {}
				, {}
			, nullptr );

			if ( !vk::checkError( res ) )
			{
				throw std::runtime_error{ "Texture binding failed: " + vk::getLastError() };
			}

			device.waitIdle();
		}
	}
}
