/*
See LICENSE file in root folder
*/
#ifndef ___Utils_FileUtils_H___
#define ___Utils_FileUtils_H___
#pragma once

#include "UtilsPrerequisites.hpp"

#include <ashes/common/FileUtils.hpp>

#include <fstream>

namespace utils
{
	using StringArray = ashes::StringArray;
	/**
	*\~french
	*\brief
	*	Charge le contenu d'un fichier texte dans un tableau d'octets.
	*\param[in] file
	*	Le chemin d'accès au fichier.
	*\return
	*	Le contenu du fichier.
	*\~english
	*\brief
	*	Loads the content of a text file into a byte array.
	*\param[in] file
	*	The file path.
	*\return
	*	The file content.
	*/
	std::string dumpTextFile( std::string const & file );
	/**
	*\~french
	*\brief
	*	Charge le contenu d'un fichier binaire dans un tableau d'octets.
	*\param[in] file
	*	Le chemin d'accès au fichier.
	*\return
	*	Le contenu du fichier.
	*\~english
	*\brief
	*	Loads the content of a binary file into a byte array.
	*\param[in] file
	*	The file path.
	*\return
	*	The file content.
	*/
	ashes::ByteArray dumpBinaryFile( std::string const & file );
}

#endif
