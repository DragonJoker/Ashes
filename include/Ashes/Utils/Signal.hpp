/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#ifndef ___Ashes_Signal_HPP___
#define ___Ashes_Signal_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

#include <functional>
#include <set>
#include <map>
#include <mutex>

namespace ashes
{
	/**
	*\brief
	*	Représente une connexion à un signal.
	*/
	template< typename SignalT >
	class SignalConnection
	{
	private:
		using my_signal = SignalT;
		using my_signal_ptr = my_signal *;
		SignalConnection( SignalConnection< my_signal > const & ) = delete;
		SignalConnection & operator=( SignalConnection< my_signal > const & ) = delete;

	public:
		/**
		*\brief
		*	Constructeur.
		*/
		SignalConnection()
			: m_connection{ 0u }
			, m_signal{ nullptr }
		{
		}
		/**
		*\brief
		*	Constructeur par déplacement.
		*\param[in,out] rhs
		*	L'objet à déplacer.
		*/
		SignalConnection( SignalConnection< my_signal > && rhs )
			: m_connection{ rhs.m_connection }
			, m_signal{ rhs.m_signal }
		{
			rhs.m_signal = nullptr;
			rhs.m_connection = 0u;
			m_signal->remConnection( rhs );
			m_signal->addConnection( *this );
		}
		/**
		*\brief
		*	Opérateur d'affectation par déplacement.
		*\param[in,out] rhs
		*	L'objet à déplacer.
		*/
		SignalConnection & operator=( SignalConnection< my_signal > && rhs )
		{
			SignalConnection tmp{ std::move( rhs ) };
			swap( *this, tmp );
			m_signal->remConnection( tmp );
			m_signal->addConnection( *this );
			return *this;
		}
		/**
		*\brief
		*	Constructeur.
		*\param[in] connection
		*	La connection au signal.
		*\param[in] signal
		*	Le signal.
		*/
		SignalConnection( uint32_t connection, my_signal & signal )
			: m_connection{ connection }
			, m_signal{ &signal }
		{
			signal.addConnection( *this );
		}
		/**
		*\brief
		*	Destructeur.
		*\remarks
		*	Déconnecte la fonction du signal.
		*/
		~SignalConnection()
		{
			disconnect();
		}
		/**
		*\brief
		*	Déconnecte la fonction du signal.
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
		/**
		*\brief
		*	Echange deux connections.
		*/
		void swap( SignalConnection & lhs, SignalConnection & rhs )
		{
			if ( &rhs != &lhs )
			{
				std::swap( lhs.m_signal, rhs.m_signal );
				std::swap( lhs.m_connection, rhs.m_connection );
			}
		}

	private:
		//! L'identifiant de la connection.
		uint32_t m_connection;
		//! Le signal.
		my_signal_ptr m_signal;
	};
	/**
	*\brief
	*	Classe basique de signal.
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
		*	Destructeur.
		*\remarks
		*	Déconnecte toutes les connexions restantes.
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
		*	Connecte une nouvelle fonction, qui sera appelée lorsque le signal
		*	est émis.
		*\param[in] function
		*	La fonction.
		*\return
		*	L'indice de la fonction, afin de pouvoir la déconnecter.
		*/
		my_connection connect( Function function )
		{
			uint32_t index = uint32_t( m_slots.size() ) + 1u;
			m_slots.emplace( index, function );
			return my_connection{ index, *this };
		}
		/**
		*\brief
		*	Emet le signal, appelant toutes les fonctions connectées.
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
		*	Emet le signal, appelant toutes les fonctions connectées.
		*\param[in] params
		*	Les paramètres des fonctions.
		*/
		template< typename ... Params >
		void operator()( Params && ... params )const
		{
			for ( auto it : m_slots )
			{
				it.second( std::forward< Params >( params )... );
			}
		}

	private:
		/**
		*\brief
		*	Déconnecte une fonction.
		*\param[in] index
		*	L'indice de la fonction.
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
		*	Ajoute une connexion à la liste.
		*\param[in] connection
		*	La connexion à ajouter.
		*/
		void addConnection( my_connection & connection )
		{
			std::unique_lock< std::recursive_mutex > lock( m_mutex );
			m_connections.insert( &connection );
		}
		/**
		*\brief
		*	Enlève une connexion de la liste.
		*\param[in] connection
		*	La connexion à enlever.
		*/
		void remConnection( my_connection & connection )
		{
			std::unique_lock< std::recursive_mutex > lock( m_mutex );
			assert( m_connections.find( &connection ) != m_connections.end() );
			m_connections.erase( &connection );
		}

	private:
		//! La liste des fonctions connectées.
		std::map< uint32_t, Function > m_slots;
		//! Lq liste des connections à ce signal.
		std::set< my_connection_ptr > m_connections;
		std::recursive_mutex m_mutex;
	};
}

#endif
