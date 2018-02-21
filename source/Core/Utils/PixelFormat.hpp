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
