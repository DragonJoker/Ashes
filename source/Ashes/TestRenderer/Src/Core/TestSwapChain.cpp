#include "Core/TestSwapChain.hpp"

#include "Command/TestCommandBuffer.hpp"
#include "Command/TestCommandPool.hpp"
#include "Command/TestQueue.hpp"
#include "Core/TestBackBuffer.hpp"
#include "Core/TestDevice.hpp"
#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestRenderer.hpp"
#include "Image/TestTexture.hpp"
#include "RenderPass/TestFrameBuffer.hpp"
#include "RenderPass/TestRenderPass.hpp"
#include "Sync/TestSemaphore.hpp"

#include <Core/RenderingResources.hpp>
#include <Miscellaneous/MemoryRequirements.hpp>
#include <RenderPass/FrameBufferAttachment.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

namespace test_renderer
{
	SwapChain::SwapChain( Device const & device
		, ashes::Extent2D const & size )
		: ashes::SwapChain{ device, size }
		, m_device{ device }
		, m_format{ ashes::Format::eR8G8B8A8_UNORM }
	{
		// Puis les tampons d'images.
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
	}

	void SwapChain::reset( ashes::Extent2D const & size )
	{
		m_dimensions = size;
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

		ashes::Logger::logError( "Can't render" );
		return nullptr;
	}

	void SwapChain::present( ashes::RenderingResources & resources )
	{
		resources.setBackBuffer( ~0u );
	}

	void SwapChain::doCreateBackBuffers()
	{
		// Et on crée des BackBuffers à partir de ces images.
		m_backBuffers.reserve( 1u );
		auto texture = std::make_unique< Texture >( m_device
			, m_format
			, m_dimensions );
		auto & ref = *texture;
		m_backBuffers.emplace_back( std::make_unique< BackBuffer >( m_device
			, std::move( texture )
			, 0u
			, m_format
			, ref ) );
	}
}
