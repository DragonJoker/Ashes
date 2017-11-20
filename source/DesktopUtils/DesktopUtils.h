/**
*\file
*	DesktopUtils.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_HPP___
#define ___DesktopUtils_HPP___
#pragma once

#include "DesktopUtilsPrerequisites.h"

namespace utils
{
	/**
	*\brief
	*	Charge le contenu d'un fichier texte des assets dans un std::string.
	*\param[in] fileName
	*	Le nom du fichier texte.
	*\return
	*	Le contenu du fichier, vide si non trouvé
	*/
	std::string getFileTextContent( std::string const & fileName );
	/**
	*\brief
	*	Charge le contenu d'un fichier binaire des assets dans un std::vector< uint8_t >.
	*\param[in] fileName
	*	Le nom du fichier binaire.
	*\return
	*	Le contenu du fichier, vide si non trouvé
	*/
	std::vector< uint8_t > getFileBinaryContent( std::string const & fileName );
}

#endif
