/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/Command/GlCommandBuffer.hpp"
#include "renderer/Gl4Renderer/Core/GlContextLock.hpp"
#include "renderer/Gl4Renderer/Core/GlPhysicalDevice.hpp"

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
		GLuint getRtocProgram()const;

		inline ContextLock getContext()const
		{
			return { *m_currentContext };
		}

		inline VkPhysicalDeviceFeatures const & getEnabledFeatures()const
		{
			return *m_createInfos.pEnabledFeatures;
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

		ContextState const & getRtocContextState()const
		{
			return m_rtocContextState;
		}

	private:
		void doInitialiseQueues();
		void doInitialiseDummy( ContextLock & context );
		void doInitialiseRtoc( ContextLock & context );

	private:
		struct QueueCreates
		{
			VkDeviceQueueCreateInfo createInfo;
			std::vector< VkQueue > queues;
		};
		using QueueCreateCountMap = std::map< uint32_t, QueueCreates >;

	private:
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkDeviceCreateInfo m_createInfos;
		QueueCreateCountMap m_queues;
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
			VkDeviceMemory indexMemory;
			VkDeviceMemory vertexMemory;
			GeometryBuffersPtr geometryBuffers;
		} m_dummyIndexed;
		mutable GLuint m_blitFbos[2];
		VkPipelineColorBlendAttachmentStateArray m_cbStateAttachments;
		VkDynamicStateArray m_dyState;
		ContextState m_rtocContextState;
		std::unique_ptr< ShaderProgram > m_rtocProgram;
		CommandBuffer m_rtocCommands;
	};
}
