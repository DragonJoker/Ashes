/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_UtilsMapping_HPP___
#define ___Renderer_UtilsMapping_HPP___
#pragma once

#include <Utils/Angle.hpp>
#include <Utils/Degrees.hpp>
#include <Utils/DynamicLibrary.hpp>
#include <Utils/FlagCombination.hpp>
#include <Utils/Mat4.hpp>
#include <Utils/PlaneEquation.hpp>
#include <Utils/PixelFormat.hpp>
#include <Utils/Quaternion.hpp>
#include <Utils/Radians.hpp>
#include <Utils/UtilsSignal.hpp>
#include <Utils/Vec2.hpp>
#include <Utils/Vec3.hpp>
#include <Utils/Vec4.hpp>

namespace renderer
{
	/**
	*\name Mappings avec la bibliothèque utilitaire.
	*/
	/**\{*/
	template< typename T >
	using Vec2T = utils::Vec2T< T >;
	template< typename T >
	using Vec3T = utils::Vec3T< T >;
	template< typename T >
	using Vec4T = utils::Vec4T< T >;
	template< typename T >
	using Mat4T = utils::Mat4T< T >;
	template< typename Function >
	using Signal = utils::Signal< Function >;
	template< typename Signal >
	using SignalConnection = utils::Connection< Signal >;

	using Vec2 = utils::Vec2;
	using Vec3 = utils::Vec3;
	using Vec4 = utils::Vec4;
	using Mat4 = utils::Mat4;
	using IVec2 = utils::IVec2;
	using IVec3 = utils::IVec3;
	using IVec4 = utils::IVec4;
	using UIVec2 = utils::UIVec2;
	using UIVec3 = utils::UIVec3;
	using UIVec4 = utils::UIVec4;
	using Quaternion = utils::Quaternion;
	using RgbaColour = utils::RgbaColour;
	using RgbColour = utils::RgbColour;
	using Radians = utils::Radians;
	using Degrees = utils::Degrees;
	using Angle = utils::Angle;
	using PixelFormat = utils::PixelFormat;
	using Clock = utils::Clock;
	using Milliseconds = utils::Milliseconds;
	using DynamicLibrary = utils::DynamicLibrary;
	using ByteArray = utils::ByteArray;
	using UInt16Array = utils::UInt16Array;
	using UInt32Array = utils::UInt32Array;
	using UInt64Array = std::vector< uint64_t >;
	using SizeTArray = std::vector< size_t >;
	using FloatArray = std::vector< float >;
	using Vec3Array = utils::Vec3Array;
	using Vec2Array = utils::Vec2Array;
	using StringArray = utils::StringArray;

	using utils::checkFlag;
	/**\}*/
}

#include "PixelFormat.hpp"

#endif
