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

	void Texture::setImage( utils::PixelFormat format
		, IVec2 const & size
		, ByteArray const & data
		, renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer )
	{
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
		m_texture = m_device.getDevice().createImage( convert( m_format )
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

	void Texture::bindAsShaderInput( CommandBuffer const & cb
		, uint32_t unit )const
	{
		assert( m_texture );
		auto & device = m_device;
		auto & commandBuffer = cb.getCommandBuffer();

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
		assert( m_texture );
		auto & device = m_device;
		auto & commandBuffer = cb.getCommandBuffer();

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
