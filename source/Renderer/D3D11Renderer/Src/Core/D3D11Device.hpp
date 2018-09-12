/*
This file belongs to RendererLib.
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
		: public renderer::Device
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
			, renderer::ConnectionPtr && connection );
		/**
		*\brief
		*	Destructeur.
		*/
		~Device();
		/**
		*\copydoc	renderer::Device::createStagingTexture
		*/
		renderer::StagingTexturePtr createStagingTexture( renderer::Format format
			, renderer::Extent3D const & extent )const;
		/**
		*\copydoc	renderer::Device::createPushConstantsBuffer
		*/
		renderer::PushConstantsBufferPtr createPushConstantsBuffer( uint32_t location
			, renderer::ShaderStageFlags stageFlags
			, renderer::PushConstantArray const & variables )const override;
		/**
		*\copydoc	renderer::Device::createRenderPass
		*/
		renderer::RenderPassPtr createRenderPass( renderer::RenderPassCreateInfo createInfo )const override;
		/**
		*\copydoc	renderer::Device::createPipelineLayout
		*/
		renderer::PipelineLayoutPtr createPipelineLayout( renderer::DescriptorSetLayoutCRefArray const & setLayouts
			, renderer::PushConstantRangeCRefArray const & pushConstantRanges )const override;
		/**
		*\copydoc	renderer::Device::createDescriptorSetLayout
		*/
		renderer::DescriptorSetLayoutPtr createDescriptorSetLayout( renderer::DescriptorSetLayoutBindingArray && bindings )const override;
		/**
		*\copydoc	renderer::Device::createDescriptorPool
		*/
		renderer::DescriptorPoolPtr createDescriptorPool( renderer::DescriptorPoolCreateFlags flags
			, uint32_t maxSets
			, renderer::DescriptorPoolSizeArray poolSizes )const override;
		/**
		*\copydoc	renderer::Device::allocateMemory
		*/
		renderer::DeviceMemoryPtr allocateMemory( renderer::MemoryRequirements const & requirements
			, renderer::MemoryPropertyFlags flags )const override;
		/**
		*\copydoc	renderer::Device::createTexture
		*/
		renderer::TexturePtr createTexture( renderer::ImageCreateInfo const & createInfo )const override;
		/**
		*\copydoc	renderer::Device::getImageSubresourceLayout
		*/
		void getImageSubresourceLayout( renderer::Texture const & image
			, renderer::ImageSubresource const & subresource
			, renderer::SubresourceLayout & layout )const override;
		/**
		*\copydoc	renderer::Device::createSampler
		*/
		renderer::SamplerPtr createSampler( renderer::SamplerCreateInfo const & createInfo )const override;
		/**
		*\copydoc	renderer::Device::createBuffer
		*/
		renderer::BufferBasePtr createBuffer( uint32_t size
			, renderer::BufferTargets target )const override;
		/**
		*\copydoc	renderer::Device::createBufferView
		*/
		renderer::BufferViewPtr createBufferView( renderer::BufferBase const & buffer
			, renderer::Format format
			, uint32_t offset
			, uint32_t range )const override;
		/**
		*\copydoc	renderer::Device::createUniformBuffer
		*/
		renderer::UniformBufferBasePtr createUniformBuffer( uint32_t count
			, uint32_t size
			, renderer::BufferTargets target
			, renderer::MemoryPropertyFlags memoryFlags )const override;
		/**
		*\copydoc	renderer::Device::createSwapChain
		*/
		renderer::SwapChainPtr createSwapChain( renderer::Extent2D const & size )const override;
		/**
		*\copydoc	renderer::Device::createSemaphore
		*/
		renderer::SemaphorePtr createSemaphore()const override;
		/**
		*\copydoc	renderer::Device::createFence
		*/
		renderer::FencePtr createFence( renderer::FenceCreateFlags flags )const override;
		/**
		*\copydoc	renderer::Device::createEvent
		*/
		renderer::EventPtr createEvent()const override;
		/**
		*\copydoc	renderer::Device::createCommandPool
		*/
		renderer::CommandPoolPtr createCommandPool( uint32_t queueFamilyIndex
			, renderer::CommandPoolCreateFlags const & flags )const override;
		/**
		*\copydoc	renderer::Device::createShaderProgram
		*/
		virtual renderer::ShaderModulePtr createShaderModule( renderer::ShaderStageFlag stage )const override;
		/**
		*\copydoc	renderer::Device::createQueryPool
		*/
		renderer::QueryPoolPtr createQueryPool( renderer::QueryType type
			, uint32_t count
			, renderer::QueryPipelineStatisticFlags pipelineStatistics )const override;
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

		inline ID3D11Device1 * getDevice1()const
		{
			return m_device1;
		}

		inline ID3D11Device2 * getDevice2()const
		{
			return m_device2;
		}

		inline ID3D11Device3 * getDevice3()const
		{
			return m_device3;
		}

		inline ID3D11Device4 * getDevice4()const
		{
			return m_device4;
		}

		inline ID3D11Device5 * getDevice5()const
		{
			return m_device5;
		}

		inline renderer::BufferBase const & getEmptyIndexedVaoIdx()const
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
		ID3D11Device1 * m_device1;
		ID3D11Device2 * m_device2;
		ID3D11Device3 * m_device3;
		ID3D11Device4 * m_device4;
		ID3D11Device5 * m_device5;
		ID3D11DeviceContext * m_deviceContext;
		D3D_FEATURE_LEVEL m_featureLevel;
		HANDLE m_idleEvent;
#if !defined( NDEBUG )
		ID3D11Debug * m_debug;
#endif
		// Mimic the behavior in Vulkan, when no IBO nor VBO is bound.
		renderer::BufferPtr< uint32_t > m_dummyIndexed;
	};
}
