/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "UtilsPrerequisites.hpp"

namespace utils
{
	/**
	*\brief
	*	Enumération des formats de pixels de texture supportés.
	*/
	enum class PixelFormat
	{
		//! Luminance 8 bits (pour les textures uniquement).
		eL8,
		//! Luminance 8 bits + Alpha 8 bits (pour les textures uniquement).
		eL8A8,
		//! RGB avec 8 bits par composante (pour les textures uniquement).
		eR8G8B8,
		//! RGB sur 16 bits.
		eRGB565,
		//! RGBA avec 8 bits par composante (pour les textures uniquement).
		eR8G8B8A8,
		//! BGRA avec 8 bits par composante (pour les textures uniquement).
		eB8G8R8A8,
		//! RGBA sur 16 bits.
		eRGBA5551,
		//! RGBA avec4 bits par composante.
		eRGBA4444,
		//! RGBA avec composante 32 bits flottant.
		eRGBA32F,
		//! Profondeur en 16 bits (pour les render buffers uniquement).
		eD16,
		//! Profondeur en 24 bits, stencil en 8 bits (pour les render buffers uniquement).
		eD24S8,
		//! Profondeur en flottant 32 bits (pour les render buffers uniquement).
		eD32F,
		//! Stencil en 8 bits (pour les render buffers uniquement).
		eS8,
	};
	/**
	*\brief
	*	Donne le nombre d'octets du format de pixels donné.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	uint32_t pixelSize( PixelFormat format )noexcept;
}
