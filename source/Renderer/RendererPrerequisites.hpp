/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "AccessFlag.hpp"
#include "BufferTarget.hpp"
#include "CommandBufferResetFlag.hpp"
#include "CommandBufferUsageFlag.hpp"
#include "DescriptorType.hpp"
#include "Filter.hpp"
#include "ImageAspectFlag.hpp"
#include "ImageLayout.hpp"
#include "MemoryMapFlag.hpp"
#include "MemoryPropertyFlag.hpp"
#include "MipmapMode.hpp"
#include "PipelineBindPoint.hpp"
#include "PipelineStageFlag.hpp"
#include "PixelFormat.hpp"
#include "PrimitiveTopology.hpp"
#include "ShaderStageFlag.hpp"
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

	class BufferMemoryBarrier;
	class CommandBuffer;
	class DescriptorSet;
	class DescriptorSetLayout;
	class DescriptorSetLayoutBinding;
	class DescriptorSetPool;
	class ImageMemoryBarrier;
	class ImageSubresourceRange;
	class Pipeline;
	class PipelineLayout;
	class RenderingResources;
	class Sampler;
	class Scissor;
	class ShaderProgram;
	class StagingBuffer;
	class Texture;
	class VertexLayout;
	class Viewport;

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
	using CommandBufferPtr = std::unique_ptr< CommandBuffer >;
	using TexturePtr = std::shared_ptr< Texture >;
	using StagingBufferPtr = std::shared_ptr< StagingBuffer >;
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
}
