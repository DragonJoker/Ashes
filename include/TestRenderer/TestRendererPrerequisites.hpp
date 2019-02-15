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
	class BackBuffer;
	class Buffer;
	class BufferView;
	class CommandBuffer;
	class CommandPool;
	class ComputePipeline;
	class Connection;
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
	class Renderer;
	class RenderPass;
	class RenderSubpass;
	class Sampler;
	class Semaphore;
	class ShaderProgram;
	class SwapChain;
	class Texture;
	class TextureView;
	class UniformBuffer;
	class VertexBufferBase;
	class VertexLayout;

	using AttributeArray = std::vector< Attribute >;

	using BackBufferPtr = std::unique_ptr< BackBuffer >;
	using ConnectionPtr = std::unique_ptr< Connection >;
	using CommandPoolPtr = std::unique_ptr< CommandPool >;
	using PhysicalDevicePtr = std::unique_ptr< PhysicalDevice >;
	using QueuePtr = std::unique_ptr< Queue >;
	using RenderSubpassPtr = std::unique_ptr< RenderSubpass >;
	using TextureViewPtr = std::unique_ptr< TextureView >;

	using BackBufferPtrArray = std::vector< BackBufferPtr >;
	using RenderSubpassPtrArray = std::vector< RenderSubpassPtr >;

	using CommandBufferCRef = std::reference_wrapper< CommandBuffer const >;
	using DescriptorSetCRef = std::reference_wrapper< DescriptorSet const >;
	using DescriptorSetLayoutCRef = std::reference_wrapper< DescriptorSetLayout const >;
	using RenderSubpassCRef = std::reference_wrapper< RenderSubpass const >;
	using SemaphoreCRef = std::reference_wrapper< Semaphore const >;
	using SwapChainCRef = std::reference_wrapper< SwapChain const >;
	using TextureCRef = std::reference_wrapper< Texture const >;
	using TextureViewCRef = std::reference_wrapper< TextureView const >;
	using VertexLayoutCRef = std::reference_wrapper< VertexLayout const >;
	using VertexBufferCRef = std::reference_wrapper< VertexBufferBase const >;

	using CommandBufferCRefArray = std::vector< CommandBufferCRef >;
	using DescriptorSetCRefArray = std::vector< DescriptorSetCRef >;
	using DescriptorSetLayoutCRefArray = std::vector< DescriptorSetLayoutCRef >;
	using RenderSubpassCRefArray = std::vector< RenderSubpassCRef >;
	using SemaphoreCRefArray = std::vector< SemaphoreCRef >;
	using SwapChainCRefArray = std::vector< SwapChainCRef >;
	using TextureCRefArray = std::vector< TextureCRef >;
	using TextureViewCRefArray = std::vector< TextureViewCRef >;
	using VertexLayoutCRefArray = std::vector< VertexLayoutCRef >;
	using VertexBufferCRefArray = std::vector< VertexBufferCRef >;
}
