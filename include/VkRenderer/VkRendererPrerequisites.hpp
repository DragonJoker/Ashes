/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <Ashes/AshesConfig.hpp>

#define VK_USE_PLATFORM_EXTENSIONS

#if ASHES_WIN32
#	define VK_USE_PLATFORM_WIN32_KHR
#elif ASHES_XCB
#	define VK_USE_PLATFORM_XCB_KHR
#elif ASHES_XLIB
#	define VK_USE_PLATFORM_XLIB_KHR
#elif ASHES_ANDROID
#	define VK_USE_PLATFORM_ANDROID_KHR
#else
#	define VK_USE_PLATFORM_XLIB_KHR
#endif

#if !defined( NDEBUG )
#	define LOAD_VALIDATION_LAYERS 1
#else
#	define LOAD_VALIDATION_LAYERS 1
#endif

#include "VkRenderer/VkRendererConfig.hpp"

#include "vulkan/vulkan.h"

#ifdef min
#	undef min
#	undef max
#	undef abs
#endif

#include "RenderPass/VkClearValue.hpp"

#include "Enum/VkAccessFlag.hpp"
#include "Enum/VkAttachmentLoadOp.hpp"
#include "Enum/VkAttachmentStoreOp.hpp"
#include "Enum/VkBlendFactor.hpp"
#include "Enum/VkBlendOp.hpp"
#include "Enum/VkBorderColour.hpp"
#include "Enum/VkBufferTarget.hpp"
#include "Enum/VkColorSpace.hpp"
#include "Enum/VkColourComponentFlag.hpp"
#include "Enum/VkCommandBufferResetFlag.hpp"
#include "Enum/VkCommandBufferUsageFlag.hpp"
#include "Enum/VkCommandPoolCreateFlag.hpp"
#include "Enum/VkComponentSwizzle.hpp"
#include "Enum/VkCompositeAlphaFlag.hpp"
#include "Enum/VkCompareOp.hpp"
#include "Enum/VkCullModeFlag.hpp"
#include "Enum/VkDebugReportFlag.hpp"
#include "Enum/VkDebugReportObjectType.hpp"
#include "Enum/VkDependencyFlag.hpp"
#include "Enum/VkDepthStencilStateFlag.hpp"
#include "Enum/VkDescriptorPoolCreateFlag.hpp"
#include "Enum/VkDescriptorType.hpp"
#include "Enum/VkDeviceCreateFlag.hpp"
#include "Enum/VkDeviceQueueCreateFlag.hpp"
#include "Enum/VkDynamicStateEnable.hpp"
#include "Enum/VkDynamicStateFlag.hpp"
#include "Enum/VkFormat.hpp"
#include "Enum/VkFormatFeatureFlag.hpp"
#include "Enum/VkFenceCreateFlag.hpp"
#include "Enum/VkFilter.hpp"
#include "Enum/VkFrontFace.hpp"
#include "Enum/VkImageAspectFlag.hpp"
#include "Enum/VkImageCreateFlag.hpp"
#include "Enum/VkImageLayout.hpp"
#include "Enum/VkImageTiling.hpp"
#include "Enum/VkImageUsageFlag.hpp"
#include "Enum/VkIndexType.hpp"
#include "Enum/VkInstanceCreateFlag.hpp"
#include "Enum/VkLogicOp.hpp"
#include "Enum/VkMemoryHeapFlag.hpp"
#include "Enum/VkMemoryMapFlag.hpp"
#include "Enum/VkMemoryPropertyFlag.hpp"
#include "Enum/VkMipmapMode.hpp"
#include "Enum/VkMultisampleStateFlag.hpp"
#include "Enum/VkPhysicalDeviceType.hpp"
#include "Enum/VkPipelineBindPoint.hpp"
#include "Enum/VkPipelineStageFlag.hpp"
#include "Enum/VkPolygonMode.hpp"
#include "Enum/VkPresentMode.hpp"
#include "Enum/VkPrimitiveTopology.hpp"
#include "Enum/VkQueryControlFlag.hpp"
#include "Enum/VkQueryPipelineStatisticFlag.hpp"
#include "Enum/VkQueryType.hpp"
#include "Enum/VkQueryResultFlag.hpp"
#include "Enum/VkQueueFlag.hpp"
#include "Enum/VkRasterisationStateFlag.hpp"
#include "Enum/VkRenderPassCreateFlag.hpp"
#include "Enum/VkResult.hpp"
#include "Enum/VkSampleCountFlag.hpp"
#include "Enum/VkShaderStageFlag.hpp"
#include "Enum/VkSharingMode.hpp"
#include "Enum/VkStencilOp.hpp"
#include "Enum/VkSubpassContents.hpp"
#include "Enum/VkSubpassDescriptionFlag.hpp"
#include "Enum/VkSurfaceCounterFlag.hpp"
#include "Enum/VkSurfaceTransformFlag.hpp"
#include "Enum/VkSwapChainCreateFlag.hpp"
#include "Enum/VkTessellationStateFlag.hpp"
#include "Enum/VkTextureType.hpp"
#include "Enum/VkTextureViewType.hpp"
#include "Enum/VkVertexInputRate.hpp"
#include "Enum/VkViewportStateFlag.hpp"
#include "Enum/VkWrapMode.hpp"
#include "Command/VkCommandBufferInheritanceInfo.hpp"
#include "Descriptor/VkDescriptorBufferInfo.hpp"
#include "Descriptor/VkDescriptorImageInfo.hpp"
#include "Descriptor/VkDescriptorSetLayoutBinding.hpp"
#include "Descriptor/VkWriteDescriptorSet.hpp"
#include "Image/VkComponentMapping.hpp"
#include "Image/VkImageSubresource.hpp"
#include "Image/VkImageSubresourceLayers.hpp"
#include "Image/VkImageSubresourceRange.hpp"
#include "Image/VkSubresourceLayout.hpp"
#include "Miscellaneous/VkApplicationInfo.hpp"
#include "Miscellaneous/VkBufferCopy.hpp"
#include "Miscellaneous/VkBufferImageCopy.hpp"
#include "Miscellaneous/VkDebugMarkerObjectNameInfo.hpp"
#include "Miscellaneous/VkDebugReportCallbackCreateInfo.hpp"
#include "Miscellaneous/VkDeviceCreateInfo.hpp"
#include "Miscellaneous/VkExtensionProperties.hpp"
#include "Miscellaneous/VkExtent2D.hpp"
#include "Miscellaneous/VkExtent3D.hpp"
#include "Miscellaneous/VkFormatProperties.hpp"
#include "Miscellaneous/VkImageBlit.hpp"
#include "Miscellaneous/VkImageCopy.hpp"
#include "Miscellaneous/VkInstanceCreateInfo.hpp"
#include "Miscellaneous/VkLayerProperties.hpp"
#include "Miscellaneous/VkMemoryAllocateInfo.hpp"
#include "Miscellaneous/VkMemoryRequirements.hpp"
#include "Miscellaneous/VkOffset2D.hpp"
#include "Miscellaneous/VkOffset3D.hpp"
#include "Miscellaneous/VkPhysicalDeviceFeatures.hpp"
#include "Miscellaneous/VkPhysicalDeviceMemoryProperties.hpp"
#include "Miscellaneous/VkPhysicalDeviceProperties.hpp"
#include "Miscellaneous/VkPushConstantRange.hpp"
#include "Miscellaneous/VkQueueCreateInfo.hpp"
#include "Miscellaneous/VkQueueFamilyProperties.hpp"
#include "Miscellaneous/VkRect2D.hpp"
#include "Miscellaneous/VkSurfaceCapabilities.hpp"
#include "Miscellaneous/VkSurfaceFormat.hpp"
#include "Miscellaneous/VkSwapChainCreateInfo.hpp"
#include "Pipeline/VkColourBlendState.hpp"
#include "Pipeline/VkColourBlendStateAttachment.hpp"
#include "Pipeline/VkDepthStencilState.hpp"
#include "Pipeline/VkDynamicState.hpp"
#include "Pipeline/VkInputAssemblyState.hpp"
#include "Pipeline/VkMultisampleState.hpp"
#include "Pipeline/VkRasterisationState.hpp"
#include "Pipeline/VkScissor.hpp"
#include "Pipeline/VkShaderStageState.hpp"
#include "Pipeline/VkSpecialisationInfo.hpp"
#include "Pipeline/VkSpecialisationMapEntry.hpp"
#include "Pipeline/VkStencilOpState.hpp"
#include "Pipeline/VkTessellationState.hpp"
#include "Pipeline/VkVertexInputAttributeDescription.hpp"
#include "Pipeline/VkVertexInputBindingDescription.hpp"
#include "Pipeline/VkVertexInputState.hpp"
#include "Pipeline/VkViewport.hpp"
#include "Pipeline/VkViewportState.hpp"
#include "RenderPass/VkAttachmentDescription.hpp"
#include "RenderPass/VkAttachmentReference.hpp"
#include "RenderPass/VkClearAttachment.hpp"
#include "RenderPass/VkClearRect.hpp"
#include "RenderPass/VkClearValue.hpp"
#include "RenderPass/VkRenderPassCreateInfo.hpp"
#include "RenderPass/VkSubpassDependency.hpp"
#include "RenderPass/VkSubpassDescription.hpp"
#include "Sync/VkBufferMemoryBarrier.hpp"
#include "Sync/VkImageMemoryBarrier.hpp"
#include "Sync/VkMemoryBarrier.hpp"

#include "Miscellaneous/VkDebug.hpp"
#include "Miscellaneous/VkError.hpp"

#include <Ashes/AshesPrerequisites.hpp>

#include <list>

namespace vk_renderer
{
#if defined( VK_API_VERSION_1_0 )
	static uint32_t const VulkanVersion = VK_API_VERSION_1_0;
#elif defined( VK_VERSION_1_0 )
	static uint32_t const VulkanVersion = VK_VERSION_1_0;
#else
	static uint32_t const VulkanVersion = VK_MAKE_VERSION( 1, 0, 0 );
#endif

	template< typename VkType, bool Image >
	class MemoryStorage;
	using BufferStorage = MemoryStorage< VkBuffer, false >;
	using ImageStorage = MemoryStorage< VkImage, true >;

	static const std::string ShortName = "VkLib";

	class Attribute;
	class BackBuffer;
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
	class Texture;
	class TextureView;
	class UniformBuffer;
	class VertexBufferBase;
	class VertexLayout;

	using AttributeArray = std::vector< Attribute >;

	using BackBufferPtr = std::unique_ptr< BackBuffer >;
	using BufferStoragePtr = std::unique_ptr< BufferStorage >;
	using CommandPoolPtr = std::unique_ptr< CommandPool >;
	using ImageStoragePtr = std::unique_ptr< ImageStorage >;
	using PhysicalDevicePtr = std::unique_ptr< PhysicalDevice >;
	using QueuePtr = std::unique_ptr< Queue >;
	using RenderSubpassPtr = std::unique_ptr< RenderSubpass >;
	using SurfacePtr = std::unique_ptr< Surface >;
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
	template< typename VkType, typename RendererType, typename LibType >
	inline std::vector< VkType > makeVkArray( std::vector< LibType > const & input )
	{
		std::vector< VkType > result;
		result.reserve( input.size() );

		for ( auto const & element : input )
		{
			result.emplace_back( static_cast< RendererType const & >( element ) );
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
	template< typename VkType, typename RendererType, typename LibType >
	inline std::vector< VkType > makeVkArray( std::vector< std::reference_wrapper< LibType > > const & input )
	{
		std::vector< VkType > result;
		result.reserve( input.size() );

		for ( auto const & element : input )
		{
			result.emplace_back( static_cast< RendererType const & >( element.get() ) );
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
	template< typename VkType, typename RendererType, typename LibType >
	inline std::vector< VkType > makeVkArray( std::vector< std::unique_ptr< LibType > > const & input )
	{
		std::vector< VkType > result;
		result.reserve( input.size() );

		for ( auto const & element : input )
		{
			result.emplace_back( static_cast< RendererType const & >( *element ) );
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
	template< typename VkType, typename RendererType, typename LibType >
	inline std::vector< VkType > makeVkArray( std::vector< std::reference_wrapper< LibType const > > const & input )
	{
		std::vector< VkType > result;
		result.reserve( input.size() );

		for ( auto const & element : input )
		{
			result.emplace_back( VkType( static_cast< RendererType const & >( element.get() ) ) );
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
