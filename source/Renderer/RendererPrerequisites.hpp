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
#include "IndexType.hpp"
#include "MemoryMapFlag.hpp"
#include "MemoryPropertyFlag.hpp"
#include "MipmapMode.hpp"
#include "PipelineBindPoint.hpp"
#include "PipelineStageFlag.hpp"
#include "PrimitiveTopology.hpp"
#include "ShaderStageFlag.hpp"
#include "WrapMode.hpp"

#include <Utils/PixelFormat.hpp>

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
	class Buffer;
	template< typename T >
	class VertexBuffer;
	template< typename T >
	class UniformBuffer;
	template< typename T >
	class Attribute;

	class BufferMemoryBarrier;
	class CommandBuffer;
	class Connection;
	class DescriptorSet;
	class DescriptorSetLayout;
	class DescriptorSetLayoutBinding;
	class DescriptorSetPool;
	class Device;
	class FrameBuffer;
	class ImageMemoryBarrier;
	class ImageSubresourceRange;
	class Pipeline;
	class PipelineLayout;
	class RenderBuffer;
	class Renderer;
	class RenderingResources;
	class Sampler;
	class Scissor;
	class ShaderProgram;
	class StagingBuffer;
	class SwapChain;
	class Texture;
	class VertexBufferBase;
	class VertexLayout;
	class Viewport;

	/**
	*\name Typedefs généralistes.
	*/
	/**@{*/
	using Vec2 = utils::Vec2;
	using Vec3 = utils::Vec3;
	using Vec4 = utils::Vec4;
	using Mat4 = utils::Mat4;
	using IVec2 = utils::IVec2;
	using IVec3 = utils::IVec3;
	using IVec4 = utils::IVec4;
	using Quaternion = utils::Quaternion;
	using RgbaColour = utils::RgbaColour;
	using RgbColour = utils::RgbColour;
	using ByteArray = utils::ByteArray;
	using UInt16Array = utils::UInt16Array;
	using UInt32Array = utils::UInt32Array;
	using Vec3Array = utils::Vec3Array;
	using Vec2Array = utils::Vec2Array;
	using StringArray = utils::StringArray;

	template< typename T >
	using BufferPtr = std::unique_ptr< Buffer< T > >;
	template< typename T >
	using VertexBufferPtr = std::unique_ptr< VertexBuffer< T > >;
	template< typename T >
	using UniformBufferPtr = std::unique_ptr< UniformBuffer< T > >;

	using CommandBufferPtr = std::unique_ptr< CommandBuffer >;
	using DescriptorSetLayoutPtr = std::unique_ptr< DescriptorSetLayout >;
	using DescriptorSetPoolPtr = std::unique_ptr< DescriptorSetPool >;
	using DescriptorSetPtr = std::unique_ptr< DescriptorSet >;
	using DevicePtr = std::unique_ptr< Device >;
	using PipelineLayoutPtr = std::unique_ptr< PipelineLayout >;
	using RenderBufferPtr = std::unique_ptr< RenderBuffer >;
	using RendererPtr = std::unique_ptr< Renderer >;
	using RenderingResourcesPtr = std::unique_ptr< RenderingResources >;
	using ShaderProgramPtr = std::unique_ptr< ShaderProgram >;
	using SwapChainPtr = std::unique_ptr< SwapChain >;
	using VertexLayoutPtr = std::unique_ptr< VertexLayout >;

	using FrameBufferPtr = std::shared_ptr< FrameBuffer >;
	using PipelinePtr = std::shared_ptr< Pipeline >;
	using SamplerPtr = std::shared_ptr< Sampler >;
	using StagingBufferPtr = std::shared_ptr< StagingBuffer >;
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
	*\brief
	*	Convertit un utils::PixelFormat en VkFormat.
	*\param[in] format
	*	Le utils::PixelFormat.
	*\return
	*	Le format Vulkan.
	*/
	VkFormat convert( utils::PixelFormat format )noexcept;
}
