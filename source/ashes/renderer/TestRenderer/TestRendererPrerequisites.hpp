/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_Prerequisites_HPP___
#define ___TestRenderer_Prerequisites_HPP___
#pragma once

#include <ashes/ashes.hpp>

#include <renderer/RendererCommon/AshesRendererPrerequisites.hpp>
#include <renderer/RendererCommon/Helper/ConstantFormat.hpp>

#include <ashes/common/ArrayView.hpp>
#include <ashes/common/Format.hpp>
#include <ashes/common/FlagCombination.hpp>
#include <ashes/common/Signal.hpp>

#pragma warning( push )
#pragma warning( disable: 5262 )
#include <array>
#include <cassert>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>
#pragma warning( pop )

#if defined( _WIN32 ) && !defined( TestRenderer_STATIC )
#	ifdef TestRenderer_EXPORTS
#		define TestRenderer_API __declspec( dllexport )
#	else
#		define TestRenderer_API __declspec( dllimport )
#	endif
#else
#	define TestRenderer_API
#endif

namespace ashes::test
{
	struct DebugLabel
	{
		std::array< float, 4u > color;
		std::string labelName;
	};

	class ExtensionNotPresentException
		: public ashes::Exception
	{
	public:
		ExtensionNotPresentException( std::string const & name )
			: Exception{ VK_ERROR_EXTENSION_NOT_PRESENT, name }
		{
		}
	};

	class Attribute;
	class Buffer;
	class BufferView;
	class CommandBase;
	class CommandBuffer;
	class CommandPool;
	class DescriptorPool;
	class DescriptorSet;
	class DescriptorSetLayout;
	class DescriptorSetLayoutBinding;
	class Device;
	class DeviceMemory;
	class FrameBuffer;
	class GeometryBuffers;
	class ObjectMemory;
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
	class ShaderModule;
	class Surface;
	class SwapChain;
	class Image;
	class ImageView;
	class VertexBufferBase;
	class VertexLayout;

	class SamplerYcbcrConversion;
	class DescriptorUpdateTemplate;
	class DisplayKHR;
	class DisplayModeKHR;
	class ObjectTableNVX;
	class IndirectCommandsLayoutNVX;
	class DebugUtilsMessengerEXT;
	class ValidationCacheEXT;

	using Action = std::function< void() >;
	using ActionArray = std::vector< Action >;

	using AttributeArray = std::vector< Attribute >;

	using CommandPtr = std::unique_ptr< CommandBase >;
	using CommandPoolPtr = std::unique_ptr< CommandPool >;
	using PipelinePtr = std::unique_ptr< Pipeline >;
	using PhysicalDevicePtr = std::unique_ptr< PhysicalDevice >;
	using QueuePtr = std::unique_ptr< Queue >;
	using RenderSubpassPtr = std::unique_ptr< RenderSubpass >;
	using SurfacePtr = std::unique_ptr< Surface >;
	using ImageViewPtr = std::unique_ptr< ImageView >;

	using BufferCRef = std::reference_wrapper< Buffer const >;
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

	using CommandArray = std::vector< CommandPtr >;

	template< typename Dst, typename Src >
	std::vector< std::reference_wrapper< Dst const > > staticCast( std::vector< std::reference_wrapper< Src const > > const & src )
	{
		std::vector< std::reference_wrapper< Dst const > > result;

		for ( auto & s : src )
		{
			result.emplace_back( static_cast< Dst const & >( s.get() ) );
		}

		return result;
	}

	struct PushConstantsDesc
	{
		VkShaderStageFlags stageFlags{};
		uint32_t offset{};
		uint32_t size{};
		std::vector< uint8_t > data;
	};

	struct PushConstantsBuffer
	{
		VkBuffer ubo{};
		uint32_t location{};
		PushConstantsDesc data;
		VkDeviceMemory memory{};
	};

	struct VbosBinding
	{
		uint32_t startIndex{};
		std::vector< VkBuffer > buffers;
		std::vector< uint32_t > offsets;
		std::vector< uint32_t > strides;
	};

	using VbosBindingArray = std::vector< VbosBinding >;

	struct LayoutBindingWrites
	{
		VkDescriptorSetLayoutBinding binding;
		VkWriteDescriptorSetArray writes;
	};
	using LayoutBindingWritesArray = std::vector< LayoutBindingWrites * >;
	using LayoutBindingWritesMap = std::map< uint32_t, LayoutBindingWrites >;

	using DeviceMemoryDestroyFunc = std::function< void( VkDeviceMemory ) >;
	using DeviceMemoryDestroySignal = Signal< DeviceMemoryDestroyFunc >;
	using DeviceMemoryDestroyConnection = SignalConnection< DeviceMemoryDestroySignal >;
}

#endif
