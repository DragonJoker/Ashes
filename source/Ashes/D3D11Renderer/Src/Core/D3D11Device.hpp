/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Core/D3D11Connection.hpp"

#include <Buffer/Buffer.hpp>
#include <Core/Device.hpp>

namespace d3d11_renderer
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
		*\param[in] d3d11_renderer
		*	L'instance de Renderer.
		*\param[in] connection
		*	La connection à l'application.
		*/
		Device( Renderer const & renderer
			, ashes::ConnectionPtr && connection );
		/**
		*\brief
		*	Destructeur.
		*/
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
		ashes::DescriptorSetLayoutPtr createDescriptorSetLayout( ashes::DescriptorSetLayoutBindingArray && bindings )const override;
		/**
		*\copydoc	ashes::Device::createDescriptorPool
		*/
		ashes::DescriptorPoolPtr createDescriptorPool( ashes::DescriptorPoolCreateFlags flags
			, uint32_t maxSets
			, ashes::DescriptorPoolSizeArray poolSizes )const override;
		/**
		*\copydoc	ashes::Device::allocateMemory
		*/
		ashes::DeviceMemoryPtr allocateMemory( ashes::MemoryRequirements const & requirements
			, ashes::MemoryPropertyFlags flags )const override;
		/**
		*\copydoc	ashes::Device::createTexture
		*/
		ashes::TexturePtr createTexture( ashes::ImageCreateInfo const & createInfo )const override;
		/**
		*\copydoc	ashes::Device::getImageSubresourceLayout
		*/
		void getImageSubresourceLayout( ashes::Texture const & image
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
		*\copydoc	ashes::Device::createUniformBuffer
		*/
		ashes::UniformBufferBasePtr createUniformBuffer( uint32_t count
			, uint32_t size
			, ashes::BufferTargets target
			, ashes::MemoryPropertyFlags memoryFlags )const override;
		/**
		*\copydoc	ashes::Device::createSwapChain
		*/
		ashes::SwapChainPtr createSwapChain( ashes::Extent2D const & size )const override;
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
		inline Renderer const & getRenderer()const
		{
			return m_renderer;
		}
		/**
		*\~french
		*\return
		*	La connection à l'application.
		*\~english
		*\return
		*	The connection to the application.
		*/
		inline Connection const & getConnection()const
		{
			return *m_connection;
		}

		inline D3D_FEATURE_LEVEL getFeatureLevel()const
		{
			return m_featureLevel;
		}

		inline ID3D11Device * getDevice()const
		{
			return m_device;
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

	private:
		Renderer const & m_renderer;
		PhysicalDevice const & m_gpu;
		ConnectionPtr m_connection;
		ID3D11Device * m_device;
		ID3D11DeviceContext * m_deviceContext;
		ID3D11Query * m_waitIdleQuery;
		D3D_FEATURE_LEVEL m_featureLevel;
#if !defined( NDEBUG )
		ID3D11Debug * m_debug;
#endif
		// Mimic the behavior in Vulkan, when no IBO nor VBO is bound.
		ashes::BufferPtr< uint32_t > m_dummyIndexed;
	};
}
