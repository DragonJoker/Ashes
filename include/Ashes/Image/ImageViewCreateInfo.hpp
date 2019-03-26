/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ImageViewCreateInfo_HPP___
#define ___Ashes_ImageViewCreateInfo_HPP___
#pragma once

#include "Ashes/Image/ComponentMapping.hpp"
#include "Ashes/Image/ImageSubresourceRange.hpp"

namespace ashes
{
	struct ImageViewCreateInfo
	{
		/**
		*\~english
		*\brief
		*	The view's texture type.
		*\~french
		*\brief
		*	Le type de texture de la vue.
		*/
		ImageViewType viewType;
		/**
		*\~english
		*\brief
		*	The view's pixels format.
		*\~french
		*\brief
		*	Le format des pixels de la vue.
		*/
		Format format;
		/**
		*\~english
		*\brief
		*	The colours component mapping.
		*\~french
		*\brief
		*	Le mapping des composantes de couleur.
		*/
		ComponentMapping components;
		/**
		*\~english
		*\brief
		*	The set of mipmap levels and array layers to be accessible to the view.
		*\~french
		*\brief
		*	L'ensemble de niveaux de mipmaps et de couches de tableau accessibles à la vue.
		*/
		ImageSubresourceRange subresourceRange;
	};
}

#endif
