/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>

namespace renderer
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
		//! RGBA sur 16 bits.
		eRGBA5551,
		//! RGBA avec4 bits par composante.
		eRGBA4444,
		//! Profondeur en 16 bits (pour les render buffers uniquement).
		eD16,
		//! Stencil en 8 bits (pour les render buffers uniquement).
		eS8,
		VkLib_EnumBounds( eL8 )
	};
	/**
	*\brief
	*	Convertit un renderer::PixelFormat en VkFormat.
	*\param[in] format
	*	Le renderer::PixelFormat.
	*\return
	*	Le format Vulkan.
	*/
	VkFormat convert( PixelFormat format );
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
