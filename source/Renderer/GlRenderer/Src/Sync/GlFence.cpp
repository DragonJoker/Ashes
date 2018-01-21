/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "Sync/GlFence.hpp"

#include "Core/GlDevice.hpp"

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
			glLogCall( glDeleteSync, m_fence );
		}
	}

	renderer::WaitResult Fence::wait( uint32_t timeout )const
	{
		if ( !m_fence )
		{
			m_fence = glLogCall( glFenceSync, GL_SYNC_GPU_COMMANDS_COMPLETE, 0u );
		}

		auto res = glLogCall( glClientWaitSync, m_fence, GL_SYNC_FLUSH_COMMANDS_BIT, timeout );
		return ( res == GL_ALREADY_SIGNALED || res == GL_CONDITION_SATISFIED )
			? renderer::WaitResult::eSuccess
			: ( res == GL_TIMEOUT_EXPIRED
				? renderer::WaitResult::eTimeOut
				: renderer::WaitResult::eError );
	}

	void Fence::reset()const
	{
		if ( m_fence )
		{
			glLogCall( glDeleteSync, m_fence );
			m_fence = nullptr;
		}
	}
}
