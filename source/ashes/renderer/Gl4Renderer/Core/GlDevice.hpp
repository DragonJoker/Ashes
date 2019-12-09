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
	class Device
		: public AutoIdIcdObject< Device >
	{
		friend class Instance;

	public:
		Device( VkInstance instance
			, VkPhysicalDevice gpu
			, VkDeviceCreateInfo createInfos );
		~Device();
		VkPhysicalDeviceLimits const & getLimits()const;
		void getImageSubresourceLayout( VkImage image
			, VkImageSubresource const & subresource
			, VkSubresourceLayout & layout )const;
		VkResult waitIdle()const;
#if VK_EXT_debug_utils
		VkResult setDebugUtilsObjectName( VkDebugUtilsObjectNameInfoEXT const & nameInfo )const;
		VkResult setDebugUtilsObjectTag( VkDebugUtilsObjectTagInfoEXT const & tagInfo )const;
		void submitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const;
#endif
#if VK_EXT_debug_marker
		VkResult debugMarkerSetObjectTag( VkDebugMarkerObjectTagInfoEXT const & nameInfo )const;
		VkResult debugMarkerSetObjectName( VkDebugMarkerObjectNameInfoEXT const & nameInfo )const;
#endif
#if VK_EXT_debug_report
		void reportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage );
#endif
		VkQueue getQueue( uint32_t familyIndex
			, uint32_t index )const;
		void swapBuffers()const;

		void link( VkSurfaceKHR surface )const;
		void unlink( VkSurfaceKHR surface )const;

		inline ContextLock getContext()const
		{
			assert( m_currentContext );
			return { *m_currentContext };
		}

		inline VkPhysicalDeviceFeatures const & getEnabledFeatures()const
		{
			return m_enabledFeatures;
		}

		inline GeometryBuffers & getEmptyIndexedVao()const
		{
			doInitialiseDummy();
			return *m_dummyIndexed.geometryBuffers;
		}

		inline VkBuffer getEmptyIndexedVaoIdx()const
		{
			doInitialiseDummy();
			return m_dummyIndexed.indexBuffer;
		}

		inline VkFramebuffer getBlitSrcFbo()const
		{
			return m_blitFbos[0];
		}

		inline VkFramebuffer getBlitDstFbo()const
		{
			return m_blitFbos[1];
		}

		inline VkInstance getInstance()const
		{
			return m_instance;
		}

		inline VkPhysicalDevice getPhysicalDevice()const
		{
			return m_physicalDevice;
		}

		inline VkSampler getSampler()const
		{
			return m_sampler;
		}

	private:
		void doInitialiseQueues();
		void doInitialiseDummy()const;

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
		VkPhysicalDeviceFeatures m_enabledFeatures{};
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
			VkBuffer indexBuffer{};
			VkBuffer vertexBuffer{};
			VkDeviceMemory indexMemory{};
			VkDeviceMemory vertexMemory{};
			GeometryBuffersPtr geometryBuffers;
		} m_dummyIndexed;
		mutable VkFramebuffer m_blitFbos[2]{};
		mutable VkSampler m_sampler{};
		VkPipelineColorBlendAttachmentStateArray m_cbStateAttachments;
		VkDynamicStateArray m_dyState;
	};
}
