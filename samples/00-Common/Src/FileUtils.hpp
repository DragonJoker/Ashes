/*
See LICENSE file in root folder
*/
#ifndef ___CASTOR_FILE_H___
#define ___CASTOR_FILE_H___

#include "Prerequisites.hpp"

#include <util/UtilsFile.hpp>

#include <fstream>

#ifdef min
#	undef min
#	undef max
#	undef abs
#endif

namespace common
{
	/**
	*\~english
	*\brief
	*	Loads an image.
	*\param[in] path
	*	The image gile path.
	*\return
	*	The image data.
	*\~french
	*\brief
	*	Charge une image.
	*\param[in] path
	*	Le chemin d'accès à l'image.
	*\return
	*	Les données de l'image.
	*/
	Image loadImage( std::string const & path );
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
	ashes::UInt32Array dumpShaderFile( ashes::Device const & device
		, VkShaderStageFlagBits stage
		, std::string const & file );
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
	ashes::UInt32Array dumpSpvFile( std::string const & file );
}

#endif
