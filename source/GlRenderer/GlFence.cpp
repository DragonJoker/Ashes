/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "GlFence.hpp"

#include "GlDevice.hpp"

namespace gl_renderer
{
	Fence::Fence( renderer::Device const & device
		, renderer::FenceCreateFlags flags )
		: renderer::Fence{ device, flags }
	{
	}

	Fence::~Fence()
	{
		if ( m_fence )
		{
			glDeleteSync( m_fence );
		}
	}

	renderer::WaitResult Fence::wait( uint32_t timeout )
	{
		if ( !m_fence )
		{
			m_fence = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0u );
		}

		auto res = glClientWaitSync( m_fence, GL_SYNC_FLUSH_COMMANDS_BIT, timeout );
		return res == VK_SUCCESS
			? renderer::WaitResult::eSuccess
			: ( res == VK_TIMEOUT
				? renderer::WaitResult::eTimeOut
				: renderer::WaitResult::eError );
	}

	void Fence::reset()
	{
		if ( m_fence )
		{
			glDeleteSync( m_fence );
			m_fence = nullptr;
		}
	}
}
