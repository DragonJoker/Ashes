/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "BackBuffer.hpp"

#include "SwapChain.hpp"
#include "Texture.hpp"

namespace renderer
{
	BackBuffer::BackBuffer( Device const & device
		, SwapChain const & swapChain
		, uint32_t imageIndex )
		: m_backBuffer{ *swapChain.getSwapChain().getBackBuffers()[imageIndex] }
		, m_texture{ device, m_backBuffer.getImage() }
	{
	}
}
