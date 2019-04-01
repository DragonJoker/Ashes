/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Renderer_HPP___
#define ___AshesPP_Renderer_HPP___
#pragma once

#include "AshesPP/Core/Instance.hpp"
#include "AshesPP/Utils/DynamicLibrary.hpp"

namespace ashespp
{
	/**
	*\~english
	*\brief
	*	The class initialising the rendering instance.
	*\~french
	*\brief
	*	Classe initialisant l'instance de rendu.
	*/
	class Renderer
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructor.
		*\~french
		*\brief
		*	Constructeur.
		*/
		Renderer();
		/**
		*\~french
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~Renderer();
		/**
		*\~french
		*\brief
		*	Récupère une fonction d'instance.
		*\~english
		*\brief
		*	Retrieves an instance function.
		*/
		inline PFN_vkVoidFunction getInstanceProcAddr( VkInstance instance
			, char const * const name )const
		{
			return m_plugin.getInstanceProcAddr( instance, name );
		}
		/**
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		//inline std::string getName()const
		//{
		//	return m_plugin.name;
		//}

		//inline std::string getDescription()const
		//{
		//	return m_plugin.description;
		//}

		//inline AshPluginDescription const & getPluginDescription()const
		//{
		//	return m_plugin;
		//}
		/**@}*/

	private:
		DynamicLibrary m_library;
		AshPluginDescription m_plugin;
	};
}

#endif
