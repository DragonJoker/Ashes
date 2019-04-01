/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Core/GlContext.hpp"
#include "Gl4Renderer/Core/GlPhysicalDevice.hpp"

#include <Ashes/Buffer/VertexBuffer.hpp>
#include <Ashes/Core/Device.hpp>
#include <Ashes/Miscellaneous/SwapChainCreateInfo.hpp>
#include <Ashes/Pipeline/ColourBlendState.hpp>
#include <Ashes/Pipeline/DepthStencilState.hpp>
#include <Ashes/Pipeline/InputAssemblyState.hpp>
#include <Ashes/Pipeline/MultisampleState.hpp>
#include <Ashes/Pipeline/RasterisationState.hpp>
#include <Ashes/Pipeline/Scissor.hpp>
#include <Ashes/Pipeline/TessellationState.hpp>
#include <Ashes/Pipeline/Viewport.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Classe contenant les informations liées au GPU logique.
	*/
	class Device
	{
		friend class Instance;

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] renderer
		*	L'instance.
		*\param[in] connection
		*	La connection à l'application.
		*/
		Device( Instance const & instance
			, PhysicalDevice const & gpu
			, Context & context
			, VkDeviceCreateInfo createInfos );
		~Device();
		/**
		*\copydoc		ashes::Device::createRenderPass
		*/
		ashes::RenderPassPtr createRenderPass( VkRenderPassCreateInfo createInfo )const;
		/**
		*\copydoc		ashes::Device::createPipelineLayout
		*/
		ashes::PipelineLayoutPtr createPipelineLayout( ashes::DescriptorSetLayoutCRefArray const & setLayouts
			, ashes::PushConstantRangeArray const & pushConstantRanges )const;
		/**
		*\copydoc		ashes::Device::createDescriptorSetLayout
		*/
		ashes::DescriptorSetLayoutPtr createDescriptorSetLayout( ashes::DescriptorSetLayoutBindingArray bindings )const;
		/**
		*\copydoc	ashes::Device::createDescriptorPool
		*/
		ashes::DescriptorPoolPtr createDescriptorPool( VkDescriptorPoolCreateFlags flags
			, uint32_t maxSets
			, ashes::DescriptorPoolSizeArray poolSizes )const;
		/**
		*\copydoc	ashes::Device::allocateMemory
		*/
		ashes::DeviceMemoryPtr allocateMemory( VkMemoryAllocateInfo allocateInfo )const;
		/**
		*\copydoc		ashes::Device::createImage
		*/
		ashes::ImagePtr createImage( VkImageCreateInfo const & createInfo )const;
		/**
		*\copydoc	ashes::Device::getImageSubresourceLayout
		*/
		void getImageSubresourceLayout( Image const & image
			, VkImageSubresource const & subresource
			, VkSubresourceLayout & layout )const;
		/**
		*\copydoc		ashes::Device::createSampler
		*/
		ashes::SamplerPtr createSampler( VkSamplerCreateInfo const & createInfo )const;
		/**
		*\copydoc		ashes::Device::createBuffer
		*/
		ashes::BufferBasePtr createBuffer( uint32_t size
			, VkBufferUsageFlags targets )const;
		/**
		*\copydoc		ashes::Device::createBufferView
		*/
		ashes::BufferViewPtr createBufferView( ashes::BufferBase const & buffer
			, VkFormat format
			, uint32_t offset
			, uint32_t range )const;
		/**
		*\copydoc		ashes::Device::createSwapChain
		*/
		ashes::SwapChainPtr createSwapChain( VkSwapchainCreateInfoKHR createInfo )const;
		/**
		*\copydoc		ashes::Device::createSemaphore
		*/
		ashes::SemaphorePtr createSemaphore()const;
		/**
		*\copydoc	ashes::Device::createFence
		*/
		ashes::FencePtr createFence( VkFenceCreateFlags flags = 0 )const;
		/**
		*\copydoc	ashes::Device::createEvent
		*/
		ashes::EventPtr createEvent()const;
		/**
		*\copydoc		ashes::Device::createCommandPool
		*/
		ashes::CommandPoolPtr createCommandPool( uint32_t queueFamilyIndex
			, VkCommandPoolCreateFlags const & flags )const;
		/**
		*\copydoc		ashes::Device::createShaderProgram
		*/
		virtual ashes::ShaderModulePtr createShaderModule( VkShaderStageFlagBits stage )const;
		/**
		*\copydoc	ashes::Device::createQueryPool
		*/
		ashes::QueryPoolPtr createQueryPool( ashes::QueryType type
			, uint32_t count
			, VkQueryPipelineStatisticFlags pipelineStatistics )const;
		/**
		*\copydoc	ashes::Device::createQueryPool
		*/
		void waitIdle()const;
		/**
		*\copydoc	ashes::Device::debugMarkerSetObjectName
		*/
		void debugMarkerSetObjectName( VkDebugMarkerObjectNameInfoEXT const & nameInfo )const;
		/**
		*\copydoc	ashes::Device::getQueue
		*/
		QueuePtr getQueue( uint32_t familyIndex
			, uint32_t index )const;
		/**
		*\brief
		*	Echange les tampons.
		*/
		void swapBuffers()const;

		void registerContext( ashes::WindowHandle const & handle )const;
		void unregisterContext( ashes::WindowHandle const & handle )const;

		inline ContextLock getContext()const
		{
			return { *m_currentContext };
		}

		inline void setCurrentFramebuffer( GLuint fbo )const
		{
			m_fbo = fbo;
		}

		inline GLuint getCurrentFramebuffer()const
		{
			return m_fbo;
		}

		inline VkRect2D & getCurrentScissor()const
		{
			return m_scissor;
		}

		inline VkViewport & getCurrentViewport()const
		{
			return m_viewport;
		}

		inline VkPipelineColorBlendStateCreateInfo & getCurrentBlendState()const
		{
			return m_cbState;
		}

		inline VkPipelineDepthStencilStateCreateInfo & getCurrentDepthStencilState()const
		{
			return m_dsState;
		}

		inline VkPipelineMultisampleStateCreateInfo & getCurrentMultisampleState()const
		{
			return m_msState;
		}

		inline VkPipelineRasterizationStateCreateInfo & getCurrentRasterisationState()const
		{
			return m_rsState;
		}

		inline VkPipelineTessellationStateCreateInfo & getCurrentTessellationState()const
		{
			return m_tsState;
		}

		inline VkPipelineInputAssemblyStateCreateInfo & getCurrentInputAssemblyState()const
		{
			return m_iaState;
		}

		inline GLuint & getCurrentProgram()const
		{
			return m_currentProgram;
		}

		inline GeometryBuffers & getEmptyIndexedVao()const
		{
			return *m_dummyIndexed.geometryBuffers;
		}

		inline ashes::BufferBase const & getEmptyIndexedVaoIdx()const
		{
			return m_dummyIndexed.indexBuffer->getBuffer();
		}

		inline GLuint getBlitSrcFbo()const
		{
			return m_blitFbos[0];
		}

		inline GLuint getBlitDstFbo()const
		{
			return m_blitFbos[1];
		}

		inline Instance const & getInstance()const
		{
			return m_instance;
		}

	private:
		void doInitialiseContext( ContextLock const & context )const;
		void doCreateQueues();

	private:
		Instance const & m_instance;
		mutable ContextPtr m_ownContext;
		mutable Context * m_currentContext;
		struct Vertex
		{
			float x;
			float y;
			float z;
		};
		// Mimic the behavior in Vulkan, when no IBO nor VBO is bound.
		mutable struct
		{
			ashes::BufferPtr< uint32_t > indexBuffer;
			ashes::VertexBufferPtr< Vertex > vertexBuffer;
			GeometryBuffersPtr geometryBuffers;
		} m_dummyIndexed;
		mutable VkRect2D m_scissor{ 0, 0, 0, 0 };
		mutable VkViewport m_viewport{ 0, 0, 0, 0 };
		mutable VkPipelineColorBlendStateCreateInfo m_cbState;
		mutable VkPipelineDepthStencilStateCreateInfo m_dsState;
		mutable VkPipelineMultisampleStateCreateInfo m_msState;
		mutable VkPipelineRasterizationStateCreateInfo m_rsState;
		mutable VkPipelineTessellationStateCreateInfo m_tsState;
		mutable VkPipelineInputAssemblyStateCreateInfo m_iaState;
		using QueueCreateCount = std::pair< ashes::DeviceQueueCreateInfo, uint32_t >;
		std::map< uint32_t, QueueCreateCount > m_queues;
		mutable GLuint m_currentProgram;
		mutable GLuint m_blitFbos[2];
		mutable GLuint m_fbo{ 0u };
	};
}
