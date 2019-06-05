/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#ifndef ___Ashes_common_Signal_HPP___
#define ___Ashes_common_Signal_HPP___
#pragma once

#include <cassert>
#include <functional>
#include <set>
#include <map>
#include <mutex>

namespace ashes
{
	/**
	*\brief
	*	A connection to a signal.
	*/
	template< typename SignalT >
	class SignalConnection
	{
	private:
		using my_signal = SignalT;
		using my_signal_ptr = my_signal *;
		SignalConnection( SignalConnection< my_signal > const & ) = delete;
		SignalConnection & operator=( SignalConnection< my_signal > const & ) = delete;

		SignalConnection( uint32_t connection
			, my_signal_ptr signal )
			: m_connection{ connection }
			, m_signal{ signal }
		{
		}

	public:
		/**
		*\name
		*	Construction/Destruction.
		*/
		/**\{*/
		SignalConnection()
			: SignalConnection{ 0u, nullptr }
		{
		}

		SignalConnection( SignalConnection< my_signal > && rhs )
			: SignalConnection{ 0u, nullptr }
		{
			swap( *this, rhs );
		}

		SignalConnection( uint32_t connection, my_signal & signal )
			: SignalConnection{ connection, &signal }
		{
			signal.addConnection( *this );
		}

		SignalConnection & operator=( SignalConnection< my_signal > && rhs )
		{
			SignalConnection tmp{ std::move( rhs ) };
			swap( *this, tmp );
			return *this;
		}

		~SignalConnection()
		{
			disconnect();
		}
		/**\}*/
		/**
		*\brief
		*	Disconnects from the signal.
		*/
		void disconnect()
		{
			if ( m_signal && m_connection )
			{
				m_signal->disconnect( m_connection );
				m_signal->remConnection( *this );
				m_signal = nullptr;
				m_connection = 0u;
			}
		}

	private:
		void swap( SignalConnection & lhs, SignalConnection & rhs )
		{
			if ( lhs.m_signal )
			{
				lhs.m_signal->remConnection( lhs );
			}

			if ( rhs.m_signal )
			{
				rhs.m_signal->remConnection( rhs );
			}

			std::swap( lhs.m_signal, rhs.m_signal );
			std::swap( lhs.m_connection, rhs.m_connection );

			if ( lhs.m_signal )
			{
				lhs.m_signal->addConnection( lhs );
			}

			if ( rhs.m_signal )
			{
				rhs.m_signal->addConnection( rhs );
			}
		}

	private:
		uint32_t m_connection;
		my_signal_ptr m_signal;
	};
	/**
	*\brief
	*	Signal implementation.
	*/
	template< typename Function >
	class Signal
	{
		friend class SignalConnection< Signal< Function > >;
		using my_connection = SignalConnection< Signal< Function > >;
		using my_connection_ptr = my_connection *;

	public:
		/**
		*\brief
		*	Destructor.
		*\remarks
		*	Disconnects all remaining connections.
		*/
		~Signal()
		{
			// SignalConnection::disconnect appelle Signal::remConnection, qui
			// supprime la connection de m_connections, invalidant ainsi
			// l'itérateur, donc on ne peut pas utiliser un for_each, ni
			// un range for loop.
			std::unique_lock< std::recursive_mutex > lock( m_mutex );
			auto it = m_connections.begin();

			while ( it != m_connections.end() )
			{
				( *it )->disconnect();
				it = m_connections.begin();
			}
		}
		/**
		*\brief
		*	Connects a new function, called when the signal is emitted.
		*\param[in] function
		*	The function.
		*\return
		*	The connection.
		*/
		my_connection connect( Function function )
		{
			uint32_t index = uint32_t( m_slots.size() ) + 1u;
			m_slots.emplace( index, function );
			return my_connection{ index, *this };
		}
		/**
		*\brief
		*	Emits the signal, calling all connected functions.
		*/
		void operator()()const
		{
			for ( auto it : m_slots )
			{
				it.second();
			}
		}
		/**
		*\brief
		*	Emits the signal, calling all connected functions.
		*\param[in] params
		*	The functions parameters.
		*/
		template< typename ... Params >
		void operator()( Params && ... params )const
		{
			auto it = m_slots.begin();
			auto size = m_slots.size();
			auto index = 0u;

			while ( it != m_slots.end() )
			{
				it->second( std::forward< Params >( params )... );

				if ( size != m_slots.size() )
				{
					// Slots changed by the slot itself.
					size = m_slots.size();
					it = m_slots.begin();
					std::advance( it, index );
				}
				else
				{
					++index;
					++it;
				}
			}
		}

	private:
		/**
		*\brief
		*	Disconnects a function.
		*\param[in] index
		*	The function index.
		*/
		void disconnect( uint32_t index )
		{
			auto it = m_slots.find( index );

			if ( it != m_slots.end() )
			{
				m_slots.erase( it );
			}
		}
		/**
		*\brief
		*	Adds a connection to the list.
		*\param[in] connection
		*	The connection to add.
		*/
		void addConnection( my_connection & connection )
		{
			std::unique_lock< std::recursive_mutex > lock( m_mutex );
			m_connections.insert( &connection );
		}
		/**
		*\brief
		*	Removes a connection from the list.
		*\param[in] connection
		*	The connection to remove.
		*/
		void remConnection( my_connection & connection )
		{
			std::unique_lock< std::recursive_mutex > lock( m_mutex );
			assert( m_connections.find( &connection ) != m_connections.end() );
			m_connections.erase( &connection );
		}

	private:
		std::map< uint32_t, Function > m_slots;
		std::set< my_connection_ptr > m_connections;
		std::recursive_mutex m_mutex;
	};
}

#endif
