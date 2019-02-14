#include "Core/D3D11SwapChain.hpp"

#include "Command/D3D11CommandBuffer.hpp"
#include "Command/D3D11CommandPool.hpp"
#include "Command/D3D11Queue.hpp"
#include "Core/D3D11BackBuffer.hpp"
#include "Core/D3D11Device.hpp"
#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11Renderer.hpp"
#include "Image/D3D11Texture.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"
#include "RenderPass/D3D11RenderPass.hpp"
#include "Sync/D3D11Semaphore.hpp"

#include <Ashes/Core/PlatformWindowHandle.hpp>
#include <Ashes/Core/RenderingResources.hpp>
#include <Ashes/Miscellaneous/MemoryRequirements.hpp>
#include <Ashes/RenderPass/FrameBufferAttachment.hpp>
#include <Ashes/Sync/ImageMemoryBarrier.hpp>

namespace d3d11_renderer
{
	SwapChain::SwapChain( Device const & device
		, ashes::Extent2D const & size )
		: ashes::SwapChain{ device, size }
		, m_device{ device }
		, m_format{ ashes::Format::eR8G8B8A8_UNORM }
	{
		doInitPresentParameters();
		doCreateSwapChain();
		doCreateBackBuffers();
		m_renderingResources.resize( 1 );

		for ( auto & resource : m_renderingResources )
		{
			resource = std::make_unique< ashes::RenderingResources >( device );
		}
	}

	SwapChain::~SwapChain()
	{
		m_backBuffers.clear();
		safeRelease( m_swapChain );
	}

	void SwapChain::reset( ashes::Extent2D const & size )
	{
		m_dimensions = size;
		doResetSwapChain();
	}

	void SwapChain::createDepthStencil( ashes::Format format )
	{
		m_depthStencil = m_device.createTexture(
			{
				0u,
				ashes::TextureType::e2D,
				format,
				ashes::Extent3D{ getDimensions().width, getDimensions().height, 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eDepthStencilAttachment,
				ashes::SharingMode::eExclusive,
				{},
				ashes::ImageLayout::eUndefined,
			} );
		m_depthStencil->bindMemory( m_device.allocateMemory( m_depthStencil->getMemoryRequirements()
			, ashes::MemoryPropertyFlag::eDeviceLocal ) );
		m_depthStencilView = m_depthStencil->createView( ashes::TextureViewType::e2D
			, format );
	}

	ashes::FrameBufferAttachmentArray SwapChain::doPrepareAttaches( uint32_t backBuffer
		, ashes::AttachmentDescriptionArray const & attaches )const
	{
		ashes::FrameBufferAttachmentArray result;

		for ( auto & attach : attaches )
		{
			if ( !ashes::isDepthOrStencilFormat( attach.format ) )
			{
				result.emplace_back( attach, m_backBuffers[backBuffer]->getView() );
			}
			else
			{
				assert( m_depthStencilView );
				result.emplace_back( attach, *m_depthStencilView );
			}
		}

		return result;
	}

	void SwapChain::doResetSwapChain()
	{
		m_device.waitIdle();
		auto colour = m_clearColour;
		m_backBuffers.clear();
		safeRelease( m_swapChain );
		m_renderingResources.clear();
		doInitPresentParameters();
		doCreateSwapChain();
		doCreateBackBuffers();

		m_renderingResources.resize( 1 );

		for ( auto & resource : m_renderingResources )
		{
			resource = std::make_unique< ashes::RenderingResources >( m_device );
		}

		if ( m_depthStencil )
		{
			createDepthStencil( m_depthStencil->getFormat() );
		}

		onReset();
	}

	ashes::FrameBufferPtrArray SwapChain::createFrameBuffers( ashes::RenderPass const & renderPass )const
	{
		ashes::FrameBufferPtrArray result;
		result.resize( m_backBuffers.size() );

		for ( size_t i = 0u; i < result.size(); ++i )
		{
			auto attaches = doPrepareAttaches( uint32_t( i ), renderPass.getAttachments() );
			result[i] = static_cast< RenderPass const & >( renderPass ).createFrameBuffer( { m_dimensions.width, m_dimensions.height }
				, std::move( attaches ) );
		}

		return result;
	}

	ashes::CommandBufferPtrArray SwapChain::createCommandBuffers()const
	{
		ashes::CommandBufferPtrArray result;
		result.resize( m_backBuffers.size() );

		for ( auto & commandBuffer : result )
		{
			commandBuffer = std::make_unique< CommandBuffer >( m_device
				, static_cast< CommandPool const & >( m_device.getGraphicsCommandPool() )
				, true );
		}

		return result;
	}

	ashes::RenderingResources * SwapChain::getResources()
	{
		auto & resources = *m_renderingResources[m_resourceIndex];
		m_resourceIndex = ( m_resourceIndex + 1 ) % m_renderingResources.size();

		if ( resources.waitRecord( ashes::FenceTimeout ) )
		{
			resources.setBackBuffer( 0u );
			return &resources;
		}

		ashes::Logger::logError( "Couldn't retrieve rendering resources" );
		return nullptr;
	}

	void SwapChain::present( ashes::RenderingResources & resources )
	{
		resources.setBackBuffer( ~0u );
		m_swapChain->Present( 0u, 0u );
	}

	void SwapChain::doInitPresentParameters()
	{
		auto const format = DXGI_FORMAT_R8G8B8A8_UNORM;
		auto hWnd = m_device.getConnection().getHandle().getInternal< ashes::IMswWindowHandle >().getHwnd();
		RECT rect{};
		::GetWindowRect( hWnd, &rect );

		UINT numModes;
		auto adapterOutput = m_device.getGpu().getOutput();
		// Get the number of modes that fit the display format for the adapter output (monitor).
		auto hr = adapterOutput->GetDisplayModeList( format
			, DXGI_ENUM_MODES_INTERLACED
			, &numModes
			, nullptr );

		if ( !dxCheckError( hr, "GetDisplayModeList" ) )
		{
			throw std::runtime_error( "GetDisplayModeList(numModes) failed" );
		}

		// Create a list to hold all the possible display modes for this monitor/video card combination, and fill it.
		std::vector< DXGI_MODE_DESC > displayModeList( numModes );
		hr = adapterOutput->GetDisplayModeList( format
			, DXGI_ENUM_MODES_INTERLACED
			, &numModes
			, displayModeList.data() );

		if ( !dxCheckError( hr, "GetDisplayModeList" ) )
		{
			throw std::runtime_error( "GetDisplayModeList(displayModeList) failed" );
		}

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		std::vector< DXGI_MODE_DESC > matchingDisplayModes;
		auto const width = UINT( rect.right - rect.left );
		auto const height = UINT( rect.bottom - rect.top );

		for ( UINT i = 0; i < numModes; i++ )
		{
			if ( displayModeList[i].Width == width
				&& displayModeList[i].Height == height )
			{
				matchingDisplayModes.push_back( displayModeList[i] );
			}
		}

		if ( matchingDisplayModes.empty() )
		{
			// No display mode match exactly the requested size.
			for ( UINT i = 0; i < numModes; i++ )
			{
				if ( displayModeList[i].Width >= width
					&& displayModeList[i].Height >= height )
				{
					matchingDisplayModes.push_back( displayModeList[i] );
				}
			}
		}

		if ( matchingDisplayModes.empty() )
		{
			// Size is really too large.
			DXGI_MODE_DESC maxWidth{};
			DXGI_MODE_DESC maxHeight{};

			for ( UINT i = 0; i < numModes; i++ )
			{
				if ( displayModeList[i].Width >= maxWidth.Width )
				{
					maxWidth = displayModeList[i];
				}

				if ( displayModeList[i].Height >= maxHeight.Height )
				{
					maxHeight = displayModeList[i];
				}
			}

			if ( width > height )
			{
				// Choose the display mode with highest width.
				matchingDisplayModes.push_back( maxWidth );
			}
			else
			{
				// Choose the display mode with highest height.
				matchingDisplayModes.push_back( maxHeight );
			}
		}

		auto & displayMode = matchingDisplayModes.back();

		// Initialize the swap chain description.
		m_desc = DXGI_SWAP_CHAIN_DESC{ 0 };

		// Set to a single back buffer.
		m_desc.BufferCount = 1;

		// Set the width and height of the back buffer.
		m_desc.BufferDesc.Width = width;
		m_desc.BufferDesc.Height = height;

		// Set regular 32-bit surface for the back buffer.
		m_desc.BufferDesc.Format = displayMode.Format;

		// Set the refresh rate of the back buffer.
		m_desc.BufferDesc.RefreshRate.Numerator = displayMode.RefreshRate.Numerator;
		m_desc.BufferDesc.RefreshRate.Denominator = displayMode.RefreshRate.Denominator;

		// Set the usage of the back buffer.
		m_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		m_desc.OutputWindow = hWnd;

		// Turn multisampling off.
		m_desc.SampleDesc.Count = 1;
		m_desc.SampleDesc.Quality = 0;

		// Set to windowed mode.
		m_desc.Windowed = TRUE;

		// Set the scan line ordering and scaling to unspecified.
		m_desc.BufferDesc.ScanlineOrdering = displayMode.ScanlineOrdering;
		m_desc.BufferDesc.Scaling = displayMode.Scaling;

		// Discard the back buffer contents after presenting.
		m_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
		m_desc.Flags = 0;

		m_dimensions.width = width;
		m_dimensions.height = height;
	}

	void SwapChain::doCreateSwapChain()
	{
		auto factory = m_device.getRenderer().getDXGIFactory();
		auto device = m_device.getDevice();
		HRESULT hr = factory->CreateSwapChain( device
			, &m_desc
			, &m_swapChain );

		if ( !dxCheckError( hr, "CreateSwapChain" ) )
		{
			throw std::runtime_error{ "Could not create the swapchain" };
		}

		dxDebugName( m_swapChain, SwapChain );
	}

	void SwapChain::doCreateBackBuffers()
	{
		ID3D11Texture2D * dsTex = nullptr;
		ID3D11Texture2D * rtTex = nullptr;
		auto hr = m_swapChain->GetBuffer( 0
			, __uuidof( ID3D11Texture2D )
			, reinterpret_cast< void ** >( &rtTex ) );

		if ( !dxCheckError( hr, "SwapChain::GetBuffer" ) )
		{
			throw std::runtime_error( "GetBuffer() failed" );
		}

		// Et on crée des BackBuffers à partir de ces images.
		m_backBuffers.reserve( 1u );
		auto texture = std::make_unique< Texture >( m_device
			, m_format
			, m_dimensions
			, rtTex );
		auto & ref = *texture;
		m_backBuffers.emplace_back( std::make_unique< BackBuffer >( m_device
			, std::move( texture )
			, 0u
			, m_format
			, ref ) );
	}
}
