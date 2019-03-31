/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <Ashes/AshesConfig.hpp>

#include "TestRenderer/TestRendererConfig.hpp"

#ifdef min
#	undef min
#	undef max
#	undef abs
#endif

#include <Ashes/AshesPrerequisites.hpp>

#include <list>

namespace test_renderer
{
	static const std::string ShortName = "TestLib";

	class Attribute;
	class Buffer;
	class BufferView;
	class CommandBuffer;
	class CommandPool;
	class ComputePipeline;
	class DescriptorPool;
	class DescriptorSet;
	class DescriptorSetLayout;
	class DescriptorSetLayoutBinding;
	class Device;
	class Pipeline;
	class PipelineLayout;
	class PhysicalDevice;
	class QueryPool;
	class Queue;
	class Instance;
	class RenderPass;
	class RenderSubpass;
	class Sampler;
	class Semaphore;
	class ShaderProgram;
	class Surface;
	class SwapChain;
	class Image;
	class ImageView;
	class UniformBuffer;
	class VertexBufferBase;
	class VertexLayout;

	using AttributeArray = std::vector< Attribute >;

	using CommandPoolPtr = std::unique_ptr< CommandPool >;
	using PhysicalDevicePtr = std::unique_ptr< PhysicalDevice >;
	using QueuePtr = std::unique_ptr< Queue >;
	using RenderSubpassPtr = std::unique_ptr< RenderSubpass >;
	using SurfacePtr = std::unique_ptr< Surface >;
	using ImageViewPtr = std::unique_ptr< ImageView >;

	using RenderSubpassPtrArray = std::vector< RenderSubpassPtr >;

	using CommandBufferCRef = std::reference_wrapper< CommandBuffer const >;
	using DescriptorSetCRef = std::reference_wrapper< DescriptorSet const >;
	using DescriptorSetLayoutCRef = std::reference_wrapper< DescriptorSetLayout const >;
	using RenderSubpassCRef = std::reference_wrapper< RenderSubpass const >;
	using SemaphoreCRef = std::reference_wrapper< Semaphore const >;
	using SwapChainCRef = std::reference_wrapper< SwapChain const >;
	using TextureCRef = std::reference_wrapper< Image const >;
	using ImageViewCRef = std::reference_wrapper< ImageView const >;
	using VertexLayoutCRef = std::reference_wrapper< VertexLayout const >;
	using VertexBufferCRef = std::reference_wrapper< VertexBufferBase const >;

	using CommandBufferCRefArray = std::vector< CommandBufferCRef >;
	using DescriptorSetCRefArray = std::vector< DescriptorSetCRef >;
	using DescriptorSetLayoutCRefArray = std::vector< DescriptorSetLayoutCRef >;
	using RenderSubpassCRefArray = std::vector< RenderSubpassCRef >;
	using SemaphoreCRefArray = std::vector< SemaphoreCRef >;
	using SwapChainCRefArray = std::vector< SwapChainCRef >;
	using TextureCRefArray = std::vector< TextureCRef >;
	using ImageViewCRefArray = std::vector< ImageViewCRef >;
	using VertexLayoutCRefArray = std::vector< VertexLayoutCRef >;
	using VertexBufferCRefArray = std::vector< VertexBufferCRef >;

	uint32_t deduceMemoryType( uint32_t typeBits
		, ashes::MemoryPropertyFlags requirements );
}
