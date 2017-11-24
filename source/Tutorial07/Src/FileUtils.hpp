/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace vkapp
{
	using ByteArray = std::vector< uint8_t >;
	using UInt32Array = std::vector< uint32_t >;
	/**
	*@~french
	*@brief
	*	Charge le contenu d'un fichier binaire dans un tableau d'octets.
	*@param[in] file
	*	Le chemin d'accès au fichier.
	*@return
	*	Le contenu du fichier.
	*@~english
	*@brief
	*	Loads the content of a binary file into a byte array.
	*@param[in] file
	*	The file path.
	*@return
	*	The file content.
	*/
	ByteArray dumpBinaryFile( std::string const & file );
	/**
	*@~french
	*@brief
	*	Charge le contenu d'un fichier binaire dans un tableau d'octets.
	*@param[in] file
	*	Le chemin d'accès au fichier.
	*@return
	*	Le contenu du fichier.
	*@~english
	*@brief
	*	Loads the content of a binary file into a byte array.
	*@param[in] file
	*	The file path.
	*@return
	*	The file content.
	*/
	UInt32Array dumpSpvFile( std::string const & file );
	/**
	*@~french
	*@return
	*	Le dossier d'exécution du programme.
	*@~english
	*@return
	*	The executable folder.
	*/
	std::string getExecutableDirectory();
	/**
	*@~french
	*@brief
	*	Récupère le dossier parent du chemin donné.
	*@param[in] path
	*	Le chemin.
	*@return
	*	Le dossier parent.
	*@~english
	*@brief
	*	Retrieves the given path's parent folder.
	*@param[in] path
	*	The path.
	*@return
	*	The parent folder.
	*/
	std::string getPath( std::string const & path );
}
/**
*@~french
*@brief
*	Concatène deux chemins en utilisant le séparateur de dossier.
*@param[in] lhs, rhs
*	Les chemins à concaténer.
*@return
*	Le chemin concaténé (lhs / rhs).
*@~english
*@brief
*	Concatenates two paths, adding the folder separator.
*@param[in] lhs, rhs
*	The paths to concatenate.
*@return
*	The concatenated path (lhs / rhs).
*/
std::string operator/( std::string const & lhs, std::string const & rhs );
/**
*@~french
*@brief
*	Concatène deux chemins en utilisant le séparateur de dossier.
*@param[in] lhs, rhs
*	Les chemins à concaténer.
*@return
*	Le chemin concaténé (lhs / rhs).
*@~english
*@brief
*	Concatenates two paths, adding the folder separator.
*@param[in] lhs, rhs
*	The paths to concatenate.
*@return
*	The concatenated path (lhs / rhs).
*/
inline std::string operator/( std::string const & lhs, char const * const rhs )
{
	return lhs / std::string{ rhs };
}
/**
*@~french
*@brief
*	Concatène deux chemins en utilisant le séparateur de dossier.
*@param[in] lhs, rhs
*	Les chemins à concaténer.
*@return
*	Le chemin concaténé (lhs / rhs).
*@~english
*@brief
*	Concatenates two paths, adding the folder separator.
*@param[in] lhs, rhs
*	The paths to concatenate.
*@return
*	The concatenated path (lhs / rhs).
*/
inline std::string operator/( char const * const lhs, std::string const & rhs )
{
	return std::string{ lhs } / rhs;
}
