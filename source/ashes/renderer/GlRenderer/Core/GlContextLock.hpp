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

		inline ContextLock( Context & context )
			: m_context{ &context }
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

	private:
		Context * m_context;
		bool m_disable;
	};

	inline bool isGl4( ContextLock const & lock )
	{
		return isGl4( lock.getContext() );
	}
}
