/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Prerequisites_HPP___
#define ___AshesPP_Prerequisites_HPP___
#pragma once

#include <ashes/ashes.hpp>

#include "Enum/AccelerationStructureBuildType.hpp"
#include "Enum/AccelerationStructureCompatibility.hpp"
#include "Enum/AccelerationStructureCreateFlags.hpp"
#include "Enum/AccessFlags.hpp"
#include "Enum/AttachmentLoadOp.hpp"
#include "Enum/AttachmentStoreOp.hpp"
#include "Enum/BlendFactor.hpp"
#include "Enum/BlendOp.hpp"
#include "Enum/BorderColor.hpp"
#include "Enum/BufferCreateFlags.hpp"
#include "Enum/BufferUsageFlags.hpp"
#include "Enum/BuildAccelerationStructureMode.hpp"
#include "Enum/ClipDirection.hpp"
#include "Enum/ColorComponentFlags.hpp"
#include "Enum/ColorSpace.hpp"
#include "Enum/CommandBufferResetFlags.hpp"
#include "Enum/CommandBufferUsageFlags.hpp"
#include "Enum/CommandPoolCreateFlags.hpp"
#include "Enum/CompareOp.hpp"
#include "Enum/ComponentSwizzle.hpp"
#include "Enum/CompositeAlphaFlags.hpp"
#include "Enum/ConstantFormat.hpp"
#include "Enum/CullModeFlags.hpp"
#include "Enum/DebugReportFlags.hpp"
#include "Enum/DebugReportObjectType.hpp"
#include "Enum/DependencyFlags.hpp"
#include "Enum/DescriptorPoolCreateFlags.hpp"
#include "Enum/DescriptorSetLayoutCreateFlags.hpp"
#include "Enum/DeviceCreateFlags.hpp"
#include "Enum/DeviceQueueCreateFlags.hpp"
#include "Enum/DynamicStateEnable.hpp"
#include "Enum/FenceCreateFlags.hpp"
#include "Enum/Filter.hpp"
#include "Enum/FormatFeatureFlags.hpp"
#include "Enum/FramebufferCreateFlags.hpp"
#include "Enum/FrontFace.hpp"
#include "Enum/ImageAspectFlags.hpp"
#include "Enum/ImageCreateFlags.hpp"
#include "Enum/ImageLayout.hpp"
#include "Enum/ImageTiling.hpp"
#include "Enum/ImageType.hpp"
#include "Enum/ImageUsageFlags.hpp"
#include "Enum/ImageViewType.hpp"
#include "Enum/IndexType.hpp"
#include "Enum/InstanceCreateFlags.hpp"
#include "Enum/LogicOp.hpp"
#include "Enum/MemoryAllocateFlags.hpp"
#include "Enum/MemoryHeapFlags.hpp"
#include "Enum/MemoryMapFlags.hpp"
#include "Enum/MemoryPropertyFlags.hpp"
#include "Enum/ObjectType.hpp"
#include "Enum/PhysicalDeviceType.hpp"
#include "Enum/PipelineBindPoint.hpp"
#include "Enum/PipelineCreateFlags.hpp"
#include "Enum/PipelineDepthStencilStateCreateFlags.hpp"
#include "Enum/PipelineDynamicStateCreateFlags.hpp"
#include "Enum/PipelineMultisampleStateCreateFlags.hpp"
#include "Enum/PipelineRasterizationStateCreateFlags.hpp"
#include "Enum/PipelineStageFlags.hpp"
#include "Enum/PipelineTessellationStateCreateFlags.hpp"
#include "Enum/PipelineViewportStateCreateFlags.hpp"
#include "Enum/PolygonMode.hpp"
#include "Enum/PresentMode.hpp"
#include "Enum/PrimitiveTopology.hpp"
#include "Enum/QueryControlFlags.hpp"
#include "Enum/QueryPipelineStatisticFlags.hpp"
#include "Enum/QueryResultFlags.hpp"
#include "Enum/QueryType.hpp"
#include "Enum/QueueFlags.hpp"
#include "Enum/RenderPassCreateFlags.hpp"
#include "Enum/SampleCountFlags.hpp"
#include "Enum/SamplerAddressMode.hpp"
#include "Enum/SamplerMipmapMode.hpp"
#include "Enum/ShaderGroupShader.hpp"
#include "Enum/ShaderStageFlags.hpp"
#include "Enum/SharingMode.hpp"
#include "Enum/StencilOp.hpp"
#include "Enum/SubpassContents.hpp"
#include "Enum/SubpassDescriptionFlags.hpp"
#include "Enum/SurfaceCounterFlags.hpp"
#include "Enum/SurfaceTransformFlags.hpp"
#include "Enum/SwapChainCreateFlags.hpp"
#include "Enum/VertexInputRate.hpp"
#include "Enum/Flags.hpp"
#include "Miscellaneous/Debug.hpp"
#include "Miscellaneous/Error.hpp"
#include "Miscellaneous/Log.hpp"

#include <ashes/common/Format.hpp>

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

namespace ashes
{
	/**
	*\brief
	*	Possible returns while waiting for a fence.
	*/
	enum class WaitResult
	{
		eSuccess,
		eTimeOut,
		eError
	};

	template< typename T >
	class Buffer;
	template< typename T >
	class PushConstantsBuffer;
	template< typename T >
	class SpecialisationInfo;
	template< typename T >
	class VertexBuffer;

	struct PushConstant;
	struct ImageView;
	struct PipelineVertexInputStateCreateInfo;
	struct WriteDescriptorSet;

	class AccelerationStructure;
	class BufferBase;
	class BufferView;
	class CommandBuffer;
	class CommandPool;
	class ComputePipeline;
	class DeferredOperation;
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
	class Pipeline;
	class PipelineLayout;
	class PushConstantsBufferBase;
	class QueryPool;
	class Queue;
	class RayTracingPipeline;
	class RenderPass;
	class Sampler;
	class Semaphore;
	class ShaderModule;
	class SpecialisationInfoBase;
	class StagingBuffer;
	class StagingTexture;
	class Surface;
	class SwapChain;
	class UniformBuffer;
	class VertexBufferBase;

	/**
	*\brief
	*	Maximum time to wait in nanoseconds.
	*/
	static const uint64_t MaxTimeout = ~( 0ull );

	template< typename T >
	using BufferPtr = std::unique_ptr< Buffer< T > >;
	template< typename T >
	using PushConstantsBufferTPtr = std::unique_ptr< PushConstantsBuffer< T > >;
	template< typename T >
	using VertexBufferPtr = std::unique_ptr< VertexBuffer< T > >;
	template< typename T >
	using SpecialisationInfoPtr = std::unique_ptr< SpecialisationInfo< T > >;

	using AccelerationStructurePtr = std::unique_ptr< AccelerationStructure >;
	using BufferBasePtr = std::unique_ptr< BufferBase >;
	using BufferViewPtr = std::unique_ptr< BufferView >;
	using CommandBufferPtr = std::unique_ptr< CommandBuffer >;
	using CommandPoolPtr = std::unique_ptr< CommandPool >;
	using ComputePipelinePtr = std::unique_ptr< ComputePipeline >;
	using DeferredOperationPtr = std::unique_ptr< DeferredOperation >;
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
	using PipelinePtr = std::unique_ptr< Pipeline >;
	using PipelineLayoutPtr = std::unique_ptr< PipelineLayout >;
	using PipelineVertexInputStateCreateInfoPtr = std::unique_ptr< PipelineVertexInputStateCreateInfo >;
	using PushConstantsBufferPtr = std::unique_ptr< PushConstantsBufferBase >;
	using QueryPoolPtr = std::unique_ptr< QueryPool >;
	using QueuePtr = std::unique_ptr< Queue >;
	using RayTracingPipelinePtr = std::unique_ptr< RayTracingPipeline >;
	using RenderPassPtr = std::unique_ptr< RenderPass >;
	using SamplerPtr = std::unique_ptr< Sampler >;
	using SemaphorePtr = std::unique_ptr< Semaphore >;
	using StagingBufferPtr = std::unique_ptr< StagingBuffer >;
	using StagingTexturePtr = std::unique_ptr< StagingTexture >;
	using SurfacePtr = std::unique_ptr< Surface >;
	using SwapChainPtr = std::unique_ptr< SwapChain >;
	using VertexBufferBasePtr = std::unique_ptr< VertexBufferBase >;
	using UniformBufferPtr = std::unique_ptr< UniformBuffer >;

	using DevicePtr = std::shared_ptr< Device >;
	using ShaderModulePtr = std::shared_ptr< ShaderModule >;
	using SpecialisationInfoBasePtr = std::shared_ptr< SpecialisationInfoBase >;
	using DeviceMemoryPtr = std::shared_ptr< DeviceMemory >;

	using PushConstantArray = std::vector< PushConstant >;
	using ImageArray = std::vector< Image >;
	using ImageViewArray = std::vector< ImageView >;
	using WriteDescriptorSetArray = std::vector< WriteDescriptorSet >;

	using FrameBufferPtrArray = std::vector< FrameBufferPtr >;
	using CommandBufferPtrArray = std::vector< CommandBufferPtr >;
	using PhysicalDevicePtrArray = std::vector< PhysicalDevicePtr >;
	using ImagePtrArray = std::vector< ImagePtr >;

	using AccelerationStructureCRef = std::reference_wrapper< AccelerationStructure const >;
	using BufferCRef = std::reference_wrapper< BufferBase const >;
	using BufferViewCRef = std::reference_wrapper< BufferView const >;
	using CommandBufferCRef = std::reference_wrapper< CommandBuffer const >;
	using DescriptorSetCRef = std::reference_wrapper< DescriptorSet const >;
	using DescriptorSetLayoutCRef = std::reference_wrapper< DescriptorSetLayout const >;
	using EventCRef = std::reference_wrapper< Event const >;
	using FrameBufferCRef = std::reference_wrapper< FrameBuffer const >;
	using ImageViewCRef = std::reference_wrapper< ImageView const >;
	using PhysicalDeviceCRef = std::reference_wrapper< PhysicalDevice const >;
	using RenderPassCRef = std::reference_wrapper< RenderPass const >;
	using SamplerCRef = std::reference_wrapper< Sampler const >;
	using SemaphoreCRef = std::reference_wrapper< Semaphore const >;
	using SwapChainCRef = std::reference_wrapper< SwapChain const >;
	using SurfaceCRef = std::reference_wrapper< Surface const >;
	using ImageViewCRef = std::reference_wrapper< ImageView const >;
	using VertexBufferCRef = std::reference_wrapper< VertexBufferBase const >;
	using PipelineVertexInputStateCreateInfoCRef = std::reference_wrapper< PipelineVertexInputStateCreateInfo const >;

	using AccelerationStructureCRefArray = std::vector< AccelerationStructureCRef >;
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
	using PipelineVertexInputStateCreateInfoCRefArray = std::vector< PipelineVertexInputStateCreateInfoCRef >;

	template< typename SignalT >
	class SignalConnection;
	template< typename FunctionT >
	class Signal;

	using WaitEndFunction = std::function< void( Fence const &, WaitResult ) >;
	using OnWaitEndSignal = Signal< WaitEndFunction >;
	using OnWaitEndConnection = SignalConnection< OnWaitEndSignal >;

	struct DebugBlockInfo
	{
		std::string markerName;
		std::array< float, 4 > color;
	};

	struct DebugObjectNameInfo
	{
		VkObjectType objectType;
		uint64_t object;
		std::string objectName;
	};

	struct DebugObjectTagInfo
	{
		VkObjectType objectType;
		uint64_t objectHandle;
		uint64_t tagName;
		size_t tagSize;
		const void * pTag;
	};

	class VkObject
	{
	public:
		VkObject( std::string name )
			: m_name{ std::move( name ) }
		{
		}

		std::string const & getName()
		{
			return m_name;
		}

	private:
		std::string m_name;
	};

	inline std::string convert( char const * const v )noexcept
	{
		return v ? std::string{ v } : std::string{};
	}
}

#endif
