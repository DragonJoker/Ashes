/*
See LICENSE file in root folder
*/
#pragma once

#include "GlContext.hpp"

#include <cassert>

namespace ashes::gl
{
	class ContextLock
	{
	public:
		ContextLock( ContextLock const & ) = delete;
		ContextLock & operator=( ContextLock const & ) = delete;

		inline ContextLock( Context & context
			, VkDevice device = VK_NULL_HANDLE )
			: m_context{ &context }
			, m_device{ device }
			, m_disable{ !context.isEnabled() }
		{
			if ( m_disable )
			{
				m_context->lock();
			}
		}

		inline ContextLock( ContextLock && rhs )
			: m_context{ rhs.m_context }
			, m_disable{ rhs.m_disable }
		{
			rhs.m_context = nullptr;
		}

		inline ContextLock & operator=( ContextLock && rhs )
		{
			m_context = rhs.m_context;
			m_disable = rhs.m_disable;
			rhs.m_context = nullptr;
			return *this;
		}

		inline ~ContextLock()
		{
			if ( m_context && m_disable )
			{
				m_context->unlock();
			}
		}

		inline Context * operator->()const
		{
			return m_context;
		}

		inline Context const & getContext()const
		{
			assert( m_context != nullptr );
			return *m_context;
		}

		inline VkDevice getDevice()const
		{
			assert( m_device != nullptr );
			return m_device;
		}

	private:
		Context * m_context;
		VkDevice m_device;
		bool m_disable;
	};
}
