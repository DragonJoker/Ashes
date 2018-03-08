/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_UtilsMapping_HPP___
#define ___Renderer_UtilsMapping_HPP___
#pragma once

#include <Utils/Angle.hpp>
#include <Utils/DynamicLibrary.hpp>
#include <Utils/FlagCombination.hpp>
#include <Utils/Mat4.hpp>
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
	template< typename Function >
	using Signal = utils::Signal< Function >;
	template< typename Signal >
	using SignalConnection = utils::Connection< Signal >;

	using Vec2 = utils::Vec2;
	using Vec3 = utils::Vec3;
	using Vec4 = utils::Vec4;
	using Mat4 = utils::Mat4;
	using RgbaColour = utils::RgbaColour;
	using Angle = utils::Angle;
	using DynamicLibrary = utils::DynamicLibrary;

	using utils::checkFlag;
	/**\}*/
}

#endif
