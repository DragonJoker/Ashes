/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_Prerequisites_HPP___
#define ___Renderer_Prerequisites_HPP___
#pragma once

#include "Config.hpp"

#include "AccessFlag.hpp"
#include "AttributeFormat.hpp"
#include "BlendFactor.hpp"
#include "BlendOp.hpp"
#include "BufferTarget.hpp"
#include "ColourComponentFlag.hpp"
#include "CommandBufferResetFlag.hpp"
#include "CommandBufferUsageFlag.hpp"
#include "CommandPoolCreateFlag.hpp"
#include "CompareOp.hpp"
#include "CullModeFlag.hpp"
#include "DepthStencilStateFlag.hpp"
#include "DescriptorType.hpp"
#include "FenceCreateFlag.hpp"
#include "Filter.hpp"
#include "FrontFace.hpp"
#include "ImageAspectFlag.hpp"
#include "ImageLayout.hpp"
#include "ImageTiling.hpp"
#include "ImageUsageFlag.hpp"
#include "IndexType.hpp"
#include "LogicOp.hpp"
#include "MemoryMapFlag.hpp"
#include "MemoryPropertyFlag.hpp"
#include "MipmapMode.hpp"
#include "MultisampleStateFlag.hpp"
#include "PipelineBindPoint.hpp"
#include "PipelineStageFlag.hpp"
#include "PolygonMode.hpp"
#include "PrimitiveTopology.hpp"
#include "QueryControlFlag.hpp"
#include "QueryPipelineStatisticFlag.hpp"
#include "RasterisationStateFlag.hpp"
#include "SampleCountFlag.hpp"
#include "ShaderStageFlag.hpp"
#include "StencilOp.hpp"
#include "SubpassContents.hpp"
#include "TessellationStateFlag.hpp"
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
	class Attribute;
	template< typename T >
	class Buffer;
	template< typename T >
	class UniformBuffer;
	template< typename T >
	class VertexBuffer;

	class AttributeBase;
	class BackBuffer;
	class BufferBase;
	class BufferMemoryBarrier;
	class ColourBlendState;
	class ColourBlendStateAttachment;
	class CommandBuffer;
	class CommandPool;
	class Connection;
	class DepthStencilState;
	class DescriptorSet;
	class DescriptorSetLayout;
	class DescriptorSetLayoutBinding;
	class DescriptorSetPool;
	class Device;
	class Fence;
	class FrameBuffer;
	class ImageMemoryBarrier;
	class ImageSubresourceRange;
	class IWindowHandle;
	class MultisampleState;
	class Pipeline;
	class PipelineLayout;
	class Queue;
	class RasterisationState;
	class RenderBuffer;
	class Renderer;
	class RenderingResources;
	class RenderPass;
	class RenderPassState;
	class RenderSubpass;
	class RenderSubpassState;
	class Sampler;
	class Semaphore;
	class Scissor;
	class ShaderProgram;
	class StagingBuffer;
	class StencilOpState;
	class SwapChain;
	class TessellationState;
	class Texture;
	class UniformBufferBase;
	class VertexBufferBase;
	class VertexLayout;
	class Viewport;

	struct ClearValue;

	/**
	*\name Typedefs généralistes.
	*/
	/**\{*/
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
	using AttributePtr = std::unique_ptr< Attribute< T > >;
	template< typename T >
	using BufferPtr = std::unique_ptr< Buffer< T > >;
	template< typename T >
	using UniformBufferPtr = std::unique_ptr< UniformBuffer< T > >;
	template< typename T >
	using VertexBufferPtr = std::unique_ptr< VertexBuffer< T > >;

	using AttributeBasePtr = std::unique_ptr< AttributeBase >;
	using BufferBasePtr = std::unique_ptr< BufferBase >;
	using CommandBufferPtr = std::unique_ptr< CommandBuffer >;
	using CommandPoolPtr = std::unique_ptr< CommandPool >;
	using ConnectionPtr = std::unique_ptr< Connection >;
	using DescriptorSetLayoutPtr = std::unique_ptr< DescriptorSetLayout >;
	using DescriptorSetLayoutBindingPtr = std::unique_ptr< DescriptorSetLayoutBinding >;
	using DescriptorSetPoolPtr = std::unique_ptr< DescriptorSetPool >;
	using DescriptorSetPtr = std::unique_ptr< DescriptorSet >;
	using DevicePtr = std::unique_ptr< Device >;
	using FencePtr = std::unique_ptr< Fence >;
	using IWindowHandlePtr = std::unique_ptr< IWindowHandle >;
	using PipelineLayoutPtr = std::unique_ptr< PipelineLayout >;
	using QueuePtr = std::unique_ptr< Queue >;
	using RenderBufferPtr = std::unique_ptr< RenderBuffer >;
	using RendererPtr = std::unique_ptr< Renderer >;
	using RenderingResourcesPtr = std::unique_ptr< RenderingResources >;
	using RenderPassPtr = std::unique_ptr< RenderPass >;
	using RenderSubpassPtr = std::unique_ptr< RenderSubpass >;
	using SemaphorePtr = std::unique_ptr< Semaphore >;
	using ShaderProgramPtr = std::unique_ptr< ShaderProgram >;
	using SwapChainPtr = std::unique_ptr< SwapChain >;
	using VertexBufferBasePtr = std::unique_ptr< VertexBufferBase >;
	using VertexLayoutPtr = std::unique_ptr< VertexLayout >;
	using UniformBufferBasePtr = std::unique_ptr< UniformBufferBase >;

	using DescriptorSetLayoutBindingArray = std::vector< DescriptorSetLayoutBinding >;
	using FrameBufferPtr = std::shared_ptr< FrameBuffer >;
	using PipelinePtr = std::shared_ptr< Pipeline >;
	using SamplerPtr = std::shared_ptr< Sampler >;
	using StagingBufferPtr = std::shared_ptr< StagingBuffer >;
	using TexturePtr = std::shared_ptr< Texture >;

	using FrameBufferPtrArray = std::vector< FrameBufferPtr >;
	using CommandBufferPtrArray = std::vector< CommandBufferPtr >;
	using RenderSubpassPtrArray = std::vector< RenderSubpassPtr >;

	using ClearValueArray = std::vector< ClearValue >;
	using ColourBlendStateAttachmentArray = std::vector< ColourBlendStateAttachment >;
	using ImageLayoutArray = std::vector< ImageLayout >;
	using PipelineStageFlagsArray = std::vector< PipelineStageFlags >;
	using RenderSubpassArray = std::vector< RenderSubpass >;

	using CommandBufferCRef = std::reference_wrapper< CommandBuffer const >;
	using SemaphoreCRef = std::reference_wrapper< Semaphore const >;
	using SwapChainCRef = std::reference_wrapper< SwapChain const >;
	using TextureCRef = std::reference_wrapper< Texture const >;
	using VertexLayoutCRef = std::reference_wrapper< VertexLayout const >;

	using CommandBufferCRefArray = std::vector< CommandBufferCRef >;
	using SemaphoreCRefArray = std::vector< SemaphoreCRef >;
	using SwapChainCRefArray = std::vector< SwapChainCRef >;
	using TextureCRefArray = std::vector< TextureCRef >;
	using VertexLayoutCRefArray = std::vector< VertexLayoutCRef >;
	/**\}*/
	/**
	*\name Typedefs d'attributs de sommets.
	*/
	/**\{*/
	using FloatAttribute = Attribute< float >;
	using Vec2Attribute = Attribute< Vec2 >;
	using Vec3Attribute = Attribute< Vec3 >;
	using Vec4Attribute = Attribute< Vec4 >;
	using FloatAttributePtr = AttributePtr< float >;
	using Vec2AttributePtr = AttributePtr< Vec2 >;
	using Vec3AttributePtr = AttributePtr< Vec3 >;
	using Vec4AttributePtr = AttributePtr< Vec4 >;
	/**\}*/
}

#endif
