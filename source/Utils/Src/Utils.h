/**
*\file
*	Transform.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Utils_HPP___
#define ___Utils_HPP___
#pragma once

#include <cassert>
#include <vector>

#define Utils_EnumBounds( MinValue )\
	eCount,\
	eMin = MinValue,\
	eMax = eCount - 1

namespace utils
{
	template< typename T >
	class Vec2T;
	template< typename T >
	class Vec3T;
	template< typename T >
	class Vec4T;
	template< typename T >
	class Mat4T;
	template< typename T >
	class QuaternionT;
	template< typename Function >
	class Signal;

	/**
	*\name Typedefs g�n�ralistes.
	*/
	/**@{*/
	using Vec2 = Vec2T< float >;
	using Vec3 = Vec3T< float >;
	using Vec4 = Vec4T< float >;
	using Mat4 = Mat4T< float >;
	using IVec2 = Vec2T< int >;
	using IVec3 = Vec3T< int >;
	using IVec4 = Vec4T< int >;
	using Quaternion = QuaternionT< float >;
	using RgbaColour = Vec4;
	using RgbColour = Vec3;
	using ByteArray = std::vector< uint8_t >;
	using UInt16Array = std::vector< uint16_t >;
	using UInt32Array = std::vector< uint32_t >;
	using Vec3Array = std::vector< Vec3 >;
	using Vec2Array = std::vector< Vec2 >;
	using StringArray = std::vector< std::string >;
	/**@}*/
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
		Utils_EnumBounds( eL8 )
	};
}

#endif
