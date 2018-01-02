#include "GlSwapChain.hpp"

#include "GlCommandBuffer.hpp"
#include "GlDevice.hpp"
#include "GlFrameBuffer.hpp"
#include "GlRenderPass.hpp"
#include "GlSemaphore.hpp"
#include "GlTexture.hpp"

namespace gl_renderer
{
	SwapChain::SwapChain( renderer::Device const & device
		, utils::IVec2 const & size )
		: renderer::SwapChain{ device, size }
	{
		m_renderingResources.resize( 3 );

		for ( auto & resource : m_renderingResources )
		{
			resource = std::make_unique< RenderingResources >( device );
		}
	}

	void SwapChain::reset( utils::IVec2 const & size )
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
		result.emplace_back( std::make_unique< CommandBuffer >( m_device
			, m_device.getGraphicsCommandPool()
			, true ) );
		return result;
	}

	void SwapChain::preRenderCommands( uint32_t index
		, renderer::CommandBuffer const & commandBuffer )const
	{
	}

	void SwapChain::postRenderCommands( uint32_t index
		, renderer::CommandBuffer const & commandBuffer )const
	{
	}

	renderer::RenderingResources * SwapChain::getResources()
	{
		auto & resources = *m_renderingResources[m_resourceIndex];
		m_resourceIndex = ( m_resourceIndex + 1 ) % m_renderingResources.size();

		if ( resources.waitRecord( vk::FenceTimeout ) )
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

	void SwapChain::doResetSwapChain()
	{
		m_device.waitIdle();
		onReset();
	}
}
