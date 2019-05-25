/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/GlFence.hpp"

#include "Core/GlDevice.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	enum GlFenceWaitResult
	{
		GL_WAIT_RESULT_ALREADY_SIGNALED = 0x911A,
		GL_WAIT_RESULT_CONDITION_SATISFIED = 0x911C,
		GL_WAIT_RESULT_TIMEOUT_EXPIRED = 0x911B,
	};

	Fence::Fence( VkDevice device
		, VkFenceCreateFlags flags )
		: m_device{ device }
	{
		auto context = get( m_device )->getContext();
		m_fence = glLogCall( context
			, glFenceSync
			, GL_WAIT_FLAG_SYNC_GPU_COMMANDS_COMPLETE
			, 0u );
	}

	Fence::~Fence()
	{
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glDeleteSync
			, m_fence );
	}

	VkResult Fence::wait( ContextLock & context
		, uint64_t timeout )const
	{
		glLogCall( context
			, glFlush );
		auto res = glLogCall( context
			, glClientWaitSync
			, m_fence
			, GL_WAIT_FLAG_SYNC_FLUSH_COMMANDS_BIT
			, timeout );
		return ( res == GL_WAIT_RESULT_ALREADY_SIGNALED || res == GL_WAIT_RESULT_CONDITION_SATISFIED )
			? VK_SUCCESS
			: ( res == GL_WAIT_RESULT_TIMEOUT_EXPIRED
				? VK_TIMEOUT
				: VK_NOT_READY );
	}

	VkResult Fence::wait( uint64_t timeout )const
	{
		auto context = get( m_device )->getContext();
		return wait( context, timeout );
	}

	void Fence::reset( ContextLock & context )const
	{
		glLogCall( context
			, glDeleteSync
			, m_fence );
		m_fence = glLogCall( context
			, glFenceSync
			, GL_WAIT_FLAG_SYNC_GPU_COMMANDS_COMPLETE
			, 0u );
	}

	void Fence::reset()const
	{
		auto context = get( m_device )->getContext();
		reset( context );
	}
}
