/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Buffer/Buffer.hpp>
#include <Ashes/Core/Surface.hpp>
#include <Ashes/Core/Device.hpp>
#include <Ashes/Miscellaneous/QueueCreateInfo.hpp>
#include <Ashes/Miscellaneous/SwapChainCreateInfo.hpp>

namespace d3d11_renderer
{
	class Device
		: public ashes::Device
	{
	public:
		Device( Instance const & instance
			, ashes::SurfacePtr surface
			, ashes::DeviceCreateInfo createInfos );
		~Device();
		/**
		*\copydoc	ashes::Device::createStagingTexture
		*/
		ashes::StagingTexturePtr createStagingTexture( ashes::Format format
			, ashes::Extent2D const & extent )const;
		/*
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
		inline ashes::Surface const & getSurface()const
		{
			return *m_surface;
		}

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
			return m_gpu;
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
		PhysicalDevice const & m_gpu;
		ashes::SurfacePtr m_surface;
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
