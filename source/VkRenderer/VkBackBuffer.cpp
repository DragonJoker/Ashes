/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkBackBuffer.hpp"

#include "VkSwapChain.hpp"
#include "VkTexture.hpp"

namespace vk_renderer
{
	BackBuffer::BackBuffer( renderer::Device const & device
		, renderer::SwapChain const & swapChain
		, uint32_t imageIndex )
		: renderer::BackBuffer{ device, swapChain, imageIndex }
		, m_backBuffer{ *static_cast< SwapChain const & >( swapChain ).getSwapChain().getBackBuffers()[imageIndex] }
		, m_texture{ device, m_backBuffer.getImage() }
	{
	}
}
