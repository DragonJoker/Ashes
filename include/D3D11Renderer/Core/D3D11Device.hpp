/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Core/D3D11Layer.hpp"

#include <Ashes/Buffer/Buffer.hpp>
#include <Ashes/Core/Surface.hpp>
#include <Ashes/Core/Device.hpp>
#include <Ashes/Miscellaneous/QueueCreateInfo.hpp>
#include <Ashes/Miscellaneous/SwapChainCreateInfo.hpp>

namespace ashes::d3d11
{
	class Device
		: public ashes::Device
	{
	public:
		Device( Instance const & instance
			, PhysicalDevice const & physicalDevice
			, ashes::DeviceCreateInfo createInfos );
		~Device();
		/*
		*\copydoc	ashes::Device::createRenderPass
		*/
		ashes::RenderPassPtr createRenderPass( ashes::RenderPassCreateInfo createInfo )const;
		/**
		*\copydoc	ashes::Device::createPipelineLayout
		*/
		ashes::PipelineLayoutPtr createPipelineLayout( ashes::DescriptorSetLayoutCRefArray const & setLayouts
			, ashes::PushConstantRangeArray const & pushConstantRanges )const;
		/**
		*\copydoc	ashes::Device::createDescriptorSetLayout
		*/
		ashes::DescriptorSetLayoutPtr createDescriptorSetLayout( VkDescriptorSetLayoutBindingArray bindings )const;
		/**
		*\copydoc	ashes::Device::createDescriptorPool
		*/
		ashes::DescriptorPoolPtr createDescriptorPool( ashes::DescriptorPoolCreateFlags flags
			, uint32_t maxSets
			, ashes::DescriptorPoolSizeArray poolSizes )const;
		/**
		*\copydoc	ashes::Device::allocateMemory
		*/
		ashes::DeviceMemoryPtr allocateMemory( ashes::MemoryAllocateInfo allocateInfo )const;
		/**
		*\copydoc	ashes::Device::createImage
		*/
		ashes::ImagePtr createImage( ashes::ImageCreateInfo const & createInfo )const;
		/**
		*\copydoc	ashes::Device::getImageSubresourceLayout
		*/
		void getImageSubresourceLayout( ashes::Image const & image
			, ashes::ImageSubresource const & subresource
			, ashes::SubresourceLayout & layout )const;
		/**
		*\copydoc	ashes::Device::createSampler
		*/
		ashes::SamplerPtr createSampler( ashes::SamplerCreateInfo const & createInfo )const;
		/**
		*\copydoc	ashes::Device::createBuffer
		*/
		ashes::BufferBasePtr createBuffer( uint32_t size
			, VkBufferUsageFlags target )const;
		/**
		*\copydoc	ashes::Device::createBufferView
		*/
		ashes::BufferViewPtr createBufferView( ashes::BufferBase const & buffer
			, VkFormat format
			, uint32_t offset
			, uint32_t range )const;
		/**
		*\copydoc	ashes::Device::createSwapChain
		*/
		ashes::SwapChainPtr createSwapChain( ashes::SwapChainCreateInfo createInfo )const;
		/**
		*\copydoc	ashes::Device::createSemaphore
		*/
		ashes::SemaphorePtr createSemaphore()const;
		/**
		*\copydoc	ashes::Device::createFence
		*/
		ashes::FencePtr createFence( ashes::FenceCreateFlags flags )const;
		/**
		*\copydoc	ashes::Device::createEvent
		*/
		ashes::EventPtr createEvent()const;
		/**
		*\copydoc	ashes::Device::createCommandPool
		*/
		ashes::CommandPoolPtr createCommandPool( uint32_t queueFamilyIndex
			, ashes::CommandPoolCreateFlags const & flags )const;
		/**
		*\copydoc	ashes::Device::createShaderProgram
		*/
		virtual ashes::ShaderModulePtr createShaderModule( ashes::ShaderStageFlag stage )const;
		/**
		*\copydoc	ashes::Device::createQueryPool
		*/
		ashes::QueryPoolPtr createQueryPool( ashes::QueryType type
			, uint32_t count
			, ashes::QueryPipelineStatisticFlags pipelineStatistics )const;
		/**
		*\copydoc	ashes::Device::debugMarkerSetObjectName
		*/
		void debugMarkerSetObjectName( ashes::DebugMarkerObjectNameInfo const & nameInfo )const;
		/**
		*\copydoc	ashes::Device::getQueue
		*/
		ashes::QueuePtr getQueue( uint32_t familyIndex
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
		bool onCopyToImageCommand( ashes::CommandBuffer const & cmd
			, ashes::VkBufferImageCopyArray const & copyInfo
			, ashes::BufferBase const & src
			, ashes::Image const & dst )const;
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

		inline ashes::BufferBase const & getEmptyIndexedVaoIdx()const
		{
			return m_dummyIndexed->getBuffer();
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
		using QueueCreateCount = std::pair< ashes::DeviceQueueCreateInfo, uint32_t >;
		std::map< uint32_t, QueueCreateCount > m_queues;
#if !defined( NDEBUG )
		ID3D11Debug * m_debug;
#endif
		// Mimic the behavior in Vulkan, when no IBO nor VBO is bound.
		ashes::BufferPtr< uint32_t > m_dummyIndexed;
	};
}
