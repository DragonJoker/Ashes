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
#include "Miscellaneous/Format.hpp"
#include "Miscellaneous/Extent2D.hpp"

#include <AshesCommon/PlatformWindowHandle.hpp>

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

#include "AshesPP/Miscellaneous/Log.hpp"

namespace ashes
{
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

	using ByteArray = std::vector< uint8_t >;
	using UInt16Array = std::vector< uint16_t >;
	using UInt32Array = std::vector< uint32_t >;
	using UInt64Array = std::vector< uint64_t >;
	using FloatArray = std::vector< float >;
	using StringArray = std::vector< std::string >;
	using CharPtrArray = std::vector< char const * >;
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

	struct PushConstant;

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
	class ImageView;
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
	using ImageViewPtr = std::unique_ptr< ImageView >;
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

	using AttachmentDescriptionArray = std::vector< VkAttachmentDescription >;
	using AttachmentReferenceArray = std::vector< VkAttachmentReference >;
	using BufferImageCopyArray = std::vector< VkBufferImageCopy >;
	using BufferMemoryBarrierArray = std::vector< VkBufferMemoryBarrier >;
	using ClearAttachmentArray = std::vector< VkClearAttachment >;
	using ClearRectArray = std::vector< VkClearRect >;
	using ClearValueArray = std::vector< VkClearValue >;
	using ColourBlendStateAttachmentArray = std::vector< VkPipelineColorBlendStateCreateInfo >;
	using DescriptorPoolSizeArray = std::vector< VkDescriptorPoolSize >;
	using DescriptorSetLayoutBindingArray = std::vector< VkDescriptorSetLayoutBinding >;
	using DeviceQueueCreateInfoArray = std::vector< VkDeviceQueueCreateInfo >;
	using DynamicStateEnableArray = std::vector< VkDynamicState >;
	using ExtensionPropertiesArray = std::vector< VkExtensionProperties >;
	using ImageLayoutArray = std::vector< VkImageLayout >;
	using ImageMemoryBarrierArray = std::vector< VkImageMemoryBarrier >;
	using LayerPropertiesArray = std::vector< VkLayerProperties >;
	using MemoryBarrierArray = std::vector< VkMemoryBarrier >;
	using PipelineStageFlagsArray = std::vector< VkPipelineStageFlags >;
	using PushConstantArray = std::vector< PushConstant >;
	using PushConstantRangeArray = std::vector< VkPushConstantRange >;
	using QueueFamilyPropertiesArray = std::vector< VkQueueFamilyProperties >;
	using ResultArray = std::vector< VkResult >;
	using ScissorArray = std::vector< VkRect2D >;
	using ShaderStageStateArray = std::vector< VkShaderModuleCreateInfo >;
	using SpecialisationMapEntryArray = std::vector< VkSpecializationMapEntry >;
	using SubpassDescriptionArray = std::vector< VkSubpassDescription >;
	using SubpassDependencyArray = std::vector< VkSubpassDependency >;
	using VertexInputAttributeDescriptionArray = std::vector< VkVertexInputAttributeDescription >;
	using VertexInputBindingDescriptionArray = std::vector< VkVertexInputBindingDescription >;
	using ViewportArray = std::vector< VkViewport >;
	using WriteDescriptorSetArray = std::vector< VkWriteDescriptorSet >;

	using FrameBufferPtrArray = std::vector< FrameBufferPtr >;
	using CommandBufferPtrArray = std::vector< CommandBufferPtr >;
	using PhysicalDevicePtrArray = std::vector< PhysicalDevicePtr >;
	using ImagePtrArray = std::vector< ImagePtr >;
	using ImageViewPtrArray = std::vector< ImageViewPtr >;

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
	/**
	*\~french
	*\brief
	*	Crée un tableau de descripteurs Vulkan à partir d'un tableau d'objets.
	*\remarks
	*	On présuppose ici que les objets ont des opérateurs de conversion implicite vers les descripteurs Vulkan.
	*\param[in] input
	*	Le tableau d'objets.
	*\return
	*	Le tableau de descripteurs Vulkan.
	*\~english
	*\brief
	*	Creates a Vulkan handles array from an objects array.
	*\remarks
	*	One prerequisite is that the object class has an implicit convertion operator to the Vulkan handle.
	*\param[in] input
	*	The objects array.
	*\return
	*	The Vulkan handles array.
	*/
	template< typename VkType, typename LibType >
	inline std::vector< VkType > makeVkArray( std::vector< LibType > const & input )
	{
		std::vector< VkType > result;
		result.reserve( input.size() );

		for ( auto const & element : input )
		{
			result.emplace_back( element );
		}

		return result;
	}
	/**
	*\~french
	*\brief
	*	Crée un tableau de descripteurs Vulkan à partir d'un tableau de références sur des objets.
	*\remarks
	*	On présuppose ici que les objets ont des opérateurs de conversion implicite vers les descripteurs Vulkan.
	*\param[in] input
	*	Le tableau d'objets.
	*\return
	*	Le tableau de descripteurs Vulkan.
	*\~english
	*\brief
	*	Creates a Vulkan handles array from an objects references array.
	*\remarks
	*	One prerequisite is that the object class has an implicit convertion operator to the Vulkan handle.
	*\param[in] input
	*	The objects array.
	*\return
	*	The Vulkan handles array.
	*/
	template< typename VkType, typename LibType >
	inline std::vector< VkType > makeVkArray( std::vector< std::reference_wrapper< LibType > > const & input )
	{
		std::vector< VkType > result;
		result.reserve( input.size() );

		for ( auto const & element : input )
		{
			result.emplace_back( element.get() );
		}

		return result;
	}
	/**
	*\~french
	*\brief
	*	Crée un tableau de descripteurs Vulkan à partir d'un tableau de pointeurs sur des objets.
	*\remarks
	*	On présuppose ici que les objets ont des opérateurs de conversion implicite vers les descripteurs Vulkan.
	*\param[in] input
	*	Le tableau d'objets.
	*\return
	*	Le tableau de descripteurs Vulkan.
	*\~english
	*\brief
	*	Creates a Vulkan handles array from an objects pointers array.
	*\remarks
	*	One prerequisite is that the object class has an implicit convertion operator to the Vulkan handle.
	*\param[in] input
	*	The objects array.
	*\return
	*	The Vulkan handles array.
	*/
	template< typename VkType, typename LibType >
	inline std::vector< VkType > makeVkArray( std::vector< std::unique_ptr< LibType > > const & input )
	{
		std::vector< VkType > result;
		result.reserve( input.size() );

		for ( auto const & element : input )
		{
			result.emplace_back( *element );
		}

		return result;
	}
	/**
	*\~french
	*\brief
	*	Crée un tableau de descripteurs Vulkan à partir d'un tableau de références sur des objets.
	*\remarks
	*	On présuppose ici que les objets ont des opérateurs de conversion implicite vers les descripteurs Vulkan.
	*\param[in] input
	*	Le tableau d'objets.
	*\return
	*	Le tableau de descripteurs Vulkan.
	*\~english
	*\brief
	*	Creates a Vulkan handles array from an objects references array.
	*\remarks
	*	One prerequisite is that the object class has an implicit convertion operator to the Vulkan handle.
	*\param[in] input
	*	The objects array.
	*\return
	*	The Vulkan handles array.
	*/
	template< typename VkType, typename LibType >
	inline std::vector< VkType > makeVkArray( std::vector< std::reference_wrapper< LibType const > > const & input )
	{
		std::vector< VkType > result;
		result.reserve( input.size() );

		for ( auto const & element : input )
		{
			result.emplace_back( VkType( element.get() ) );
		}

		return result;
	}
	/**
	*\brief
	*	Convertit un tableau de LibType en tableau de VkType.
	*\remarks
	*	Un prérequis à cette fonction est que la fonction VkType convert( LibType ) existe.
	*\param[in] values
	*	Le tableau de LibType.
	*\return
	*	Le tableau de VkType.
	*/
	template< typename VkType, typename LibType >
	std::vector< VkType > convert( std::vector< LibType > const & values )
	{
		std::vector< VkType > result;
		result.reserve( values.size() );

		for ( auto & value : values )
		{
			result.emplace_back( convert( value ) );
		}

		return result;
	}
	/**
	*\brief
	*	Convertit un tableau de LibType en tableau de VkType.
	*\remarks
	*	Un prérequis à cette fonction est que la fonction VkType convert( LibType ) existe.
	*\param[in] values
	*	Le tableau de LibType.
	*\return
	*	Le tableau de VkType.
	*/
	template< typename VkType, typename LibType >
	std::vector< VkType > convert( std::vector< std::reference_wrapper< LibType const > > const & values )
	{
		std::vector< VkType > result;
		result.reserve( values.size() );

		for ( auto & value : values )
		{
			result.emplace_back( convert( value.get() ) );
		}

		return result;
	}
	/**
	*\brief
	*	Convertit un tableau de *ItType en tableau de VkType.
	*\remarks
	*	Un prérequis à cette fonction est que la fonction VkType convert( *ItType ) existe.
	*\param[in] begin, end
	*	Les limites d'un tableau de *ItType.
	*\return
	*	Le tableau de VkType.
	*/
	template< typename VkType, typename ItType >
	std::vector< VkType > convert( ItType begin, ItType end )
	{
		std::vector< VkType > result;
		result.reserve( std::distance( begin, end ) );

		for ( auto it = begin; it != end; ++it )
		{
			result.emplace_back( convert( *it ) );
		}

		return result;
	}
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
