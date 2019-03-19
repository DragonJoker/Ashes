#include "Core/GlSwapChain.hpp"

#include "Core/GlBackBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"

namespace gl_renderer
{
	SwapChain::SwapChain( Device const & device
		, ashes::SwapChainCreateInfo createInfo )
		: ashes::SwapChain{ device, std::move( createInfo ) }
		, m_device{ device }
	{
		doCreateBackBuffers();
	}

	ashes::Result SwapChain::acquireNextImage( uint64_t timeout
		, ashes::Semaphore const * semaphore
		, ashes::Fence const * fence
		, uint32_t & imageIndex )const
	{
		imageIndex = 0u;
		return ashes::Result::eSuccess;
	}

	void SwapChain::createDepthStencil( ashes::Format format )
	{
		auto texture = std::make_unique< Texture >( m_device
			, format
			, getDimensions() );
		m_depthStencilView = std::make_unique< TextureView >( m_device
			, *texture );
		m_depthStencil = std::move( texture );
	}

	void SwapChain::doCreateBackBuffers()
	{
		m_backBuffers.clear();
		auto texture = std::make_unique< Texture >( m_device
			, getFormat()
			, getDimensions() );
		auto view = std::make_unique< TextureView >( m_device
			, *texture );
		m_backBuffers.emplace_back( std::make_unique< BackBuffer >( m_device
			, std::move( texture )
			, std::move( view )
			, 0u ) );
	}
}
