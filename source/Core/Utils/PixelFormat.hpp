/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "UtilsPrerequisites.hpp"

namespace utils
{
	/**
	*\~english
	*\brief
	*	Supported pixel formats enumeration.
	*\~french
	*\brief
	*	Enumération des formats de pixels de texture supportés.
	*/
	enum class PixelFormat
	{
		/**
		*\~english
		*name
		*	Colour formats.
		*\~french
		*name
		*	Formats de couleur.
		*/
		/**@{*/
		//!\~english	1 integer component on 8 bits.
		//!\~french		1 composante entière sur 8 bits.
		eR8,
		//!\~english	1 floating point component on 32 bits.
		//!\~french		1 composante flottante sur 32 bits.
		eR32F,
		//!\~english	2 integer components on 8 bits.
		//!\~french		2 composantes entières sur 8 bits.
		eR8G8,
		//!\~english	2 floating point components on 32 bits.
		//!\~french		2 composantes flottantes sur 32 bits.
		eRG32F,
		//!\~english	3 integer components on 8 bits.
		//!\~french		3 composantes entières sur 8 bits.
		eR8G8B8,
		//!\~english	3 floating point components on 32 bits.
		//!\~french		3 composantes flottantes sur 32 bits.
		eRGB32F,
		//!\~english	4 integer components on 8 bits, RGBA.
		//!\~french		4 composantes entières sur 8 bits, RGBA.
		eR8G8B8A8,
		//!\~english	4 integer components on 8 bits, BGRA.
		//!\~french		4 composantes entières sur 8 bits, BGRA.
		eB8G8R8A8,
		//!\~english	4 floating point components on 32 bits.
		//!\~french		4 composantes flottantes sur 32 bits.
		eRGBA32F,
		/**@}*/
		/**
		*\~english
		*name
		*	Depth / stencil formats.
		*\~french
		*name
		*	Formats de profondeur / stencil.
		*/
		/**@{*/
		//!\~english	Integer depth on 16 bits.
		//!\~french		Profondeur entière sur 16 bits.
		eD16,
		//!\~english	Integer depth on 24 bits, stencil on 8 bits.
		//!\~french		Profondeur entière sur 24 bits, stencil sur 8 bits.
		eD24S8,
		//!\~english	Floating point depth on 32 bits.
		//!\~french		Profondeur flottante sur 32 bits.
		eD32F,
		//!\~english	Floating point depth on 32 bits, stencil on 8 bits.
		//!\~french		Profondeur flottante sur 32 bits, stencil sur 8 bits.
		eD32FS8,
		//!\~english	Stencil on 8 bits.
		//!\~french		Stencil sur 8 bits.
		eS8,
		/**@}*/
		/**
		*\~english
		*name
		*	Block compression formats.
		*\~french
		*name
		*	Formats de compression par bloc.
		*/
		/**@{*/
		//!\~english	4x4 Block compression (DXT1), 8 bytes per block.
		//!\~french		Compression par bloc 4x4 (DXT1), 8 octets par bloc.
		eBC1_RGB,
		//!\~english	4x4 Block compression (DXT1), 8 bytes per block, with 1 alpha bit.
		//!\~french		Compression par bloc 4x4 (DXT1), 8 octets par bloc, dont 1 bit pour l'alpha.
		eBC1_RGBA,
		//!\~english	4x4 Block compression (DXT1), 8 bytes per block.
		//!\~french		Compression par bloc 4x4 (DXT1), 8 octets par bloc.
		eBC1_SRGB,
		//!\~english	4x4 Block compression (DXT1), 8 bytes per block, with 1 alpha bit.
		//!\~french		Compression par bloc 4x4 (DXT1), 8 octets par bloc, dont 1 bit pour l'alpha.
		eBC1_SRGBA,
		//!\~english	4x4 Block compression (DXT3), 16 bytes per block (combination of BC1 for RGB, and 4 bits per pixel for alpha).
		//!\~french		Compression par bloc 4x4 (DXT3), 16 octets par bloc (combinaison de BC1 pour le RGB, et 4 bits par pixel pour l'alpha).
		eBC2_RGBA,
		//!\~english	4x4 Block compression BC2 compressed (DXT3), 16 bytes per block (combination of BC1 for SRGB, and 4 bits per pixel for alpha).
		//!\~french		Compression par bloc 4x4, BC2 (DXT3), 16 octets par bloc (combinaison de BC1 pour le SRGB, et 4 bits par pixel pour l'alpha).
		eBC2_SRGBA,
		//!\~english	4x4 Block compression (DXT5), 16 bytes per block (combination of BC1 for RGB and BC4 for alpha).
		//!\~french		Compression par bloc 4x4 (DXT5), 16 octets par bloc (combinaison de BC1 pour le RGB et BC4 pour l'alpha).
		eBC3_RGBA,
		//!\~english	4x4 Block compression (DXT5), 16 bytes per block (combination of BC1 for SRGB and BC4 for alpha).
		//!\~french		Compression par bloc 4x4 (DXT5), 16 octets par bloc (combinaison de BC1 pour le SRGB et BC4 pour l'alpha).
		eBC3_SRGBA,
		//!\~english	4x4 grayscale block compression compressed, 8 bytes per block.
		//!\~french		Compression par bloc 4x4 en niveaux de gris, 8 octets par bloc.
		eBC4,
		//!\~english	4x4 Block compression, 16 bytes per block (combination of two BC4).
		//!\~french		Compression par bloc 4x4, 16 octets par bloc (combinaison de two BC4).
		eBC5_RG,
		/**@}*/
		/**
		*\~english
		*name
		*	ASTC compressed formats.
		*\~french
		*name
		*	Formats de compression ASTC.
		*/
		/**@{*/
		eASTC_4x4_RGBA,
		eASTC_4x4_SRGBA,
		eASTC_5x4_RGBA,
		eASTC_5x4_SRGBA,
		eASTC_5x5_RGBA,
		eASTC_5x5_SRGBA,
		eASTC_6x5_RGBA,
		eASTC_6x5_SRGBA,
		eASTC_6x6_RGBA,
		eASTC_6x6_SRGBA,
		eASTC_8x5_RGBA,
		eASTC_8x5_SRGBA,
		eASTC_8x6_RGBA,
		eASTC_8x6_SRGBA,
		eASTC_8x8_RGBA,
		eASTC_8x8_SRGBA,
		eASTC_10x5_RGBA,
		eASTC_10x5_SRGBA,
		eASTC_10x6_RGBA,
		eASTC_10x6_SRGBA,
		eASTC_10x8_RGBA,
		eASTC_10x8_SRGBA,
		eASTC_10x10_RGBA,
		eASTC_10x10_SRGBA,
		eASTC_12x10_RGBA,
		eASTC_12x10_SRGBA,
		eASTC_12x12_RGBA,
		eASTC_12x12_SRGBA,
		/**@}*/
		/**
		*\~english
		*name
		*	ETC compressed formats.
		*\~french
		*name
		*	Formats de compression ETC.
		*/
		/**@{*/
		eETC2_R8G8B8,
		eETC2_R8G8B8_SRGB,
		eETC2_R8G8B8A1,
		eETC2_R8G8B8A1_SRGB,
		eETC2_R8G8B8A8,
		eETC2_R8G8B8A8_SRGB,
		/**@}*/
		/**
		*\~english
		*name
		*	ETC compressed formats.
		*\~french
		*name
		*	Formats de compression ETC.
		*/
		/**@{*/
		eEAC_R11U,
		eEAC_R11S,
		eEAC_R11G11U,
		eEAC_R11G11S,
		/**@}*/
	};
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given pixel format.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels donné.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	uint32_t pixelSize( PixelFormat format )noexcept;
}
