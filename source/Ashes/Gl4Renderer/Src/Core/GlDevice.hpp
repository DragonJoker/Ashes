/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Core/GlContext.hpp"
#include "Core/GlPhysicalDevice.hpp"

#include <Buffer/VertexBuffer.hpp>
#include <Core/Device.hpp>
#include <Pipeline/ColourBlendState.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/RasterisationState.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/TessellationState.hpp>
#include <Pipeline/Viewport.hpp>

namespace gl_renderer
{
	class ContextLock
	{
	public:
		ContextLock( ContextLock const & ) = delete;
		ContextLock & operator=( ContextLock const & ) = delete;

		inline ContextLock( Context const & context )
			: m_context{ &context }
			, m_disable{ !context.isEnabled() }
		{
			if ( m_disable )
			{
				m_context->enable();
			}
		}

		ContextLock( ContextLock && rhs )
			: m_context{ rhs.m_context }
			, m_disable{ rhs.m_disable }
		{
			rhs.m_context = nullptr;
		}

		ContextLock & operator=( ContextLock && rhs )
		{
			if ( &rhs != this )
			{
				m_context = rhs.m_context;
				m_disable = rhs.m_disable;
				rhs.m_context = nullptr;
			}

			return *this;
		}

		inline ~ContextLock()
		{
			if ( m_context && m_disable )
			{
				m_context->disable();
			}
		}

		Context const * operator->()const
		{
			return m_context;
		}

	private:
		Context const * m_context;
		bool m_disable;
	};
	/**
	*\brief
	*	Classe contenant les informations liées au GPU logique.
	*/
	class Device
		: public ashes::Device
	{
		friend class Renderer;

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] renderer
		*	L'instance de Renderer.
		*\param[in] connection
		*	La connection à l'application.
		*/
		Device( Renderer const & renderer
			, PhysicalDevice const & gpu
			, ashes::ConnectionPtr && connection );
		~Device();
		/**
		*\copydoc	ashes::Device::createStagingTexture
		*/
		ashes::StagingTexturePtr createStagingTexture( ashes::Format format
			, ashes::Extent2D const & extent )const;
		/**
		*\copydoc		ashes::Device::createRenderPass
		*/
		ashes::RenderPassPtr createRenderPass( ashes::RenderPassCreateInfo createInfo )const override;
		/**
		*\copydoc		ashes::Device::createPipelineLayout
		*/
		ashes::PipelineLayoutPtr createPipelineLayout( ashes::DescriptorSetLayoutCRefArray const & setLayouts
			, ashes::PushConstantRangeArray const & pushConstantRanges )const override;
		/**
		*\copydoc		ashes::Device::createDescriptorSetLayout
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
		*\copydoc		ashes::Device::createTexture
		*/
		ashes::TexturePtr createTexture( ashes::ImageCreateInfo const & createInfo )const override;
		/**
		*\copydoc	ashes::Device::getImageSubresourceLayout
		*/
		void getImageSubresourceLayout( ashes::Texture const & image
			, ashes::ImageSubresource const & subresource
			, ashes::SubresourceLayout & layout )const override;
		/**
		*\copydoc		ashes::Device::createSampler
		*/
		ashes::SamplerPtr createSampler( ashes::SamplerCreateInfo const & createInfo )const override;
		/**
		*\copydoc		ashes::Device::createBuffer
		*/
		ashes::BufferBasePtr createBuffer( uint32_t size
			, ashes::BufferTargets targets )const override;
		/**
		*\copydoc		ashes::Device::createBufferView
		*/
		ashes::BufferViewPtr createBufferView( ashes::BufferBase const & buffer
			, ashes::Format format
			, uint32_t offset
			, uint32_t range )const override;
		/**
		*\copydoc		ashes::Device::createUniformBuffer
		*/
		ashes::UniformBufferBasePtr createUniformBuffer( uint32_t count
			, uint32_t size
			, ashes::BufferTargets target
			, ashes::MemoryPropertyFlags memoryFlags )const override;
		/**
		*\copydoc		ashes::Device::createSwapChain
		*/
		ashes::SwapChainPtr createSwapChain( ashes::Extent2D const & size )const override;
		/**
		*\copydoc		ashes::Device::createSemaphore
		*/
		ashes::SemaphorePtr createSemaphore()const override;
		/**
		*\copydoc	ashes::Device::createFence
		*/
		ashes::FencePtr createFence( ashes::FenceCreateFlags flags = 0 )const override;
		/**
		*\copydoc	ashes::Device::createEvent
		*/
		ashes::EventPtr createEvent()const override;
		/**
		*\copydoc		ashes::Device::createCommandPool
		*/
		ashes::CommandPoolPtr createCommandPool( uint32_t queueFamilyIndex
			, ashes::CommandPoolCreateFlags const & flags )const override;
		/**
		*\copydoc		ashes::Device::createShaderProgram
		*/
		virtual ashes::ShaderModulePtr createShaderModule( ashes::ShaderStageFlag stage )const override;
		/**
		*\copydoc	ashes::Device::createQueryPool
		*/
		ashes::QueryPoolPtr createQueryPool( ashes::QueryType type
			, uint32_t count
			, ashes::QueryPipelineStatisticFlags pipelineStatistics )const override;
		/**
		*\copydoc	ashes::Device::createQueryPool
		*/
		void waitIdle()const override;
		/**
		*\copydoc	ashes::Device::debugMarkerSetObjectName
		*/
		void debugMarkerSetObjectName( ashes::DebugMarkerObjectNameInfo const & nameInfo )const override;
		/**
		*\brief
		*	Echange les tampons.
		*/
		void swapBuffers()const;

		inline ContextLock getContext()const
		{
			return { *m_context };
		}

		inline void setCurrentFramebuffer( GLuint fbo )const
		{
			m_fbo = fbo;
		}

		inline GLuint getCurrentFramebuffer()const
		{
			return m_fbo;
		}

		inline ashes::Scissor & getCurrentScissor()const
		{
			return m_scissor;
		}

		inline ashes::Viewport & getCurrentViewport()const
		{
			return m_viewport;
		}

		inline ashes::ColourBlendState & getCurrentBlendState()const
		{
			return m_cbState;
		}

		inline ashes::DepthStencilState & getCurrentDepthStencilState()const
		{
			return m_dsState;
		}

		inline ashes::MultisampleState & getCurrentMultisampleState()const
		{
			return m_msState;
		}

		inline ashes::RasterisationState & getCurrentRasterisationState()const
		{
			return m_rsState;
		}

		inline ashes::TessellationState & getCurrentTessellationState()const
		{
			return m_tsState;
		}

		inline ashes::InputAssemblyState & getCurrentInputAssemblyState()const
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

	private:
		Renderer const & m_renderer;
		ashes::ConnectionPtr m_connection;
		ContextPtr m_context;
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
		mutable ashes::Scissor m_scissor{ 0, 0, 0, 0 };
		mutable ashes::Viewport m_viewport{ 0, 0, 0, 0 };
		mutable ashes::ColourBlendState m_cbState;
		mutable ashes::DepthStencilState m_dsState;
		mutable ashes::MultisampleState m_msState;
		mutable ashes::RasterisationState m_rsState;
		mutable ashes::TessellationState m_tsState;
		mutable ashes::InputAssemblyState m_iaState;
		mutable GLuint m_currentProgram;
		GLuint m_blitFbos[2];
		mutable GLuint m_fbo{ 0u };
	};
}
