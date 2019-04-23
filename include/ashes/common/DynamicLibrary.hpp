/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_common_DynamicLibrary_HPP___
#define ___Ashes_common_DynamicLibrary_HPP___
#pragma once

#include <string>

namespace ashes
{
	/**
	*\brief
	*	Platform independant shared library (.dll or .so).
	*\remarks
	*	Loads a library and gives access to it's functions in a platform independant way.
	*\~french
	*\brief
	*	Bibliothèque dynamique (Dll, shared lib).
	*\remarks
	*	Charge une bibliothèque et permet l'accès a ses fonction de manière indépendante de l'OS.
	*/
	class DynamicLibrary
	{
	public:
		DynamicLibrary( DynamicLibrary const & ) = delete;
		DynamicLibrary & operator=( DynamicLibrary const & ) = delete;
		DynamicLibrary( DynamicLibrary && rhs );
		DynamicLibrary & operator=( DynamicLibrary && rhs );
		/**
		*\~english
		*\brief
		*	Constructor, ppens a library from a path.
		*\remarks
		*	If the library couldn't be loaded, throws a std::runtime_error.
		*\param[in] path
		*	The file path.
		*\~french
		*\brief
		*	Charge une bibliothèque à partir d'un chemin.
		*\remarks
		*	Si la bibliothèque n'a pas pu être chargée, une std::runtime_error est lancée.
		*\param[in] path
		*	Le chemin du fichier.
		*/
		DynamicLibrary( std::string const & path );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur
		*/
		~DynamicLibrary()noexcept;
		/**
		*\~english
		*\brief
		*	Retrieves a function.
		*\param[in] name
		*	The function name.
		*\param[out] function
		*	Receives the function.
		*\return
		*	\p true if the function was correctly retrieved.
		*\~french
		*\brief
		*	Récupère une fonction.
		*\param[out] function
		*	Reçoit la fonction.
		*\param[in] name
		*	Le nom de la fonction.
		*\return
		*	\p true si la fonction a été correctement récupérée
		*/
		template< typename FuncType >
		bool getFunction( std::string const & name, FuncType & function )noexcept
		{
			function = reinterpret_cast< FuncType >( doGetFunction( name ) );
			return function != nullptr;
		}
		/**
		*\~english
		*\return
		*	The file path.
		*\~french
		*\return
		*	Le chemin vers le fichier.
		*/
		inline std::string const & getPath()const
		{
			return m_path;
		}

	private:
		void doOpen();
		void doClose()noexcept;
		void * doGetFunction( std::string const & p_name )noexcept;

	private:
		std::string m_path;
		void * m_library{ nullptr };
	};
}

#endif
