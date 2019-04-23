/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Core/TestSurface.hpp"

#include <Ashes/Core/Device.hpp>
#include <Ashes/Miscellaneous/SwapChainCreateInfo.hpp>

namespace test_renderer
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
		*\param[in] test_renderer
		*	L'instance.
		*\param[in] connection
		*	La connection à l'application.
		*/
		Device( Instance const & instance
			, PhysicalDevice const & physicalDevice
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
			, VkBufferUsageFlags target )const override;
		/**
		*\copydoc	ashes::Device::createBufferView
		*/
		ashes::BufferViewPtr createBufferView( ashes::BufferBase const & buffer
			, VkFormat format
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
		virtual ashes::ShaderModulePtr createShaderModule( VkShaderStageFlagBits stage )const override;
		/**
		*\copydoc	ashes::Device::createQueryPool
		*/
		ashes::QueryPoolPtr createQueryPool( VkQueryType type
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

	private:
		void doCreateQueues();

	private:
		Instance const & m_instance;
		PhysicalDevice const & m_gpu;
		using QueueCreateCount = std::pair< ashes::DeviceQueueCreateInfo, uint32_t >;
		std::map< uint32_t, QueueCreateCount > m_queues;
	};
}
