/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Sync/GlFence.hpp"

#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	enum GlFenceWaitResult
	{
		GL_WAIT_RESULT_ALREADY_SIGNALED = 0x911A,
		GL_WAIT_RESULT_CONDITION_SATISFIED = 0x911C,
		GL_WAIT_RESULT_TIMEOUT_EXPIRED = 0x911B,
	};

	Fence::Fence( Device const & device
		, renderer::FenceCreateFlags flags )
		: renderer::Fence{ device, flags }
		, m_device{ device }
	{
		auto context = m_device.getContext();
		m_fence = glLogCall( context
			, glFenceSync
			, GL_WAIT_FLAG_SYNC_GPU_COMMANDS_COMPLETE
			, 0u );
	}

	Fence::~Fence()
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glDeleteSync
			, m_fence );
	}

	renderer::WaitResult Fence::wait( uint64_t timeout )const
	{
		auto context = m_device.getContext();
		auto res = glLogCall( context
			, glClientWaitSync
			, m_fence
			, GL_WAIT_FLAG_SYNC_FLUSH_COMMANDS_BIT
			, timeout );
		return ( res == GL_WAIT_RESULT_ALREADY_SIGNALED || res == GL_WAIT_RESULT_CONDITION_SATISFIED )
			? renderer::WaitResult::eSuccess
			: ( res == GL_WAIT_RESULT_TIMEOUT_EXPIRED
				? renderer::WaitResult::eTimeOut
				: renderer::WaitResult::eError );
	}

	void Fence::reset()const
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glDeleteSync
			, m_fence );
		m_fence = glLogCall( context
			, glFenceSync
			, GL_WAIT_FLAG_SYNC_GPU_COMMANDS_COMPLETE
			, 0u );
	}
}
