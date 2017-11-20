/**
*\file
*	ElementsList.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_ElementsList_HPP___
#define ___RenderLib_ElementsList_HPP___
#pragma once

#include "RenderLibPrerequisites.h"

namespace render
{
	/**
	*\brief
	*	Un wrapper de std::map pour faciliter son utilisation avec des
	*	éléments nommés.
	*\remarks
	*	Si les éléments ne sont pas default constructible, il faut utiliser
	*	un allocateur personnalisé.
	*/
	template< typename T >
	class ElementsList
	{
	private:
		using ElementPtr = std::shared_ptr< T >;
		using ElementMap = std::map< std::string, ElementPtr >;
		using Allocator = std::function< ElementPtr () >;

	public:
		/**
		*\brief
		*	Constructeur.
		*/
		ElementsList()
		{
		}
		/**
		*\brief
		*	Vide la map.
		*/
		void clear()
		{
			m_elements.clear();
		}
		/**
		*\brief
		*	Ajoute un élément déjà créé, s'il n'est pas déjà présent dans
		*	la liste.
		*\param[in] name
		*	Le nom de l'élément.
		*\param[in] element
		*	L'élément à ajouter.
		*/
		inline void addElement( std::string const & name
			, ElementPtr element )
		{
			if ( m_elements.find( name ) == m_elements.end() )
			{
				m_elements.emplace( name, element );
			}
		}
		/**
		*\brief
		*	Supprime un élément.
		*\param[in] name
		*	Le nom de l'élément.
		*/
		inline void removeElement( std::string const & name )
		{
			auto it = m_elements.find( name );

			if ( m_elements.find( name ) != m_elements.end() )
			{
				m_elements.erase( it );
			}
		}
		/**
		*\brief
		*	Récupère un élément depuis la map.
		*\param[in] name
		*	Le nom de l'élément.
		*\return
		*	L'élément récupéré, \p nullptr si non trouvé.
		*/
		inline ElementPtr findElement( std::string const & name )const
		{
			ElementPtr ret;
			auto it = m_elements.find( name );

			if ( it != m_elements.end() )
			{
				ret = it->second;
			}

			return ret;
		}
		/**
		*\return
		*	L'itérateur de début des éléments.
		*/
		inline typename ElementMap::iterator begin()
		{
			return m_elements.begin();
		}
		/**
		*\return
		*	L'itérateur de début des éléments.
		*/
		inline typename ElementMap::const_iterator begin()const
		{
			return m_elements.begin();
		}
		/**
		*\return
		*	L'itérateur de fin des éléments.
		*/
		inline typename ElementMap::iterator end()
		{
			return m_elements.end();
		}
		/**
		*\return
		*	L'itérateur de fin des éléments.
		*/
		inline typename ElementMap::const_iterator end()const
		{
			return m_elements.end();
		}
		/**
		*\return
		*	Le nombre d'éléments.
		*/
		inline typename ElementMap::size_type size()const
		{
			return m_elements.size();
		}

	private:
		//! La liste d'éléments.
		ElementMap m_elements;
	};
}

#endif
