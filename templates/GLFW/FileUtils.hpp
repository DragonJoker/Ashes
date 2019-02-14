/*
See LICENSE file in root folder
*/
#ifndef ___CASTOR_FILE_H___
#define ___CASTOR_FILE_H___

#include <fstream>
#include <vector>

namespace common
{
	using StringArray = std::vector< std::string >;
	/**
	*\~english
	*\brief
	*	List all files in a directory, recursively or not.
	*\param[in] folderPath
	*	The directory path.
	*\param[out] files
	*	Receives the files list.
	*\param[in] recursive
	*	Tells if search must be recursive.
	*\return
	*	\p true if the directory is listed.
	*\~french
	*\brief
	*	Liste tous les fichiers d'un répertoire, récursivement ou pas.
	*\param[in] folderPath
	*	Le chemin du dossier.
	*\param[out] files
	*	Reçoit la liste des fichiers.
	*\param[in] recursive
	*	Définit si la recherche doit être récursive ou pas.
	*\return
	*	\p true si le dossier a été listé.
	*/
	bool listDirectoryFiles( std::string const & folderPath
		, StringArray & files
		, bool recursive = false );
	/**
	*\~french
	*\return
	*	Le dossier d'exécution du programme.
	*\~english
	*\return
	*	The executable folder.
	*/
	std::string getExecutableDirectory();
	/**
	*\~french
	*\brief
	*	Récupère le dossier parent du chemin donné.
	*\param[in] path
	*	Le chemin.
	*\return
	*	Le dossier parent.
	*\~english
	*\brief
	*	Retrieves the given path's parent folder.
	*\param[in] path
	*	The path.
	*\return
	*	The parent folder.
	*/
	std::string getPath( std::string const & path );
}
/**
*\~french
*\brief
*	Concatène deux chemins en utilisant le séparateur de dossier.
*\param[in] lhs, rhs
*	Les chemins à concaténer.
*\return
*	Le chemin concaténé (lhs / rhs).
*\~english
*\brief
*	Concatenates two paths, adding the folder separator.
*\param[in] lhs, rhs
*	The paths to concatenate.
*\return
*	The concatenated path (lhs / rhs).
*/
std::string operator/( std::string const & lhs, std::string const & rhs );
/**
*\~french
*\brief
*	Concatène deux chemins en utilisant le séparateur de dossier.
*\param[in] lhs, rhs
*	Les chemins à concaténer.
*\return
*	Le chemin concaténé (lhs / rhs).
*\~english
*\brief
*	Concatenates two paths, adding the folder separator.
*\param[in] lhs, rhs
*	The paths to concatenate.
*\return
*	The concatenated path (lhs / rhs).
*/
inline std::string operator/( std::string const & lhs, char const * const rhs )
{
	return lhs / std::string{ rhs };
}
/**
*\~french
*\brief
*	Concatène deux chemins en utilisant le séparateur de dossier.
*\param[in] lhs, rhs
*	Les chemins à concaténer.
*\return
*	Le chemin concaténé (lhs / rhs).
*\~english
*\brief
*	Concatenates two paths, adding the folder separator.
*\param[in] lhs, rhs
*	The paths to concatenate.
*\return
*	The concatenated path (lhs / rhs).
*/
inline std::string operator/( char const * const lhs, std::string const & rhs )
{
	return std::string{ lhs } / rhs;
}

#endif
