/*
See LICENSE file in root folder
*/
#pragma once

#include "GlContext.hpp"

namespace ashes::gl4
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
			if ( &rhs != this )
			{
				m_context = rhs.m_context;
				m_disable = rhs.m_disable;
				rhs.m_context = nullptr;
			}

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
			return *m_context;
		}

		inline void apply( Device const & device
			, ContextState const & state )const
		{
			m_context->apply( *this, device, state );
		}

	private:
		Context * m_context;
		bool m_disable;
	};
}
