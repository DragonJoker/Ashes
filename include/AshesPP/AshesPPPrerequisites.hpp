/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Prerequisites_HPP___
#define ___AshesPP_Prerequisites_HPP___
#pragma once

#include <ashes/ashes.h>

#include "Miscellaneous/Debug.hpp"
#include "Miscellaneous/Error.hpp"
#include "Miscellaneous/Log.hpp"

#include <AshesRenderer/AshesRendererPrerequisites.hpp>

#include <AshesRenderer/Util/Format.hpp>

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <functional>
#include <iomanip>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace ashes
{
	template< typename T >
	class Buffer;
	template< typename T >
	class UniformBuffer;
	template< typename T >
	class PushConstantsBuffer;
	template< typename T >
	class SpecialisationInfo;
	template< typename T >
	class VertexBuffer;

	struct PushConstant;
	struct ImageView;

	class BufferBase;
	class BufferView;
	class CommandBuffer;
	class CommandPool;
	class ComputePipeline;
	class DescriptorPool;
	class DescriptorSet;
	class DescriptorSetLayout;
	class DescriptorSetPool;
	class Device;
	class DeviceMemory;
	class Event;
	class Fence;
	class FrameBuffer;
	class GraphicsPipeline;
	class Image;
	class Instance;
	class IWindowHandle;
	class PhysicalDevice;
	class PipelineLayout;
	class PushConstantsBufferBase;
	class QueryPool;
	class Queue;
	class Renderer;
	class RenderPass;
	class Sampler;
	class Semaphore;
	class ShaderModule;
	class SpecialisationInfoBase;
	class StagingBuffer;
	class StagingTexture;
	class Surface;
	class SwapChain;
	class UniformBufferBase;
	class VertexBufferBase;

	/**
	*\~french
	*\brief
	*	Temps en maximal à attendre, en nanosecondes.
	*\~english
	*\brief
	*	Maximum time to wait in nanoseconds.
	*/
	static const uint64_t MaxTimeout = ~( 0ull );

	/**
	*\name Typedefs généralistes.
	*/
	/**\{*/
	template< class Obj
		, class Key
		, class PtrType = std::shared_ptr< Obj >
		, typename Creator = std::function< PtrType() >
		, class Predicate = std::less< Key > >
	class Factory;

	template< typename T >
	using BufferPtr = std::unique_ptr< Buffer< T > >;
	template< typename T >
	using PushConstantsBufferTPtr = std::unique_ptr< PushConstantsBuffer< T > >;
	template< typename T >
	using UniformBufferPtr = std::unique_ptr< UniformBuffer< T > >;
	template< typename T >
	using VertexBufferPtr = std::unique_ptr< VertexBuffer< T > >;
	template< typename T >
	using SpecialisationInfoPtr = std::unique_ptr< SpecialisationInfo< T > >;

	using BufferBasePtr = std::unique_ptr< BufferBase >;
	using BufferViewPtr = std::unique_ptr< BufferView >;
	using CommandBufferPtr = std::unique_ptr< CommandBuffer >;
	using CommandPoolPtr = std::unique_ptr< CommandPool >;
	using ComputePipelinePtr = std::unique_ptr< ComputePipeline >;
	using DescriptorPoolPtr = std::unique_ptr< DescriptorPool >;
	using DescriptorSetLayoutPtr = std::unique_ptr< DescriptorSetLayout >;
	using DescriptorSetPoolPtr = std::unique_ptr< DescriptorSetPool >;
	using DescriptorSetPtr = std::unique_ptr< DescriptorSet >;
	using EventPtr = std::unique_ptr< Event >;
	using FencePtr = std::unique_ptr< Fence >;
	using FrameBufferPtr = std::unique_ptr< FrameBuffer >;
	using GraphicsPipelinePtr = std::unique_ptr< GraphicsPipeline >;
	using ImagePtr = std::unique_ptr< Image >;
	using InstancePtr = std::unique_ptr< Instance >;
	using IWindowHandlePtr = std::unique_ptr< IWindowHandle >;
	using PhysicalDevicePtr = std::unique_ptr< PhysicalDevice >;
	using PipelineLayoutPtr = std::unique_ptr< PipelineLayout >;
	using PushConstantsBufferPtr = std::unique_ptr< PushConstantsBufferBase >;
	using QueryPoolPtr = std::unique_ptr< QueryPool >;
	using QueuePtr = std::unique_ptr< Queue >;
	using RenderPassPtr = std::unique_ptr< RenderPass >;
	using SamplerPtr = std::unique_ptr< Sampler >;
	using SemaphorePtr = std::unique_ptr< Semaphore >;
	using StagingBufferPtr = std::unique_ptr< StagingBuffer >;
	using StagingTexturePtr = std::unique_ptr< StagingTexture >;
	using SurfacePtr = std::unique_ptr< Surface >;
	using SwapChainPtr = std::unique_ptr< SwapChain >;
	using VertexBufferBasePtr = std::unique_ptr< VertexBufferBase >;
	using UniformBufferBasePtr = std::unique_ptr< UniformBufferBase >;

	using DevicePtr = std::shared_ptr< Device >;
	using ShaderModulePtr = std::shared_ptr< ShaderModule >;
	using SpecialisationInfoBasePtr = std::shared_ptr< SpecialisationInfoBase >;
	using DeviceMemoryPtr = std::shared_ptr< DeviceMemory >;

	using PushConstantArray = std::vector< PushConstant >;
	using ImageArray = std::vector< Image >;
	using ImageViewArray = std::vector< ImageView >;

	using FrameBufferPtrArray = std::vector< FrameBufferPtr >;
	using CommandBufferPtrArray = std::vector< CommandBufferPtr >;
	using PhysicalDevicePtrArray = std::vector< PhysicalDevicePtr >;
	using ImagePtrArray = std::vector< ImagePtr >;

	using BufferCRef = std::reference_wrapper< BufferBase const >;
	using BufferViewCRef = std::reference_wrapper< BufferView const >;
	using CommandBufferCRef = std::reference_wrapper< CommandBuffer const >;
	using DescriptorSetCRef = std::reference_wrapper< DescriptorSet const >;
	using DescriptorSetLayoutCRef = std::reference_wrapper< DescriptorSetLayout const >;
	using EventCRef = std::reference_wrapper< Event const >;
	using FrameBufferCRef = std::reference_wrapper< FrameBuffer const >;
	using ImageViewCRef = std::reference_wrapper< ImageView const >;
	using PhysicalDeviceCRef = std::reference_wrapper< PhysicalDevice const >;
	using PushConstantRangeCRef = std::reference_wrapper< VkPushConstantRange const >;
	using RenderPassCRef = std::reference_wrapper< RenderPass const >;
	using SamplerCRef = std::reference_wrapper< Sampler const >;
	using SemaphoreCRef = std::reference_wrapper< Semaphore const >;
	using SwapChainCRef = std::reference_wrapper< SwapChain const >;
	using SurfaceCRef = std::reference_wrapper< Surface const >;
	using ImageViewCRef = std::reference_wrapper< ImageView const >;
	using VertexBufferCRef = std::reference_wrapper< VertexBufferBase const >;

	using BufferCRefArray = std::vector< BufferCRef >;
	using CommandBufferCRefArray = std::vector< CommandBufferCRef >;
	using DescriptorSetCRefArray = std::vector< DescriptorSetCRef >;
	using DescriptorSetLayoutCRefArray = std::vector< DescriptorSetLayoutCRef >;
	using EventCRefArray = std::vector< EventCRef >;
	using ImageViewCRefArray = std::vector< ImageViewCRef >;
	using SemaphoreCRefArray = std::vector< SemaphoreCRef >;
	using SwapChainCRefArray = std::vector< SwapChainCRef >;
	using ImageViewCRefArray = std::vector< ImageViewCRef >;
	using VertexBufferCRefArray = std::vector< VertexBufferCRef >;
	/**\}*/
}

#endif
