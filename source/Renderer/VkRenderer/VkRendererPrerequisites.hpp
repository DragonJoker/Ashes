/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <Config.hpp>

#include "VkConfig.hpp"

#define VK_USE_PLATFORM_EXTENSIONS

#if RENDERLIB_WIN32
#	define VK_USE_PLATFORM_WIN32_KHR
#elif RENDERLIB_XCB
#	define VK_USE_PLATFORM_XCB_KHR
#elif RENDERLIB_XLIB
#	define VK_USE_PLATFORM_XLIB_KHR
#elif RENDERLIB_ANDROID
#	define VK_USE_PLATFORM_ANDROID_KHR
#endif

#include "vulkan/vulkan.h"

#ifdef min
#	undef min
#	undef max
#	undef abs
#endif

#include "VkAccessFlag.hpp"
#include "VkAttributeFormat.hpp"
#include "VkBlendFactor.hpp"
#include "VkBlendOp.hpp"
#include "VkBorderColour.hpp"
#include "VkBufferTarget.hpp"
#include "VkClearValue.hpp"
#include "VkColourComponentFlag.hpp"
#include "VkCommandBufferResetFlag.hpp"
#include "VkCommandBufferUsageFlag.hpp"
#include "VkCommandPoolCreateFlag.hpp"
#include "VkCompareOp.hpp"
#include "VkCullModeFlag.hpp"
#include "VkDepthStencilStateFlag.hpp"
#include "VkDescriptorType.hpp"
#include "VkFenceCreateFlag.hpp"
#include "VkFilter.hpp"
#include "VkFrontFace.hpp"
#include "VkImageAspectFlag.hpp"
#include "VkImageLayout.hpp"
#include "VkImageTiling.hpp"
#include "VkImageUsageFlag.hpp"
#include "VkIndexType.hpp"
#include "VkLogicOp.hpp"
#include "VkMemoryMapFlag.hpp"
#include "VkMemoryPropertyFlag.hpp"
#include "VkMipmapMode.hpp"
#include "VkMultisampleStateFlag.hpp"
#include "VkPipelineBindPoint.hpp"
#include "VkPipelineStageFlag.hpp"
#include "VkPixelFormat.hpp"
#include "VkPolygonMode.hpp"
#include "VkPrimitiveTopology.hpp"
#include "VkQueryControlFlag.hpp"
#include "VkQueryPipelineStatisticFlag.hpp"
#include "VkRasterisationStateFlag.hpp"
#include "VkSampleCountFlag.hpp"
#include "VkShaderStageFlag.hpp"
#include "VkStencilOp.hpp"
#include "VkSubpassContents.hpp"
#include "VkTessellationStateFlag.hpp"
#include "VkTextureType.hpp"
#include "VkWrapMode.hpp"

#include "VkDebug.hpp"
#include "VkError.hpp"
#include "VulkanFunctions.hpp"

#include <Renderer/RendererPrerequisites.hpp>

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
	class Connection;
	class DescriptorSet;
	class DescriptorSetBinding;
	class DescriptorSetLayout;
	class DescriptorSetLayoutBinding;
	class DescriptorSetPool;
	class Device;
	class Pipeline;
	class PipelineLayout;
	class PhysicalDevice;
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
	using BufferStoragePtr = std::unique_ptr< BufferStorage >;
	using ConnectionPtr = std::unique_ptr< Connection >;
	using CommandPoolPtr = std::unique_ptr< CommandPool >;
	using DescriptorSetBindingPtr = std::unique_ptr< DescriptorSetBinding >;
	using ImageStoragePtr = std::unique_ptr< ImageStorage >;
	using PhysicalDevicePtr = std::unique_ptr< PhysicalDevice >;
	using QueuePtr = std::unique_ptr< Queue >;
	using RenderSubpassPtr = std::unique_ptr< RenderSubpass >;
	using TextureViewPtr = std::unique_ptr< TextureView >;

	using BackBufferPtrArray = std::vector< BackBufferPtr >;
	using RenderSubpassPtrArray = std::vector< RenderSubpassPtr >;

	using CommandBufferCRef = std::reference_wrapper< CommandBuffer const >;
	using DescriptorSetLayoutCRef = std::reference_wrapper< DescriptorSetLayout const >;
	using RenderSubpassCRef = std::reference_wrapper< RenderSubpass const >;
	using SemaphoreCRef = std::reference_wrapper< Semaphore const >;
	using SwapChainCRef = std::reference_wrapper< SwapChain const >;
	using TextureCRef = std::reference_wrapper< Texture const >;
	using TextureViewCRef = std::reference_wrapper< TextureView const >;
	using VertexLayoutCRef = std::reference_wrapper< VertexLayout const >;
	using VertexBufferCRef = std::reference_wrapper< VertexBufferBase const >;

	using CommandBufferCRefArray = std::vector< CommandBufferCRef >;
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
	*	Contient les propriétés et extensions d'une couche de la VkInstance.
	*\~english
	*\brief
	*	Holds the properties and extensions of a VkInstance layer.
	*/
	struct LayerProperties
	{
		//!\~french		Les propriétés de la couche.
		//!\~english	The layer's properties.
		VkLayerProperties m_properties;
		//!\~french		Les extensions supportées par la couche.
		//!\~english	The layer's supported extensions.
		std::vector< VkExtensionProperties > m_extensions;
	};
	/**
	*\~french
	*\brief
	*	Vérifie si les extensions demandées sont toutes disponibles dans la liste des extensions supportées.
	*\remarks
	*	Si une extension n'est pas supportée, une std::runtime_error sera lancée.
	*\param[in] supported
	*	La liste des extensions supportées.
	*\param[in] requested
	*	La liste des extensions requises.
	*\~english
	*\brief
	*	Checks if the requested extensions are all available in the list of supported extensions.
	*\remarks
	*	If an extension is not supported, a std::runtime_error will be thrown.
	*\param[in] supported
	*	The supported extensions list.
	*\param[in] requested
	*	The requested extensions list.
	*/
	void checkExtensionsAvailability( std::vector< VkExtensionProperties > const & supported
		, std::vector< char const * > const & requested );
	/**
	*\~french
	*\brief
	*	Dit si le format Vulkan donné est un format utilisable pour les tampons de profondeur et stencil.
	*\param[in] format
	*	Le format Vulkan à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given Vulkan format is usable in depth and stencil buffers.
	*\param[in] format
	*	The Vulkan format.
	*\return
	*	\p true if it is usable in depth and stencil buffers.
	*/
	bool isDepthStencilFormat( VkFormat format );
	/**
	*\~french
	*\brief
	*	Dit si le format Vulkan donné est un format utilisable pour les tampons de stencil.
	*\param[in] format
	*	Le format Vulkan à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given Vulkan format is usable in stencil buffers.
	*\param[in] format
	*	The Vulkan format.
	*\return
	*	\p true if it is usable in stencil buffers.
	*/
	bool isStencilFormat( VkFormat format );
	/**
	*\~french
	*\brief
	*	Dit si le format Vulkan donné est un format utilisable pour les tampons de profondeur.
	*\param[in] format
	*	Le format Vulkan à tester.
	*\return
	*	\p true s'il l'est...
	*\~english
	*\brief
	*	Tells if the given Vulkan format is usable in depth buffers.
	*\param[in] format
	*	The Vulkan format.
	*\return
	*	\p true if it is usable in depth buffers.
	*/
	bool isDepthFormat( VkFormat format );
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
	*	Convertit un tableau de RendererType en tableau de VkType.
	*\remarks
	*	Un prérequis à cette fonction est que la fonction VkType convert( RendererType ) existe.
	*\param[in] values
	*	Le tableau de RendererType.
	*\return
	*	Le tableau de VkType.
	*/
	template< typename VkType, typename RendererType >
	std::vector< VkType > convert( std::vector< RendererType > const & values )
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
	*	Convertit un tableau de RendererType en tableau de VkType.
	*\remarks
	*	Un prérequis à cette fonction est que la fonction VkType convert( RendererType ) existe.
	*\param[in] values
	*	Le tableau de RendererType.
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
