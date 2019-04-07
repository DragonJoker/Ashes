/*
See LICENSE file in root folder
*/
#ifndef ___Common_FileUtils_H___
#define ___Common_FileUtils_H___

#include "Prerequisites.hpp"

#include <Utils/UtilsFile.hpp>

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
	ImageData loadImage( std::string const & path );
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
	ashes::UInt32Array parseShaderFile( ashes::Device const & device
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
