/**
*\file
*	ObjLoader.h
*\author
*	Sylvain Doremus
*/
#ifndef ___ObjLoader_HPP___
#define ___ObjLoader_HPP___
#pragma once

#include "FileUtils.hpp"

namespace common
{
	/**
	*\brief
	*	Charge un objet.
	*\param[in] filePath
	*	Le chemin d'accès au fichier.
	*\return
	*	L'objet chargé.
	*/
	Object loadObject( std::string const & folder
		, std::string const & fileName
		, common::ImagePtrArray & images
		, float rescale = 8.0f );
}

#endif
