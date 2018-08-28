#include "Core/GlContextSelector.hpp"

#include "Core/GlContext.hpp"

#include <algorithm>

namespace gl_renderer
{
	void ContextSelector::registerContext( Context const & context )const
	{
		std::unique_lock< std::mutex > lock{ m_contextsMtx };
		assert( std::find( m_contexts.begin(), m_contexts.end(), &context ) == m_contexts.end() );
		m_contexts.emplace_back( &context );

	}

	void ContextSelector::unregisterContext( Context const & context )const
	{
		std::unique_lock< std::mutex > lock{ m_contextsMtx };
		auto it = std::find( m_contexts.begin(), m_contexts.end(), &context );
		assert( it != m_contexts.end() );
		m_contexts.erase( it );
		auto current = getCurrentContext();

		if ( current == &context )
		{
			m_currentContext.store( nullptr );
			current->endCurrent();
		}
	}

	void ContextSelector::enableContextForCurrentThread()const
	{
		auto const id = std::this_thread::get_id();
		auto current = getCurrentContext();

		if ( !current
			|| id != current->getThreadId() )
		{
#if !defined( NDEBUG )
			{
				std::unique_lock< std::mutex > lock{ m_contextsMtx };
				auto it = std::find_if( m_contexts.begin()
					, m_contexts.end()
					, [this, id]( Context const * lookup )
					{
						return id == lookup->getThreadId();
					} );;

				if ( it == m_contexts.end() )
				{
					throw std::runtime_error{ "No context created for current thread." };
				}
			}
#endif
			setCurrentContext( id );
		}
	}

	Context const * ContextSelector::getCurrentContext()const
	{
		return m_currentContext.load();
	}

	void ContextSelector::setCurrentContext( std::thread::id id )const
	{
		auto current = getCurrentContext();

		if ( current )
		{
			current->endCurrent();
		}

		current = getContext( id );
		current->setCurrent();
		m_currentContext.store( current );
	}

	Context const * ContextSelector::getContext( std::thread::id id )const
	{
		std::unique_lock< std::mutex > lock{ m_contextsMtx };
		auto it = std::find_if( m_contexts.begin()
			, m_contexts.end()
			, [this, id]( Context const * lookup )
			{
				return id == lookup->getThreadId();
			} );
		assert( it != m_contexts.end() );
		return *it;
	}
}
