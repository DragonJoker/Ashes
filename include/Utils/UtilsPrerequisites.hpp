/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

#include <cassert>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace utils
{
	using Clock = std::chrono::high_resolution_clock;
	using Milliseconds = std::chrono::milliseconds;

	/**
	*\name Prédéclarations.
	*/
	/**\{*/
	template< class Obj
		, class Key
		, class PtrType = std::shared_ptr< Obj >
		, typename Creator = std::function< PtrType() >
		, class Predicate = std::less< Key > >
	class Factory;
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
	/**\}*/
	/**
	*\name Typedefs généralistes.
	*/
	/**\{*/
	using Vec2 = Vec2T< float >;
	using Vec3 = Vec3T< float >;
	using Vec4 = Vec4T< float >;
	using Mat4 = Mat4T< float >;
	using IVec2 = Vec2T< int >;
	using IVec3 = Vec3T< int >;
	using IVec4 = Vec4T< int >;
	using UIVec2 = Vec2T< uint32_t >;
	using UIVec3 = Vec3T< uint32_t >;
	using UIVec4 = Vec4T< uint32_t >;
	using Quaternion = QuaternionT< float >;
	using RgbaColour = Vec4;
	using RgbColour = Vec3;
	using ByteArray = std::vector< uint8_t >;
	using UInt16Array = std::vector< uint16_t >;
	using UInt32Array = std::vector< uint32_t >;
	using Vec3Array = std::vector< Vec3 >;
	using Vec2Array = std::vector< Vec2 >;
	using StringArray = std::vector< std::string >;
	/**\}*/
	/**
	*\name Fonctions d'aide.
	*/
	/**\{*/
	ashes::Extent2D makeExtent2D( UIVec2 const & value );
	ashes::Extent3D makeExtent3D( UIVec2 const & value );
	ashes::Extent3D makeExtent3D( UIVec3 const & value );
	ashes::Offset2D makeOffset2D( IVec2 const & value );
	ashes::Offset3D makeOffset3D( IVec3 const & value );
	ashes::ClearColorValue makeClearColorValue( RgbColour const & value );
	ashes::ClearColorValue makeClearColorValue( RgbaColour const & value );
	/**\}*/
}
