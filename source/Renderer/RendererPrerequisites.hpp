/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "BufferTarget.hpp"
#include "Filter.hpp"
#include "MemoryMapFlag.hpp"
#include "MemoryPropertyFlag.hpp"
#include "MipmapMode.hpp"
#include "PixelFormat.hpp"
#include "PrimitiveTopology.hpp"
#include "WrapMode.hpp"

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
	template< typename T >
	class Buffer;
	template< typename T >
	class VertexBuffer;
	template< typename T >
	class UniformBuffer;
	template< typename T >
	class Attribute;

	class Pipeline;
	class RenderingResources;
	class ShaderProgram;
	class Texture;
	class VertexLayout;

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
	template< typename T >
	using BufferPtr = std::unique_ptr< Buffer< T > >;
	template< typename T >
	using VertexBufferPtr = std::unique_ptr< VertexBuffer< T > >;
	using ShaderProgramPtr = std::unique_ptr< ShaderProgram >;
	using VertexLayoutPtr = std::unique_ptr< VertexLayout >;
	using TexturePtr = std::shared_ptr< Texture >;
	/**@}*/
	/**
	*\name Typedefs d'attributs de sommets.
	*/
	/**@{*/
	template< typename T >
	class Attribute;
	using FloatAttribute = Attribute< float >;
	using Vec2Attribute = Attribute< Vec2 >;
	using Vec3Attribute = Attribute< Vec3 >;
	using Vec4Attribute = Attribute< Vec4 >;
	template< typename T >
	using AttributePtr = std::unique_ptr< Attribute< T > >;
	using FloatAttributePtr = AttributePtr< float >;
	using Vec2AttributePtr = AttributePtr< Vec2 >;
	using Vec3AttributePtr = AttributePtr< Vec3 >;
	using Vec4AttributePtr = AttributePtr< Vec4 >;
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
	VkFilter convert( Filter filter );
	VkSamplerMipmapMode convert( MipmapMode mode );
	VkSamplerAddressMode convert( WrapMode mode );
	VkMemoryPropertyFlags convert( MemoryPropertyFlags flags );
	VkBufferUsageFlags convert( BufferTargets targets );
	VkMemoryMapFlags convert( MemoryMapFlags flags );
	VkPrimitiveTopology convert( PrimitiveTopology topology );
}
