/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Core/D3D11Layer.hpp"

#include "renderer/D3D11Renderer/Buffer/D3D11Buffer.hpp"
#include "renderer/D3D11Renderer/Core/D3D11Surface.hpp"

namespace ashes::d3d11
{
	class Device
	{
	public:
		Device( Instance const & instance
			, PhysicalDevice const & physicalDevice
			, VkDeviceCreateInfo createInfos );
		~Device();
		/*
		*\copydoc	ashes::Device::createRenderPass
		*/
		VkRenderPass createRenderPass( VkRenderPassCreateInfo createInfo )const;
		/**
		*\copydoc	ashes::Device::createPipelineLayout
		*/
		VkPipelineLayout createPipelineLayout( ashes::VkDescriptorSetLayoutArray const & setLayouts
			, ashes::VkPushConstantRangeArray const & pushConstantRanges )const;
		/**
		*\copydoc	ashes::Device::createDescriptorSetLayout
		*/
		VkDescriptorSetLayout createDescriptorSetLayout( VkDescriptorSetLayoutBindingArray bindings )const;
		/**
		*\copydoc	ashes::Device::createDescriptorPool
		*/
		VkDescriptorPool createDescriptorPool( VkDescriptorPoolCreateFlags flags
			, uint32_t maxSets
			, ashes::VkDescriptorPoolSizeArray poolSizes )const;
		/**
		*\copydoc	ashes::Device::allocateMemory
		*/
		VkDeviceMemory allocateMemory( VkMemoryAllocateInfo allocateInfo )const;
		/**
		*\copydoc	ashes::Device::createImage
		*/
		VkImage createImage( VkImageCreateInfo const & createInfo )const;
		/**
		*\copydoc	ashes::Device::getImageSubresourceLayout
		*/
		void getImageSubresourceLayout( VkImage image
			, VkImageSubresource const & subresource
			, VkSubresourceLayout & layout )const;
		/**
		*\copydoc	ashes::Device::createSampler
		*/
		VkSampler createSampler( VkSamplerCreateInfo const & createInfo )const;
		/**
		*\copydoc	ashes::Device::createBuffer
		*/
		VkBuffer createBuffer( uint32_t size
			, VkBufferUsageFlags target )const;
		/**
		*\copydoc	ashes::Device::createBufferView
		*/
		VkBufferView createBufferView( VkBuffer buffer
			, VkFormat format
			, uint32_t offset
			, uint32_t range )const;
		/**
		*\copydoc	ashes::Device::createSwapChain
		*/
		VkSwapchainKHR createSwapChain( VkSwapchainCreateInfoKHR createInfo )const;
		/**
		*\copydoc	ashes::Device::createSemaphore
		*/
		VkSemaphore createSemaphore()const;
		/**
		*\copydoc	ashes::Device::createFence
		*/
		VkFence createFence( VkFenceCreateFlags flags )const;
		/**
		*\copydoc	ashes::Device::createEvent
		*/
		VkEvent createEvent()const;
		/**
		*\copydoc	ashes::Device::createCommandPool
		*/
		VkCommandPool createCommandPool( uint32_t queueFamilyIndex
			, VkCommandPoolCreateFlags const & flags )const;
		/**
		*\copydoc	ashes::Device::createShaderProgram
		*/
		VkShaderModule createShaderModule( VkShaderStageFlagBits stage )const;
		/**
		*\copydoc	ashes::Device::createQueryPool
		*/
		VkQueryPool createQueryPool( VkQueryType type
			, uint32_t count
			, VkQueryPipelineStatisticFlags pipelineStatistics )const;
		/**
		*\copydoc	ashes::Device::debugMarkerSetObjectName
		*/
		void debugMarkerSetObjectName( VkDebugMarkerObjectNameInfoEXT const & nameInfo )const;
		/**
		*\copydoc	ashes::Device::getQueue
		*/
		VkQueue getQueue( uint32_t familyIndex
			, uint32_t index )const;
		/**
		*\brief
		*	Attend que le périphérique soit inactif.
		*/
		void waitIdle()const;
		/**
		*\~english
		*name
		*	Layers delegation.
		*\~french
		*name
		*	Délégation aux layers.
		*/
		/**@{*/
		bool onCopyToImageCommand( VkCommandBuffer cmd
			, ashes::VkBufferImageCopyArray const & copyInfo
			, VkBuffer src
			, VkImage dst )const;
		bool onCheckHResultCommand( HRESULT hresult
			, std::string message )const;
		/**@}*/
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
		inline D3D_FEATURE_LEVEL getFeatureLevel()const
		{
			return m_featureLevel;
		}

		inline ID3D11Device * getDevice()const
		{
			return m_d3dDevice;
		}

		inline VkBuffer getEmptyIndexedVaoIdx()const
		{
			return m_dummyIndexed;
		}

		inline PhysicalDevice const & getGpu()const
		{
			return m_physicalDevice;
		}

#if !defined( NDEBUG )

		inline ID3D11Debug * getDebug()
		{
			return m_debug;
		}

#endif

	private:
		void doCreateD3D11Device();
		void doCreateDummyIndexBuffer();
		void doCreateQueues();

	private:
		Instance const & m_instance;
		PhysicalDevice const & m_physicalDevice;
		ID3D11Device * m_d3dDevice;
		ID3D11DeviceContext * m_deviceContext;
		ID3D11Query * m_waitIdleQuery;
		D3D_FEATURE_LEVEL m_featureLevel;
		using QueueCreateCount = std::pair< VkDeviceQueueCreateInfo, uint32_t >;
		std::map< uint32_t, QueueCreateCount > m_queues;
#if !defined( NDEBUG )
		ID3D11Debug * m_debug;
#endif
		// Mimic the behavior in Vulkan, when no IBO nor VBO is bound.
		VkBuffer m_dummyIndexed;
	};
}
