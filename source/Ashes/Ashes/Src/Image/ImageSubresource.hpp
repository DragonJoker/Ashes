/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ImageSubresource_HPP___
#define ___Ashes_ImageSubresource_HPP___
#pragma once

#include "AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Structure specifying a image subresource.
	*\~french
	*\brief
	*	Spécifie une sous-ressource d'une image.
	*/
	struct ImageSubresource
	{
		/**
		*\~english
		*\brief
		*	Combination of ashes::ImageAspectFlag, selecting the image aspects.
		*\~french
		*\brief
		*	Combinaison de ashes::ImageAspectFlag, sélectionnant les aspects de l'image.
		*/
		ImageAspectFlags aspectMask;
		/**
		*\~english
		*\brief
		*	The mipmap level.
		*\~french
		*\brief
		*	Le niveau de mipmap.
		*/
		uint32_t mipLevel;
		/**
		*\~english
		*\brief
		*	The array layer.
		*\~french
		*\brief
		*	La couche du tableau.
		*/
		uint32_t arrayLayer;
	};
}

#endif
