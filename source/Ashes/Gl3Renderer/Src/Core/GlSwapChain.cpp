#include "Core/GlSwapChain.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Core/GlBackBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Sync/GlSemaphore.hpp"
#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"

#include <Core/RenderingResources.hpp>

#include <iostream>

namespace gl_renderer
{
	SwapChain::SwapChain( Device const & device
		, ashes::Extent2D const & size )
		: ashes::SwapChain{ device, size }
		, m_device{ device }
	{
		m_format = ashes::Format::eR8G8B8A8_UNORM;
		m_renderingResources.emplace_back( std::make_unique< ashes::RenderingResources >( device ) );
		doCreateBackBuffers();
	}

	void SwapChain::reset( ashes::Extent2D const & size )
	{
		m_dimensions = size;
		doResetSwapChain();
	}

	ashes::FrameBufferPtrArray SwapChain::createFrameBuffers( ashes::RenderPass const & renderPass )const
	{
		ashes::FrameBufferPtrArray result;
		result.emplace_back( std::make_unique< FrameBuffer >( m_device
			, static_cast< RenderPass const & >( renderPass )
			, m_dimensions ) );
		return result;
	}

	ashes::CommandBufferPtrArray SwapChain::createCommandBuffers()const
	{
		ashes::CommandBufferPtrArray result;
		result.emplace_back( std::make_unique< CommandBuffer >( m_device
			, m_device.getGraphicsCommandPool()
			, true ) );
		return result;
	}

	ashes::RenderingResources * SwapChain::getResources()
	{
		auto & resources = *m_renderingResources[m_resourceIndex];

		if ( resources.waitRecord( ashes::FenceTimeout ) )
		{
			uint32_t backBuffer{ 0u };
			resources.setBackBuffer( backBuffer );
			return &resources;
		}

		ashes::Logger::logError( "Can't render" );
		return nullptr;
	}

	void SwapChain::present( ashes::RenderingResources & resources )
	{
		static_cast< Device const & >( m_device ).swapBuffers();
		resources.setBackBuffer( ~0u );
	}

	void SwapChain::createDepthStencil( ashes::Format format )
	{
		auto texture = std::make_unique< Texture >( m_device
			, format
			, m_dimensions );
		m_depthStencilView = std::make_unique< TextureView >( m_device
			, *texture );
		m_depthStencil = std::move( texture );
	}

	void SwapChain::doResetSwapChain()
	{
		m_device.waitIdle();
		doCreateBackBuffers();
		onReset();
	}

	void SwapChain::doCreateBackBuffers()
	{
		m_backBuffers.clear();
		auto texture = std::make_unique< Texture >( m_device
			, m_format
			, m_dimensions );
		auto view = std::make_unique< TextureView >( m_device
			, *texture );
		m_backBuffers.emplace_back( std::make_unique< BackBuffer >( m_device
			, std::move( texture )
			, std::move( view )
			, 0u ) );
	}
}
