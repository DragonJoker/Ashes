#include "Core/GlSwapChain.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Core/GlBackBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Sync/GlSemaphore.hpp"
#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"

#include <iostream>

#include <DXGI1_2.h>

namespace gl_renderer
{
	SwapChain::SwapChain( Device const & device
		, renderer::UIVec2 const & size )
		: renderer::SwapChain{ device, size }
		, m_device{ device }
	{
		m_format = renderer::PixelFormat::eR8G8B8A8;
		m_renderingResources.emplace_back( std::make_unique< RenderingResources >( device ) );
		doCreateBackBuffers();
	}

	void SwapChain::reset( renderer::UIVec2 const & size )
	{
		m_dimensions = size;
		doResetSwapChain();
	}

	renderer::FrameBufferPtrArray SwapChain::createFrameBuffers( renderer::RenderPass const & renderPass )const
	{
		return renderer::FrameBufferPtrArray
		{
			std::make_shared< FrameBuffer >( renderPass, m_dimensions )
		};
	}

	renderer::CommandBufferPtrArray SwapChain::createCommandBuffers()const
	{
		renderer::CommandBufferPtrArray result;
		result.emplace_back( std::make_unique< CommandBuffer >( static_cast< Device const & >( m_device )
			, m_device.getGraphicsCommandPool()
			, true ) );
		return result;
	}

	renderer::RenderingResources * SwapChain::getResources()
	{
		auto & resources = *m_renderingResources[m_resourceIndex];

		if ( resources.waitRecord( renderer::FenceTimeout ) )
		{
			uint32_t backBuffer{ 0u };
			resources.setBackBuffer( backBuffer );
			return &resources;
		}

		std::cerr << "Can't render" << std::endl;
		return nullptr;
	}

	void SwapChain::present( renderer::RenderingResources & resources )
	{
		static_cast< Device const & >( m_device ).swapBuffers();
		resources.setBackBuffer( ~0u );
	}

	void SwapChain::createDepthStencil( renderer::PixelFormat format )
	{
		m_depthStencil = std::make_unique< Texture >( m_device
			, format
			, m_dimensions );
		m_depthStencilView = m_depthStencil->createView( renderer::TextureType::e2D
			, format );
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
