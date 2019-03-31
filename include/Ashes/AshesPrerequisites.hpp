/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_Prerequisites_HPP___
#define ___Ashes_Prerequisites_HPP___
#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#define Ashes_EnumBounds( MinValue )\
	eCount,\
	eMin = MinValue,\
	eMax = eCount - 1

#define Ashes_EnumBoundsEx( MinValue, MaxValue )\
	eCount,\
	eMin = MinValue,\
	eMax = eCount - 1

#include "Ashes/AshesConfig.hpp"

#if ASHES_WIN32
#	define ASHES_API __stdcall
#else
#	define ASHES_API
#endif

#include "Ashes/Utils/FlagCombination.hpp"
#include "Ashes/Utils/Signal.hpp"

#include "Ashes/Enum/AccessFlag.hpp"
#include "Ashes/Enum/AttachmentLoadOp.hpp"
#include "Ashes/Enum/AttachmentStoreOp.hpp"
#include "Ashes/Enum/BlendFactor.hpp"
#include "Ashes/Enum/BlendOp.hpp"
#include "Ashes/Enum/BorderColour.hpp"
#include "Ashes/Enum/BufferTarget.hpp"
#include "Ashes/Enum/ClipDirection.hpp"
#include "Ashes/Enum/ColorSpace.hpp"
#include "Ashes/Enum/ComponentSwizzle.hpp"
#include "Ashes/Enum/CompositeAlphaFlag.hpp"
#include "Ashes/Enum/ConstantFormat.hpp"
#include "Ashes/Enum/ColourComponentFlag.hpp"
#include "Ashes/Enum/CommandBufferResetFlag.hpp"
#include "Ashes/Enum/CommandBufferUsageFlag.hpp"
#include "Ashes/Enum/CommandPoolCreateFlag.hpp"
#include "Ashes/Enum/CompareOp.hpp"
#include "Ashes/Enum/CullModeFlag.hpp"
#include "Ashes/Enum/DebugReportFlag.hpp"
#include "Ashes/Enum/DebugReportObjectType.hpp"
#include "Ashes/Enum/DependencyFlag.hpp"
#include "Ashes/Enum/DepthStencilStateFlag.hpp"
#include "Ashes/Enum/DescriptorPoolCreateFlag.hpp"
#include "Ashes/Enum/DescriptorType.hpp"
#include "Ashes/Enum/DeviceCreateFlag.hpp"
#include "Ashes/Enum/DeviceQueueCreateFlag.hpp"
#include "Ashes/Enum/DynamicStateEnable.hpp"
#include "Ashes/Enum/DynamicStateFlag.hpp"
#include "Ashes/Enum/FenceCreateFlag.hpp"
#include "Ashes/Enum/Filter.hpp"
#include "Ashes/Enum/Format.hpp"
#include "Ashes/Enum/FormatFeatureFlag.hpp"
#include "Ashes/Enum/FrontFace.hpp"
#include "Ashes/Enum/ImageAspectFlag.hpp"
#include "Ashes/Enum/ImageCreateFlag.hpp"
#include "Ashes/Enum/ImageLayout.hpp"
#include "Ashes/Enum/ImageTiling.hpp"
#include "Ashes/Enum/ImageUsageFlag.hpp"
#include "Ashes/Enum/IndexType.hpp"
#include "Ashes/Enum/InstanceCreateFlag.hpp"
#include "Ashes/Enum/LogicOp.hpp"
#include "Ashes/Enum/MemoryHeapFlag.hpp"
#include "Ashes/Enum/MemoryMapFlag.hpp"
#include "Ashes/Enum/MemoryPropertyFlag.hpp"
#include "Ashes/Enum/MipmapMode.hpp"
#include "Ashes/Enum/MultisampleStateFlag.hpp"
#include "Ashes/Enum/PhysicalDeviceType.hpp"
#include "Ashes/Enum/PipelineBindPoint.hpp"
#include "Ashes/Enum/PipelineStageFlag.hpp"
#include "Ashes/Enum/PolygonMode.hpp"
#include "Ashes/Enum/PresentMode.hpp"
#include "Ashes/Enum/PrimitiveTopology.hpp"
#include "Ashes/Enum/QueryControlFlag.hpp"
#include "Ashes/Enum/QueryPipelineStatisticFlag.hpp"
#include "Ashes/Enum/QueryResultFlag.hpp"
#include "Ashes/Enum/QueryType.hpp"
#include "Ashes/Enum/QueueFlag.hpp"
#include "Ashes/Enum/RasterisationStateFlag.hpp"
#include "Ashes/Enum/RenderPassCreateFlag.hpp"
#include "Ashes/Enum/Result.hpp"
#include "Ashes/Enum/SampleCountFlag.hpp"
#include "Ashes/Enum/ShaderStageFlag.hpp"
#include "Ashes/Enum/SharingMode.hpp"
#include "Ashes/Enum/StencilOp.hpp"
#include "Ashes/Enum/SubpassContents.hpp"
#include "Ashes/Enum/SubpassDescriptionFlag.hpp"
#include "Ashes/Enum/SurfaceCounterFlag.hpp"
#include "Ashes/Enum/SurfaceTransformFlag.hpp"
#include "Ashes/Enum/SwapChainCreateFlag.hpp"
#include "Ashes/Enum/TessellationStateFlag.hpp"
#include "Ashes/Enum/ImageType.hpp"
#include "Ashes/Enum/ImageViewType.hpp"
#include "Ashes/Enum/VertexInputRate.hpp"
#include "Ashes/Enum/ViewportStateFlag.hpp"
#include "Ashes/Enum/WrapMode.hpp"

#include <cassert>
#include <cstring>
#include <ctime>
#include <functional>
#include <iomanip>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

#if defined( __GNUG__ )
#	define ASHES_COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#	if ASHES_COMPILER_VERSION < 40900
#		error "Unsupported version of GCC"
#	elif ASHES_COMPILER_VERSION < 70400
#		include <experimental/optional>
namespace ashes
{
	template< typename T >
	using Optional = std::experimental::optional< T >;
	using std::experimental::nullopt;
}
#	else
#		include <optional>
namespace ashes
{
	template< typename T >
	using Optional = std::optional< T >;
	using std::nullopt;
}
#	endif
#else
#	include <optional>
namespace ashes
{
	template< typename T >
	using Optional = std::optional< T >;
	using std::nullopt;
}
#	if defined( MemoryBarrier )
#		undef MemoryBarrier
#	endif
#endif

#include "Ashes/Miscellaneous/Log.hpp"


namespace ashes
{
	static char const * const KHR_SURFACE_EXTENSION_NAME = "VK_KHR_surface";

	static char const * const KHR_WIN32_SURFACE_EXTENSION_NAME = "VK_KHR_win32_surface";
	static char const * const KHR_XCB_SURFACE_EXTENSION_NAME = "VK_KHR_xcb_surface";
	static char const * const KHR_XLIB_SURFACE_EXTENSION_NAME = "VK_KHR_xlib_surface";
	static char const * const KHR_ANDROID_SURFACE_EXTENSION_NAME = "VK_KHR_android_surface";
	static char const * const KHR_MIR_SURFACE_EXTENSION_NAME = "VK_KHR_mir_surface";
	static char const * const KHR_WAYLAND_SURFACE_EXTENSION_NAME = "VK_KHR_wayland_surface";

	static char const * const EXT_DEBUG_REPORT_EXTENSION_NAME = "VK_EXT_debug_report";
	static char const * const EXT_DEBUG_MARKER_EXTENSION_NAME = "VK_EXT_debug_marker";

	static char const * const KHR_SWAPCHAIN_EXTENSION_NAME = "VK_KHR_swapchain";

	static char const * const KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME = "VK_KHR_sampler_ycbcr_conversion";
	static char const * const IMG_FORMAT_PVRTC_EXTENSION_NAME = "VK_IMG_format_pvrtc";

#if ASHES_WIN32
	static char const * const KHR_PLATFORM_SURFACE_EXTENSION_NAME = KHR_WIN32_SURFACE_EXTENSION_NAME;
#elif ASHES_XCB
	static char const * const KHR_PLATFORM_SURFACE_EXTENSION_NAME = KHR_XCB_SURFACE_EXTENSION_NAME;
#elif ASHES_XLIB
	static char const * const KHR_PLATFORM_SURFACE_EXTENSION_NAME = KHR_XLIB_SURFACE_EXTENSION_NAME;
#elif ASHES_ANDROID
	static char const * const KHR_PLATFORM_SURFACE_EXTENSION_NAME = KHR_ANDROID_SURFACE_EXTENSION_NAME;
#elif ASHES_MIR
	static char const * const KHR_PLATFORM_SURFACE_EXTENSION_NAME = KHR_MIR_SURFACE_EXTENSION_NAME;
#elif ASHES_WAYLAND
	static char const * const KHR_PLATFORM_SURFACE_EXTENSION_NAME = KHR_WAYLAND_SURFACE_EXTENSION_NAME;
#endif

	inline constexpr uint32_t getMajor( uint32_t version )
	{
		return ( ( uint32_t )( version ) >> 22 );
	}

	inline constexpr uint32_t getMinor( uint32_t version )
	{
		return ( ( ( uint32_t )( version ) >> 12 ) & 0x3ff );
	}

	inline constexpr uint32_t getPatch( uint32_t version )
	{
		return ( ( uint32_t )( version ) & 0xfff );
	}

	inline constexpr uint32_t makeVersion( uint32_t major
		, uint32_t minor
		, uint32_t patch )
	{
		return ( ( ( major ) << 22 ) | ( ( minor ) << 12 ) | ( patch ) );
	}

	static uint32_t constexpr HEADER_VERSION = 70;
	static uint32_t constexpr API_VERSION_1_0 = makeVersion( 1, 0, 0 );


	using ByteArray = std::vector< uint8_t >;
	using UInt16Array = std::vector< uint16_t >;
	using UInt32Array = std::vector< uint32_t >;
	using UInt64Array = std::vector< uint64_t >;
	using FloatArray = std::vector< float >;
	using StringArray = std::vector< std::string >;
	using DeviceSize = uint64_t;

	static DeviceSize constexpr WholeSize = ~( 0ull );

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

	union ClearColorValue;

	struct AttachmentDescription;
	struct AttachmentReference;
	struct BufferCopy;
	struct BufferImageCopy;
	struct BufferMemoryBarrier;
	struct ClearAttachment;
	struct ClearRect;
	struct ClearValue;
	struct ColourBlendState;
	struct ColourBlendStateAttachment;
	struct CommandBufferInheritanceInfo;
	struct DepthStencilState;
	struct DescriptorBufferInfo;
	struct DescriptorImageInfo;
	struct DescriptorPoolSize;
	struct DeviceQueueCreateInfo;
	struct ExtensionProperties;
	struct Extent2D;
	struct Extent3D;
	struct FormatProperties;
	struct GraphicsPipelineCreateInfo;
	struct ImageBlit;
	struct ImageCopy;
	struct ImageCreateInfo;
	struct ImageFormatProperties;
	struct ImageMemoryBarrier;
	struct ImageSubresource;
	struct ImageSubresourceLayers;
	struct ImageSubresourceRange;
	struct InputAssemblyState;
	struct LayerProperties;
	struct MemoryBarrier;
	struct MemoryHeap;
	struct MemoryRequirements;
	struct MemoryType;
	struct MultisampleState;
	struct Offset2D;
	struct Offset3D;
	struct PhysicalDeviceFeatures;
	struct PhysicalDeviceLimits;
	struct PhysicalDeviceMemoryProperties;
	struct PhysicalDeviceProperties;
	struct PhysicalDeviceSparseProperties;
	struct PushConstant;
	struct PushConstantRange;
	struct QueueFamilyProperties;
	struct RasterisationState;
	struct RenderPassCreateInfo;
	struct RenderSubpassState;
	struct Scissor;
	struct ShaderStageState;
	struct SpecialisationMapEntry;
	struct StencilOpState;
	struct SubpassDependency;
	struct SubpassDescription;
	struct SubresourceLayout;
	struct TessellationState;
	struct VertexInputAttributeDescription;
	struct VertexInputBindingDescription;
	struct VertexInputState;
	struct Viewport;
	struct WriteDescriptorSet;

	class Attribute;
	class BufferBase;
	class BufferView;
	class CommandBuffer;
	class CommandPool;
	class ComputePipeline;
	class DebugReportCallback;
	class DescriptorPool;
	class DescriptorSet;
	class DescriptorSetLayout;
	class DescriptorSetLayoutBinding;
	class DescriptorSetPool;
	class Device;
	class DeviceMemory;
	class Event;
	class Fence;
	class FrameBuffer;
	class FrameBufferAttachment;
	class Image;
	class ImageView;
	class Instance;
	class IWindowHandle;
	class PhysicalDevice;
	class Pipeline;
	class PipelineLayout;
	class PushConstantsBufferBase;
	class QueryPool;
	class Queue;
	class RenderPass;
	class RenderSubpass;
	class Sampler;
	class Semaphore;
	class ShaderModule;
	class ShaderStorageBufferBase;
	class SpecialisationInfoBase;
	class StagingBuffer;
	class StagingTexture;
	class Surface;
	class SwapChain;
	class UniformBufferBase;
	class VertexBufferBase;
	class VertexLayout;
	class WindowHandle;

	/**
	*\~french
	*\brief
	*	Temps en nanosecondes à attendre pour qu'un tampon de commandes soit exécuté.
	*\~english
	*\brief
	*	Nanoseconds time to wait for a command buffer to be executed.
	*/
	static const uint64_t FenceTimeout = ~( 0ull );

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

	using AttributeBasePtr = std::unique_ptr< Attribute >;
	using BufferBasePtr = std::unique_ptr< BufferBase >;
	using BufferViewPtr = std::unique_ptr< BufferView >;
	using CommandBufferPtr = std::unique_ptr< CommandBuffer >;
	using CommandPoolPtr = std::unique_ptr< CommandPool >;
	using ComputePipelinePtr = std::unique_ptr< ComputePipeline >;
	using DebugReportCallbackPtr = std::unique_ptr< DebugReportCallback >;
	using DescriptorPoolPtr = std::unique_ptr< DescriptorPool >;
	using DescriptorSetLayoutPtr = std::unique_ptr< DescriptorSetLayout >;
	using DescriptorSetLayoutBindingPtr = std::unique_ptr< DescriptorSetLayoutBinding >;
	using DescriptorSetPoolPtr = std::unique_ptr< DescriptorSetPool >;
	using DescriptorSetPtr = std::unique_ptr< DescriptorSet >;
	using EventPtr = std::unique_ptr< Event >;
	using FencePtr = std::unique_ptr< Fence >;
	using FrameBufferPtr = std::unique_ptr< FrameBuffer >;
	using InstancePtr = std::unique_ptr< Instance >;
	using IWindowHandlePtr = std::unique_ptr< IWindowHandle >;
	using PhysicalDevicePtr = std::unique_ptr< PhysicalDevice >;
	using PipelinePtr = std::unique_ptr< Pipeline >;
	using PipelineLayoutPtr = std::unique_ptr< PipelineLayout >;
	using PushConstantsBufferPtr = std::unique_ptr< PushConstantsBufferBase >;
	using QueryPoolPtr = std::unique_ptr< QueryPool >;
	using QueuePtr = std::unique_ptr< Queue >;
	using RenderPassPtr = std::unique_ptr< RenderPass >;
	using RenderSubpassPtr = std::unique_ptr< RenderSubpass >;
	using SamplerPtr = std::unique_ptr< Sampler >;
	using SemaphorePtr = std::unique_ptr< Semaphore >;
	using ShaderStorageBufferBasePtr = std::unique_ptr< ShaderStorageBufferBase >;
	using StagingBufferPtr = std::unique_ptr< StagingBuffer >;
	using StagingTexturePtr = std::unique_ptr< StagingTexture >;
	using SurfacePtr = std::unique_ptr< Surface >;
	using SwapChainPtr = std::unique_ptr< SwapChain >;
	using ImagePtr = std::unique_ptr< Image >;
	using VertexBufferBasePtr = std::unique_ptr< VertexBufferBase >;
	using VertexLayoutPtr = std::unique_ptr< VertexLayout >;
	using UniformBufferBasePtr = std::unique_ptr< UniformBufferBase >;

	using DevicePtr = std::shared_ptr< Device >;
	using DeviceMemoryPtr = std::shared_ptr< DeviceMemory >;
	using ImageViewPtr = std::shared_ptr< ImageView >;
	using ShaderModulePtr = std::shared_ptr< ShaderModule >;
	using SpecialisationInfoBasePtr = std::shared_ptr< SpecialisationInfoBase >;

	using AttachmentDescriptionArray = std::vector< AttachmentDescription >;
	using AttachmentReferenceArray = std::vector< AttachmentReference >;
	using AttributeArray = std::vector< Attribute >;
	using BufferImageCopyArray = std::vector< BufferImageCopy >;
	using BufferMemoryBarrierArray = std::vector< BufferMemoryBarrier >;
	using ClearAttachmentArray = std::vector< ClearAttachment >;
	using ClearRectArray = std::vector< ClearRect >;
	using ClearValueArray = std::vector< ClearValue >;
	using ColourBlendStateAttachmentArray = std::vector< ColourBlendStateAttachment >;
	using DescriptorPoolSizeArray = std::vector< DescriptorPoolSize >;
	using DescriptorSetLayoutBindingArray = std::vector< DescriptorSetLayoutBinding >;
	using DeviceQueueCreateInfoArray = std::vector< DeviceQueueCreateInfo >;
	using DynamicStateEnableArray = std::vector< DynamicStateEnable >;
	using ExtensionPropertiesArray = std::vector< ExtensionProperties >;
	using FrameBufferAttachmentArray = std::vector< FrameBufferAttachment >;
	using ImageLayoutArray = std::vector< ImageLayout >;
	using ImageMemoryBarrierArray = std::vector< ImageMemoryBarrier >;
	using LayerPropertiesArray = std::vector< LayerProperties >;
	using MemoryBarrierArray = std::vector< MemoryBarrier >;
	using PipelineStageFlagsArray = std::vector< PipelineStageFlags >;
	using PushConstantArray = std::vector< PushConstant >;
	using PushConstantRangeArray = std::vector< PushConstantRange >;
	using QueueFamilyPropertiesArray = std::vector< QueueFamilyProperties >;
	using RenderSubpassArray = std::vector< RenderSubpass >;
	using ScissorArray = std::vector< Scissor >;
	using ShaderStageStateArray = std::vector< ShaderStageState >;
	using SpecialisationMapEntryArray = std::vector< SpecialisationMapEntry >;
	using SubpassDescriptionArray = std::vector< SubpassDescription >;
	using SubpassDependencyArray = std::vector< SubpassDependency >;
	using VertexInputAttributeDescriptionArray = std::vector< VertexInputAttributeDescription >;
	using VertexInputBindingDescriptionArray = std::vector< VertexInputBindingDescription >;
	using ViewportArray = std::vector< Viewport >;
	using WriteDescriptorSetArray = std::vector< WriteDescriptorSet >;

	using FrameBufferPtrArray = std::vector< FrameBufferPtr >;
	using CommandBufferPtrArray = std::vector< CommandBufferPtr >;
	using ImagePtrArray = std::vector< ImagePtr >;
	using ImageViewPtrArray = std::vector< ImageViewPtr >;
	using RenderSubpassPtrArray = std::vector< RenderSubpassPtr >;
	using PhysicalDevicePtrArray = std::vector< PhysicalDevicePtr >;

	using BufferCRef = std::reference_wrapper< BufferBase const >;
	using BufferViewCRef = std::reference_wrapper< BufferView const >;
	using CommandBufferCRef = std::reference_wrapper< CommandBuffer const >;
	using DescriptorSetCRef = std::reference_wrapper< DescriptorSet const >;
	using DescriptorSetLayoutCRef = std::reference_wrapper< DescriptorSetLayout const >;
	using EventCRef = std::reference_wrapper< Event const >;
	using FrameBufferCRef = std::reference_wrapper< FrameBuffer const >;
	using PhysicalDeviceCRef = std::reference_wrapper< PhysicalDevice const >;
	using PushConstantRangeCRef = std::reference_wrapper< PushConstantRange const >;
	using RenderPassCRef = std::reference_wrapper< RenderPass const >;
	using SamplerCRef = std::reference_wrapper< Sampler const >;
	using SemaphoreCRef = std::reference_wrapper< Semaphore const >;
	using SwapChainCRef = std::reference_wrapper< SwapChain const >;
	using SurfaceCRef = std::reference_wrapper< Surface const >;
	using ImageViewCRef = std::reference_wrapper< ImageView const >;
	using VertexLayoutCRef = std::reference_wrapper< VertexLayout const >;
	using VertexBufferCRef = std::reference_wrapper< VertexBufferBase const >;

	using BufferCRefArray = std::vector< BufferCRef >;
	using CommandBufferCRefArray = std::vector< CommandBufferCRef >;
	using DescriptorSetCRefArray = std::vector< DescriptorSetCRef >;
	using DescriptorSetLayoutCRefArray = std::vector< DescriptorSetLayoutCRef >;
	using EventCRefArray = std::vector< EventCRef >;
	using SemaphoreCRefArray = std::vector< SemaphoreCRef >;
	using SwapChainCRefArray = std::vector< SwapChainCRef >;
	using ImageViewCRefArray = std::vector< ImageViewCRef >;
	using VertexLayoutCRefArray = std::vector< VertexLayoutCRef >;
	using VertexBufferCRefArray = std::vector< VertexBufferCRef >;
	/**\}*/
	/**
	*\name Signaux.
	*/
	/**\{*/
	using DeviceEnabledFunction = std::function< void( Device const & ) >;
	using DeviceDisabledFunction = std::function< void( Device const & ) >;

	using DeviceEnabledSignal = Signal< DeviceEnabledFunction >;
	using DeviceDisabledSignal = Signal< DeviceDisabledFunction >;

	using DeviceEnabledConnection = SignalConnection< DeviceEnabledSignal >;
	using DeviceDisabledConnection = SignalConnection< DeviceDisabledSignal >;
	/**\}*/
}

#ifndef NDEBUG
#	define declareDebugVariable( Type, Name, Value )\
	mutable Type Name{ Value }
#	define setDebugValue( Name, Value )\
	Name = Value;
#	define assertDebugValue( Name, Value )\
	assert( Name == Value )
#else
#	define declareDebugVariable( Type, Name, Value )
#	define setDebugValue( Name, Value )
#	define assertDebugValue( Name, Value )
#endif

#endif
