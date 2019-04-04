/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Core/GlContext.hpp"
#include "Gl4Renderer/Core/GlPhysicalDevice.hpp"

namespace ashes::gl4
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
		Device( VkInstance instance
			, VkPhysicalDevice gpu
			, Context & context
			, VkDeviceCreateInfo createInfos );
		~Device();
		/**
		*\copydoc	Device::getImageSubresourceLayout
		*/
		void getImageSubresourceLayout( VkImage image
			, VkImageSubresource const & subresource
			, VkSubresourceLayout & layout )const;
		/**
		*\copydoc	Device::createQueryPool
		*/
		VkResult waitIdle()const;
		/**
		*\copydoc	Device::debugMarkerSetObjectName
		*/
		void debugMarkerSetObjectName( VkDebugMarkerObjectNameInfoEXT const & nameInfo )const;
		/**
		*\copydoc	Device::getQueue
		*/
		VkQueue getQueue( uint32_t familyIndex
			, uint32_t index )const;
		/**
		*\brief
		*	Echange les tampons.
		*/
		void swapBuffers()const;

		void registerContext( VkSurfaceKHR surface )const;
		void unregisterContext( VkSurfaceKHR surface )const;

		inline ContextLock getContext()const
		{
			return { *m_currentContext };
		}

		inline void setCurrentFramebuffer( GLuint fbo )const
		{
			m_fbo = fbo;
		}

		inline VkPhysicalDeviceFeatures const & getEnabledFeatures()const
		{
			return *m_createInfos.pEnabledFeatures;
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

		inline VkBuffer getEmptyIndexedVaoIdx()const
		{
			return m_dummyIndexed.indexBuffer;
		}

		inline GLuint getBlitSrcFbo()const
		{
			return m_blitFbos[0];
		}

		inline GLuint getBlitDstFbo()const
		{
			return m_blitFbos[1];
		}

		inline VkInstance getInstance()const
		{
			return m_instance;
		}

	private:
		void doInitialiseContext( ContextLock const & context )const;
		void doCreateQueues();

	private:
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkDeviceCreateInfo m_createInfos;
		VkQueueCreateCountMap m_queues;
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
			VkBuffer indexBuffer;
			VkBuffer vertexBuffer;
			GeometryBuffersPtr geometryBuffers;
		} m_dummyIndexed;
		mutable VkRect2D m_scissor{ 0, 0, 0, 0 };
		mutable VkViewport m_viewport{ 0, 0, 0, 0 };
		mutable VkPipelineColorBlendAttachmentStateArray m_cbStateAttachments;
		mutable VkPipelineColorBlendStateCreateInfo m_cbState;
		mutable VkPipelineDepthStencilStateCreateInfo m_dsState;
		mutable VkPipelineMultisampleStateCreateInfo m_msState;
		mutable VkPipelineRasterizationStateCreateInfo m_rsState;
		mutable VkPipelineTessellationStateCreateInfo m_tsState;
		mutable VkPipelineInputAssemblyStateCreateInfo m_iaState;
		mutable GLuint m_currentProgram;
		mutable GLuint m_blitFbos[2];
		mutable GLuint m_fbo{ 0u };
	};
}
