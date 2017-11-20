/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "PixelFormat.hpp"

#include <cassert>
#include <ctime>
#include <functional>
#include <iomanip>
#include <map>
#include <memory>
#include <sstream>
#include <vector>


namespace renderer
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
	*\name Typedefs généralistes.
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
	*@~french
	*@brief
	*	Convertit un renderer::PixelFormat en VkFormat.
	*@param[in] format
	*	Le renderer::PixelFormat.
	*@return
	*	Le format Vulkan.
	*@~english
	*@brief
	*	Converts a renderer::PixelFormat to a VkFormat.
	*@param[in] format
	*	The renderer::PixelFormat.
	*@return
	*	The Vulkan format.
	*/
	VkFormat convert( PixelFormat format );
}
