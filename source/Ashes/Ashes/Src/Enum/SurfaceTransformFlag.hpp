/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_SurfaceTransformFlag_HPP___
#define ___Ashes_SurfaceTransformFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Surface transformations enumeration.
	*\~french
	*\brief
	*	Enumération des types de transformation de la surface.
	*/
	enum class SurfaceTransformFlag
		: uint32_t
	{
		eIdentity = 0x00000001,
		eRotate90 = 0x00000002,
		eRotate180 = 0x00000004,
		eRotate270 = 0x00000008,
		eHorizontalMirror = 0x00000010,
		eHorizontalMirrorRotate90 = 0x00000020,
		eHorizontalMirrorRotate180 = 0x00000040,
		eHorizontalMirrorRotate270 = 0x00000080,
		eInherit = 0x00000100,
	};
	Ashes_ImplementFlag( SurfaceTransformFlag )
	/**
	*\~english
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*\~french
	*\brief
	*	Récupère le nom du type d'élément donné.
	*\param[in] value
	*	Le type d'élément.
	*\return
	*	Le nom.
	*/
	inline std::string getName( SurfaceTransformFlags value )
	{
		std::string result;
		std::string sep;

		if ( checkFlag( value, SurfaceTransformFlag::eIdentity ) )
		{
			result += sep + "identity";
			sep = " | ";
		}

		if ( checkFlag( value, SurfaceTransformFlag::eRotate90 ) )
		{
			result += sep + "rotate_90";
			sep = " | ";
		}

		if ( checkFlag( value, SurfaceTransformFlag::eRotate180 ) )
		{
			result += sep + "rotate_180";
			sep = " | ";
		}

		if ( checkFlag( value, SurfaceTransformFlag::eRotate270 ) )
		{
			result += sep + "rotate_270";
			sep = " | ";
		}

		if ( checkFlag( value, SurfaceTransformFlag::eHorizontalMirror ) )
		{
			result += sep + "horizontal_mirror";
			sep = " | ";
		}

		if ( checkFlag( value, SurfaceTransformFlag::eHorizontalMirrorRotate90 ) )
		{
			result += sep + "horizontal_mirror_rotate_90";
			sep = " | ";
		}

		if ( checkFlag( value, SurfaceTransformFlag::eHorizontalMirrorRotate180 ) )
		{
			result += sep + "horizontal_mirror_rotate_180";
			sep = " | ";
		}

		if ( checkFlag( value, SurfaceTransformFlag::eHorizontalMirrorRotate270 ) )
		{
			result += sep + "horizontal_mirror_rotate_270";
			sep = " | ";
		}

		if ( checkFlag( value, SurfaceTransformFlag::eInherit ) )
		{
			result += sep + "inherit";
			sep = " | ";
		}

		return result;
	}
}

#endif
