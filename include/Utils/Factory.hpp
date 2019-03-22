/*
See LICENSE file in root folder
*/
#ifndef ___Utils_Factory_H___
#define ___Utils_Factory_H___
#pragma once

#include "Utils/UtilsPlugin.hpp"

#include <type_traits>
#include <functional>
#include <iostream>

namespace utils
{
	/**
	*\~english
	*\brief
	*	Frabric concept implementation.
	*\~french
	*\brief
	*	Implémentation du concept de fabrique.
	*/
	class InstanceFactory
	{
	protected:
		using Key = std::string;
		using PluginMap = std::map< Key, Plugin const * >;

	public:
		/**
		*\~english
		*\brief
		*	Registers an object type.
		*\param[in] key
		*	The object type.
		*\param[in] creator
		*	The object creation function.
		*\~french
		*\brief
		*	Enregistre un type d'objet.
		*\param[in] key
		*	Le type d'objet.
		*\param[in] creator
		*	La fonction de création d'objet.
		*/
		inline void registerType( Key const & key, Plugin const * plugin )
		{
			m_registered.emplace( key, plugin );
		}
		/**
		*\~english
		*\brief
		*	Unregisters an object type.
		*\param[in]key
		*	The object type.
		*\~french
		*\brief
		*	Désenregistre un type d'objet.
		*\param[in] key
		*	Le type d'objet.
		*/
		inline void unregisterType( Key const & key )
		{
			auto it = m_registered.find( key );

			if ( it != m_registered.end() )
			{
				m_registered.erase( key );
			}
		}
		/**
		*\~english
		*\brief
		*	Checks if the given object type is registered.
		*\param[in] key
		*	The object type.
		*\return
		*	\p true if registered.
		*\~french
		*\brief
		*	Vérifie si un type d'objet est enregistré.
		*\param[in] key
		*	Le type d'objet.
		*\return
		*	\p true si enregistré.
		*/
		inline bool isTypeRegistered( Key const & key )
		{
			return m_registered.find( key ) != m_registered.end();
		}
		/**
		*\~english
		*\return
		*	Retrieves the registered types list.
		*\~french
		*\return
		*	La liste des types enregistrés.
		*/
		inline std::vector< Key > listRegisteredTypes()
		{
			std::vector< Key > result;
			result.reserve( m_registered.size() );

			for ( auto const & it : m_registered )
			{
				result.push_back( it.first );
			}

			return result;
		}
		Plugin const * findPlugin( Key const & key )const
		{
			Plugin const * result{ nullptr };
			auto it = m_registered.find( key );

			if ( it != m_registered.end() )
			{
				result = it->second;
			}
			else
			{
				static std::string const Error = "Unknown object type";
				std::cerr << Error << std::endl;
				throw std::runtime_error{ Error };
			}

			return result;
		}

	private:
		PluginMap m_registered;
	};
}

#endif
