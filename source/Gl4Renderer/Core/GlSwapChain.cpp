#include "Core/GlSwapChain.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"

namespace gl_renderer
{
	SwapChain::SwapChain( Device const & device
		, ashes::SwapChainCreateInfo createInfo )
		: ashes::SwapChain{ device, std::move( createInfo ) }
		, m_device{ device }
	{
	}

	ashes::Result SwapChain::acquireNextImage( uint64_t timeout
		, ashes::Semaphore const * semaphore
		, ashes::Fence const * fence
		, uint32_t & imageIndex )const
	{
		imageIndex = 0u;
		return ashes::Result::eSuccess;
	}

	ashes::ImagePtrArray SwapChain::getImages()const
	{
		ashes::ImagePtrArray result;
		result.emplace_back( std::make_unique< Image >( m_device
			, getFormat()
			, getDimensions() ) );
		return result;
	}

	void SwapChain::createDepthStencil( ashes::Format format )
	{
		auto texture = std::make_unique< Image >( m_device
			, format
			, getDimensions() );
		m_depthStencilView = std::make_unique< ImageView >( m_device
			, *texture );
		m_depthStencil = std::move( texture );
	}
}
