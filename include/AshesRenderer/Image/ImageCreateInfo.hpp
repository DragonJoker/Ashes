/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ImageCreateInfo_HPP___
#define ___Ashes_ImageCreateInfo_HPP___
#pragma once

#include "AshesRenderer/Miscellaneous/Extent3D.hpp"

namespace ashes
{
	struct ImageCreateInfo
	{
		/**
		*\~english
		*\brief
		*	Bitmask describing additional parameters to the image.
		*\~french
		*\brief
		*	Masque de bits décrivant des paramètres additionnels pour l'image.
		*/
		ImageCreateFlags flags;
		/**
		*\~english
		*\brief
		*	The image type.
		*\~french
		*\brief
		*	Le type d'image.
		*/
		ImageType imageType;
		/**
		*\~english
		*\brief
		*	The pixel format.
		*\~french
		*\brief
		*	Le format de l'image.
		*/
		Format format;
		/**
		*\~english
		*\brief
		*	The image dimensions.
		*\~french
		*\brief
		*	Les dimensions de l'image.
		*/
		Extent3D extent{ 1u, 1u, 1u };
		/**
		*\~french
		*\brief
		*	Le nombre de niveaux de mipmaps de la texture.
		*\~english
		*\brief
		*	The mipmap levels count for the texture.
		*/
		uint32_t mipLevels{ 1u };
		/**
		*\~french
		*\brief
		*	Le nombre de couches.
		*\~english
		*\brief
		*	The number of layers.
		*/
		uint32_t arrayLayers{ 1u };
		/**
		*\~english
		*\brief
		*	The image samples count.
		*\~french
		*\brief
		*	Le nombre d'échantillons de l'image.
		*/
		SampleCountFlag samples{ SampleCountFlag::e1 };
		/**
		*\~english
		*\brief
		*	The image tiling mode.
		*\~french
		*\brief
		*	Le mode de tiling de l'image.
		*/
		ImageTiling tiling{ ImageTiling::eOptimal };
		/**
		*\~english
		*\brief
		*	The image usage flags.
		*\~french
		*\brief
		*	Les indicateurs d'utilisation de l'image.
		*/
		ImageUsageFlags usage;
		/**
		*\~english
		*\brief
		*	The sharing mode of the image when it will be accessed by multiple queue families.
		*\~french
		*\brief
		*	Le mode de partage de l'image lorsqu'elle sera accédée via de multiples familles de files.
		*/
		SharingMode sharingMode{ SharingMode::eExclusive };
		/**
		*\~english
		*\brief
		*	La liste des familles de files pouvant accéder à l'image (ignoré si \p sharingMode ne vaut pas ashes::SharingMode::eConcurrent).
		*\~french
		*\brief
		*	The list of queue families that will access this image (ignored if \p sharingMode is not ashes::SharingMode::eConcurrent).
		*/
		std::vector< uint32_t > queueFamilyIndices = {};
		/**
		*\~english
		*\brief
		*	The initial ashes::ImageLayout of all image subresources of the image.
		*\~french
		*\brief
		*	Le layout initial de toutes les sous-ressoureces de l'image.
		*/
		ImageLayout initialLayout{ ImageLayout::eUndefined };
	};
}

#endif
