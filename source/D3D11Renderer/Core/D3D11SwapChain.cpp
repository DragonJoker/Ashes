#include "Core/D3D11SwapChain.hpp"

#include "Core/D3D11Device.hpp"
#include "Core/D3D11Instance.hpp"
#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11Surface.hpp"
#include "Image/D3D11Image.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"
#include "RenderPass/D3D11RenderPass.hpp"
#include "Sync/D3D11Semaphore.hpp"

#include <Ashes/Core/PlatformWindowHandle.hpp>
#include <Ashes/Miscellaneous/MemoryRequirements.hpp>

namespace d3d11_renderer
{
	SwapChain::SwapChain( Device const & device
		, ashes::SwapChainCreateInfo createInfo )
		: ashes::SwapChain{ device, std::move( createInfo ) }
		, m_device{ device }
		, m_surface{ static_cast< Surface const & >( m_createInfo.surface.get() ) }
	{
		auto desc = doInitPresentParameters();
		auto factory = m_device.getInstance().getDXGIFactory();
		auto d3ddevice = m_device.getDevice();
		HRESULT hr = factory->CreateSwapChain( d3ddevice
			, &desc
			, &m_swapChain );

		if ( !checkError( m_device, hr, "CreateSwapChain" ) )
		{
			throw std::runtime_error{ "Could not create the swapchain" };
		}

		dxDebugName( m_swapChain, SwapChain );
	}

	SwapChain::~SwapChain()
	{
		safeRelease( m_swapChain );
	}

	ashes::ImagePtrArray SwapChain::getImages()const
	{
		ID3D11Texture2D * rtTex = nullptr;
		auto hr = m_swapChain->GetBuffer( 0
			, __uuidof( ID3D11Texture2D )
			, reinterpret_cast< void ** >( &rtTex ) );

		if ( !checkError( m_device, hr, "SwapChain::GetBuffer" ) )
		{
			throw std::runtime_error( "GetBuffer() failed" );
		}

		// Et on crée des BackBuffers à partir de ces images.
		ashes::ImagePtrArray result;
		result.emplace_back( std::make_unique< Image >( m_device
			, getFormat()
			, getDimensions()
			, rtTex ) );
		return result;
	}

	ashes::Result SwapChain::acquireNextImage( uint64_t timeout
		, ashes::Semaphore const * semaphore
		, ashes::Fence const * fence
		, uint32_t & imageIndex )const
	{
		imageIndex = 0u;
		return ashes::Result::eSuccess;
	}

	DXGI_SWAP_CHAIN_DESC SwapChain::doInitPresentParameters()
	{
		auto caps = m_surface.getCapabilities();
		auto & descs = m_surface.getDescs( m_createInfo.imageFormat );
		assert( !descs.empty() );
		auto hWnd = m_surface.getHandle().getInternal< ashes::IMswWindowHandle >().getHwnd();

		auto & displayMode = descs.back();

		// Initialize the swap chain description.
		auto result = DXGI_SWAP_CHAIN_DESC{ 0 };

		// Set to a single back buffer.
		result.BufferCount = 1;

		// Set the width and height of the back buffer.
		result.BufferDesc.Width = caps.currentExtent.width;
		result.BufferDesc.Height = caps.currentExtent.height;

		// Set regular 32-bit surface for the back buffer.
		result.BufferDesc.Format = displayMode.Format;

		// Set the refresh rate of the back buffer.
		result.BufferDesc.RefreshRate.Numerator = displayMode.RefreshRate.Numerator;
		result.BufferDesc.RefreshRate.Denominator = displayMode.RefreshRate.Denominator;

		// Set the usage of the back buffer.
		result.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		result.OutputWindow = hWnd;

		// Turn multisampling off.
		result.SampleDesc.Count = 1;
		result.SampleDesc.Quality = 0;

		// Set to windowed mode.
		result.Windowed = TRUE;

		// Set the scan line ordering and scaling to unspecified.
		result.BufferDesc.ScanlineOrdering = displayMode.ScanlineOrdering;
		result.BufferDesc.Scaling = displayMode.Scaling;

		// Discard the back buffer contents after presenting.
		result.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
		result.Flags = 0;

		return result;
	}
}
