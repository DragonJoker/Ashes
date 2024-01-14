/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Core/TestLayer.hpp"

#include "renderer/TestRenderer/Buffer/TestBuffer.hpp"
#include "renderer/TestRenderer/Core/TestSurface.hpp"

#include <renderer/RendererCommon/IcdObject.hpp>

#include <unordered_map>

namespace ashes::test
{
	class Device
		: public ashes::IcdObject
		, public NonCopyable
	{
	public:
		Device( VkInstance instance
			, VkPhysicalDevice physicalDevice
			, VkDeviceCreateInfo createInfos );
		~Device()noexcept;

		bool hasExtension( std::string_view extension )const;
		VkPhysicalDeviceLimits const & getLimits()const;
		VkImage getStagingImage( VkImage image
			, VkDeviceMemory & memory );
		/**
		*\copydoc	ashes::Device::getImageSubresourceLayout
		*/
		void getImageSubresourceLayout( VkImage image
			, VkImageSubresource const & subresource
			, VkSubresourceLayout & layout )const;
#if VK_EXT_debug_utils
		/**
		*\copydoc	ashes::Device::setDebugUtilsObjectName
		*/
		VkResult setDebugUtilsObjectName( VkDebugUtilsObjectNameInfoEXT const & nameInfo )const;
		/**
		*\copydoc	ashes::Device::setDebugUtilsObjectTag
		*/
		VkResult setDebugUtilsObjectTag( VkDebugUtilsObjectTagInfoEXT const & tagInfo )const;
#endif
#if VK_EXT_debug_marker
		/**
		*\copydoc	ashes::Device::debugMarkerSetObjectTag
		*/
		VkResult debugMarkerSetObjectTag( VkDebugMarkerObjectTagInfoEXT const & nameInfo )const;
		/**
		*\copydoc	ashes::Device::debugMarkerSetObjectName
		*/
		VkResult debugMarkerSetObjectName( VkDebugMarkerObjectNameInfoEXT const & nameInfo )const;
#endif
#if VK_EXT_debug_report
		/**
		*\copydoc	ashes::Device::reportMessage
		*/
		void reportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage );
#endif
		/**
		*\copydoc	ashes::Device::getQueue
		*/
		VkQueue getQueue( uint32_t familyIndex
			, uint32_t index )const;
		/**
		*\brief
		*	Attend que le périphérique soit inactif.
		*/
		VkResult waitIdle()const;
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
			, VkBufferImageCopyArray const & copyInfo
			, VkBuffer src
			, VkImage dst )const;
		void onReportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage );
		/**@}*/

		VkInstance getInstance()const noexcept
		{
			return m_instance;
		}

		VkBuffer getEmptyIndexedVaoIdx()const noexcept
		{
			return m_dummyIndexed.buffer;
		}

		VkPhysicalDevice getGpu()const noexcept
		{
			return m_physicalDevice;
		}

	private:
		void doCreateDummyIndexBuffer();
		void doCreateQueues();

	private:
		struct QueueCreates
		{
			QueueCreates( VkDeviceQueueCreateInfo createInfo
				, std::vector< VkQueue > queues = {} )
				: createInfo{ std::move( createInfo ) }
				, queues{ std::move( queues ) }
			{
			}

			VkDeviceQueueCreateInfo createInfo;
			std::vector< VkQueue > queues;
		};
		using QueueCreateCountMap = std::map< uint32_t, QueueCreates >;

	private:
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkDeviceCreateInfo m_createInfos;
		QueueCreateCountMap m_queues;
		// Mimic the behavior in Vulkan, when no IBO nor VBO is bound.
		struct Buffer
		{
			VkBuffer buffer;
			VkDeviceMemory memory;
		};
		Buffer m_dummyIndexed;
		std::unordered_map< size_t, std::pair< VkImage, VkDeviceMemory > > m_stagingTextures;
	};
}
