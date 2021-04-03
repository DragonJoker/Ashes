/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/GlFence.hpp"

#include "Core/GlDevice.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	Fence::Fence( VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, VkFenceCreateFlags flags )
		: m_device{ device }
		, m_signaled{ checkFlag( flags, VK_FENCE_CREATE_SIGNALED_BIT ) }
	{
		registerObject( m_device, *this );
	}

	Fence::~Fence()
	{
		unregisterObject( m_device, *this );

		if ( m_fence )
		{
			auto context = get( m_device )->getContext();
			glLogCall( context
				, glDeleteSync
				, m_fence );
		}
	}

	void Fence::insert( ContextLock & context )
	{
		if ( !m_fence )
		{
			m_firstUse = false;
			m_fence = glLogNonVoidCall( context
				, glFenceSync
				, GL_WAIT_FLAG_SYNC_GPU_COMMANDS_COMPLETE
				, 0u );
		}
	}

	VkResult Fence::wait( ContextLock & context
		, uint64_t timeout
		, bool forceWait
		, bool ignoreFirst )
	{
		if ( !m_fence )
		{
			if ( m_signaled )
			{
				return VK_SUCCESS;
			}

			if ( m_firstUse && !ignoreFirst )
			{
				return VK_TIMEOUT;
			}

			if ( forceWait )
			{
				std::this_thread::sleep_for( std::chrono::milliseconds( timeout ) );
			}

			return VK_TIMEOUT;
		}

		glLogEmptyCall( context
			, glFlush );
		auto res = glLogNonVoidCall( context
			, glClientWaitSync
			, m_fence
			, GL_WAIT_FLAG_SYNC_FLUSH_COMMANDS_BIT
			, timeout );
		m_signaled = ( res == GL_WAIT_RESULT_ALREADY_SIGNALED || res == GL_WAIT_RESULT_CONDITION_SATISFIED );
		return m_signaled
			? VK_SUCCESS
			: ( res == GL_WAIT_RESULT_TIMEOUT_EXPIRED
				? VK_TIMEOUT
				: VK_NOT_READY );
	}

	void Fence::reset( ContextLock & context )
	{
		glLogCall( context
			, glDeleteSync
			, m_fence );
		m_fence = nullptr;
		m_signaled = false;
	}

	VkResult Fence::getStatus( ContextLock & context )
	{
		m_firstUse = false;

		if ( !m_fence )
		{
			return VK_NOT_READY;
		}

		GLint value;
		GLsizei size;
		glLogCall( context
			, glGetSynciv
			, m_fence
			, GL_WAIT_RESULT_SYNC_STATUS
			, GLsizei( sizeof( value ) )
			, &size
			, &value );
		m_signaled = value != GL_WAIT_RESULT_UNSIGNALED;
		return m_signaled
			? VK_SUCCESS
			: VK_NOT_READY;
	}
}
