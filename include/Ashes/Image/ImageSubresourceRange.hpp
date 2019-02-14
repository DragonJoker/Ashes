/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ImageSubresourceRange_HPP___
#define ___Ashes_ImageSubresourceRange_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies an image subresource range.
	*\~french
	*\brief
	*	Spécifie l'intervalle de sous-ressource d'une image.
	*/
	struct ImageSubresourceRange
	{
		/**
		*\~english
		*\brief
		*	Bitmask specifying an image aspects.
		*\~french
		*\brief
		*	Masques de bits décrivant les aspects d'une image.
		*/
		ImageAspectFlags aspectMask;
		/**
		*\~english
		*\brief
		*	The mipmap base level.
		*\~french
		*\brief
		*	Le niveau de base de mipmap.
		*/
		uint32_t baseMipLevel{ 0u };
		/**
		*\~english
		*\brief
		*	The level count.
		*\~french
		*\brief
		*	Le nombre de niveaux.
		*/
		uint32_t levelCount{ 1u };
		/**
		*\~english
		*\brief
		*	The array base layer.
		*\~french
		*\brief
		*	La couche de base du tableau.
		*/
		uint32_t baseArrayLayer{ 0u };
		/**
		*\~english
		*\brief
		*	The array layers count.
		*\~french
		*\brief
		*	Le nombre de couches du tableau.
		*/
		uint32_t layerCount{ 1u };
	};
}

#endif
