#include "Core/D3D11SwapChain.hpp"

#include "Core/D3D11Device.hpp"
#include "Core/D3D11Instance.hpp"
#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11Surface.hpp"
#include "Image/D3D11Image.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"
#include "RenderPass/D3D11RenderPass.hpp"
#include "Sync/D3D11Semaphore.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	SwapchainKHR::SwapchainKHR( VkDevice device
		, VkSwapchainCreateInfoKHR createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		auto desc = doInitPresentParameters();
		auto factory = get( get( m_device )->getInstance() )->getDXGIFactory();
		auto d3ddevice = get( m_device )->getDevice();
		HRESULT hr = factory->CreateSwapChain( d3ddevice
			, &desc
			, &m_swapChain );

		if ( !checkError( m_device, hr, "CreateSwapChain" ) )
		{
			throw std::runtime_error{ "Could not create the swapchain" };
		}

		dxDebugName( m_swapChain, SwapChain );
	}

	SwapchainKHR::~SwapchainKHR()
	{
		safeRelease( m_swapChain );
	}

	uint32_t SwapchainKHR::getImageCount()const
	{
		return 1u;
	}

	VkImageArray SwapchainKHR::getImages()const
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
		VkImageArray result;
		VkImage img;
		allocate( img
			, nullptr
			, m_device
			, m_createInfo.imageFormat
			, m_createInfo.imageExtent
			, rtTex );
		result.emplace_back( img );
		return result;
	}

	VkResult SwapchainKHR::acquireNextImage( uint64_t timeout
		, VkSemaphore semaphore
		, VkFence fence
		, uint32_t & imageIndex )const
	{
		imageIndex = 0u;
		return VK_SUCCESS;
	}

	DXGI_SWAP_CHAIN_DESC SwapchainKHR::doInitPresentParameters()
	{
		auto caps = get( m_createInfo.surface )->getCapabilities();
		auto & descs = get( m_createInfo.surface )->getDescs( m_createInfo.imageFormat );
		assert( !descs.empty() );
		auto hWnd = get( m_createInfo.surface )->getHwnd();

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
