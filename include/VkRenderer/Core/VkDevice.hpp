/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Core/VkSurface.hpp"

#include <Ashes/Core/Device.hpp>
#include <Ashes/Miscellaneous/PhysicalDeviceMemoryProperties.hpp>
#include <Ashes/Miscellaneous/QueueCreateInfo.hpp>
#include <Ashes/Miscellaneous/SwapChainCreateInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Classe contenant les informations liées au GPU logique.
	*/
	class Device
		: public ashes::Device
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] vk_renderer
		*	L'instance.
		*\param[in] connection
		*	La connection à l'application.
		*/
		Device( Instance const & instance
			, ashes::SurfacePtr surface
			, ashes::DeviceCreateInfo createInfos );
		/**
		*\brief
		*	Destructeur.
		*/
		~Device();
		/**
		*\copydoc	ashes::Device::createRenderPass
		*/
		ashes::RenderPassPtr createRenderPass( ashes::RenderPassCreateInfo createInfo )const override;
		/**
		*\copydoc	ashes::Device::createPipelineLayout
		*/
		ashes::PipelineLayoutPtr createPipelineLayout( ashes::DescriptorSetLayoutCRefArray const & setLayouts
			, ashes::PushConstantRangeArray const & pushConstantRanges )const override;
		/**
		*\copydoc	ashes::Device::createDescriptorSetLayout
		*/
		ashes::DescriptorSetLayoutPtr createDescriptorSetLayout( ashes::DescriptorSetLayoutBindingArray bindings )const override;
		/**
		*\copydoc	ashes::Device::createDescriptorPool
		*/
		ashes::DescriptorPoolPtr createDescriptorPool( ashes::DescriptorPoolCreateFlags flags
			, uint32_t maxSets
			, ashes::DescriptorPoolSizeArray poolSizes )const override;
		/**
		*\copydoc	ashes::Device::allocateMemory
		*/
		ashes::DeviceMemoryPtr allocateMemory( ashes::MemoryAllocateInfo allocateInfo )const override;
		/**
		*\copydoc	ashes::Device::createImage
		*/
		ashes::ImagePtr createImage( ashes::ImageCreateInfo const & createInfo )const override;
		/**
		*\copydoc	ashes::Device::getImageSubresourceLayout
		*/
		void getImageSubresourceLayout( ashes::Image const & image
			, ashes::ImageSubresource const & subresource
			, ashes::SubresourceLayout & layout )const override;
		/**
		*\copydoc	ashes::Device::createSampler
		*/
		ashes::SamplerPtr createSampler( ashes::SamplerCreateInfo const & createInfo )const override;
		/**
		*\copydoc	ashes::Device::createBuffer
		*/
		ashes::BufferBasePtr createBuffer( uint32_t size
			, ashes::BufferTargets target )const override;
		/**
		*\copydoc	ashes::Device::createBufferView
		*/
		ashes::BufferViewPtr createBufferView( ashes::BufferBase const & buffer
			, ashes::Format format
			, uint32_t offset
			, uint32_t range )const override;
		/**
		*\copydoc	ashes::Device::createSwapChain
		*/
		ashes::SwapChainPtr createSwapChain( ashes::SwapChainCreateInfo createInfo )const override;
		/**
		*\copydoc	ashes::Device::createSemaphore
		*/
		ashes::SemaphorePtr createSemaphore()const override;
		/**
		*\copydoc	ashes::Device::createFence
		*/
		ashes::FencePtr createFence( ashes::FenceCreateFlags flags )const override;
		/**
		*\copydoc	ashes::Device::createEvent
		*/
		ashes::EventPtr createEvent()const override;
		/**
		*\copydoc	ashes::Device::createCommandPool
		*/
		ashes::CommandPoolPtr createCommandPool( uint32_t queueFamilyIndex
			, ashes::CommandPoolCreateFlags const & flags )const override;
		/**
		*\copydoc	ashes::Device::createShaderProgram
		*/
		virtual ashes::ShaderModulePtr createShaderModule( ashes::ShaderStageFlag stage )const override;
		/**
		*\copydoc	ashes::Device::createQueryPool
		*/
		ashes::QueryPoolPtr createQueryPool( ashes::QueryType type
			, uint32_t count
			, ashes::QueryPipelineStatisticFlags pipelineStatistics )const override;
		/**
		*\copydoc	ashes::Device::debugMarkerSetObjectName
		*/
		void debugMarkerSetObjectName( ashes::DebugMarkerObjectNameInfo const & nameInfo )const override;
		/**
		*\copydoc	ashes::Device::getQueue
		*/
		ashes::QueuePtr getQueue( uint32_t familyIndex
			, uint32_t index )const override;
		/**
		*\brief
		*	Attend que le périphérique soit inactif.
		*/
		void waitIdle()const override;
		/**
		*\~french
		*\brief
		*	Récupère l'indice de la famille de mémoire ayant les propriétés requises demandées.
		*\param[in] typeBits
		*	Un masque de bits contenant un bit défini pour chaque type de mémoire supporté pour la ressource.
		*	Le \p i-ème bit est défini ssi le \p i-ème type de mémoire dans la structure ashes::PhysicalDeviceMemoryProperties pour le périphérique physique est supporté pour la ressource.
		*\param[in] requirements
		*	Les propriétés requises pour la famille de mémoire.
		*return
		*	L'indice de la famille de mémoire.
		*\~english
		*\brief
		*	Retrieves the memory family index for given requirements.
		*\param[in] typeBits
		*	A bitmask containing one bit set for every supported memory type for the resource.
		*	Bit \p i is set iff the memory type \p i in the ashes::PhysicalDeviceMemoryProperties structure for the physical device is supported for the resource.
		*\param[in] requirements
		*	The required properties for the memory family.
		*return
		*	The memory family index.
		*/
		uint32_t deduceMemoryType( uint32_t typeBits
			, ashes::MemoryPropertyFlags requirements )const;
		/**
		*\~french
		*\return
		*	L'API de rendu.
		*\~english
		*\return
		*	The rendering API.
		*/
		inline Instance const & getInstance()const
		{
			return m_instance;
		}
		/**
		*\~french
		*\return
		*	La connection à l'application.
		*\~english
		*\return
		*	The connection to the application.
		*/
		inline Surface const & getSurface()const
		{
			return *m_surface;
		}
		/**
		*\brief
		*	Le VkDevice.
		*/
		inline operator VkDevice const &()const
		{
			return m_device;
		}

#define VK_LIB_DEVICE_FUNCTION( fun ) PFN_##fun fun;
#	include "Miscellaneous/VulkanFunctionsList.inl"

	private:
		void doCreateQueues();

	private:
		Instance const & m_instance;
		PhysicalDevice const & m_gpu;
		SurfacePtr m_surface;
		std::vector< VkDeviceQueueCreateInfo > m_queueCreateInfos;
		std::vector< char const * > m_enabledLayerNames;
		std::vector< char const * > m_enabledExtensionNames;
		VkPhysicalDeviceFeatures m_enabledFeatures;
		VkDeviceCreateInfo m_vkCreateInfos;
		VkDevice m_device{ VK_NULL_HANDLE };
		std::map< uint32_t, ashes::DeviceQueueCreateInfo > m_queues;
		ashes::PhysicalDeviceMemoryProperties m_memoryProperties;
	};
}
