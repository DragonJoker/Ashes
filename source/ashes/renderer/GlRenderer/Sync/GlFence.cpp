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
		, bool ignoreFirst )const
	{
		if ( !m_fence && m_firstUse && !ignoreFirst )
		{
			return VK_TIMEOUT;
		}

		if ( !m_fence )
		{
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
		return ( res == GL_WAIT_RESULT_ALREADY_SIGNALED || res == GL_WAIT_RESULT_CONDITION_SATISFIED )
			? VK_SUCCESS
			: ( res == GL_WAIT_RESULT_TIMEOUT_EXPIRED
				? VK_TIMEOUT
				: VK_NOT_READY );
	}

	void Fence::reset( ContextLock & context )const
	{
		glLogCall( context
			, glDeleteSync
			, m_fence );
		m_fence = nullptr;
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
		return value == GL_WAIT_RESULT_UNSIGNALED
			? VK_NOT_READY
			: VK_SUCCESS;
	}

	void Fence::reset()const
	{
		auto context = get( m_device )->getContext();
		reset( context );
	}
}
