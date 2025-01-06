/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Core/XBoxDeviceContextLock.hpp"
#include "renderer/XBoxRenderer/Core/XBoxLayer.hpp"

#include "renderer/XBoxRenderer/Buffer/XBoxBuffer.hpp"
#include "renderer/XBoxRenderer/Core/XBoxSurface.hpp"

#include <renderer/RendererCommon/IcdObject.hpp>

namespace ashes::xbox
{
	VkExtent3D getTexelBlockExtent( VkFormat format );
	uint32_t getTexelBlockByteSize( VkExtent3D const & texelBlockExtent
		, VkFormat format );

	template< typename T >
	T getSubresourceValue( T value, uint32_t mipLevel )
	{
		return T( value >> mipLevel );
	};

	class Device
		: public ashes::IcdObject
	{
		using lock_type = std::unique_lock< std::mutex >;

	public:
		Device( VkInstance instance
			, VkPhysicalDevice physicalDevice
			, VkAllocationCallbacks const * callbacks
			, VkDeviceCreateInfo createInfos );
		~Device()noexcept;

		DeviceContextLock getImmediateContext()const;

		bool hasExtension( std::string_view extension )const;
		VkPhysicalDeviceLimits const & getLimits()const;
		VkImage getStagingImage( VkImage image
			, VkDeviceMemory & memory );
		void getImageSubresourceLayout( VkImage image
			, VkImageSubresource const & subresource
			, VkSubresourceLayout & layout )const;
#if VK_EXT_debug_utils
		VkResult setDebugUtilsObjectName( VkDebugUtilsObjectNameInfoEXT const & nameInfo )const;
		VkResult setDebugUtilsObjectTag( VkDebugUtilsObjectTagInfoEXT const & tagInfo )const;
		void submitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const;
		void onSubmitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
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
			, const char * pMessage )const noexcept;
		void onReportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage )const noexcept;
#endif
		VkQueue getQueue( uint32_t familyIndex
			, uint32_t index )const;
		VkResult waitIdle()const;
		/**
		*name
		*	Layers delegation.
		*/
		/**@{*/
		void onCopyToImageCommand( VkCommandBuffer cmd
			, ArrayView< VkBufferImageCopy const > const & copyInfo
			, VkBuffer src
			, VkImage dst )const;
		void onCheckHResultCommand( HRESULT hresult
			, std::string const & message )const;
		/**@}*/
		/**
		*name
		*	Getters.
		*/
		/**@{*/
		VkInstance getInstance()const
		{
			return m_instance;
		}

		D3D_FEATURE_LEVEL getFeatureLevel()const
		{
			return m_featureLevel;
		}

		ID3D11Device * getDevice()const
		{
			return m_d3dDevice;
		}

		VkBuffer getEmptyIndexedVaoIdx()const
		{
			return m_dummyIndexed.buffer;
		}

		VkPhysicalDevice getPhysicalDevice()const
		{
			return m_physicalDevice;
		}

		VkSampler getSampler()const
		{
			return m_sampler;
		}

		VkAllocationCallbacks const * getAllocationCallbacks()const
		{
			return m_callbacks;
		}

#if !defined( NDEBUG )

		ID3D11Debug * getDebug()
		{
			return m_debug;
		}

#endif
		/**@}*/

	private:
		void doCreateD3D11Device();
		void doCreateDummyIndexBuffer();
		void doCreateQueues();

		friend class DeviceContextLock;
		ID3D11DeviceContext * lockImmediateContext()const;
		void unlockImmediateContext()const noexcept;

	private:
		struct QueueCreates
		{
			QueueCreates( VkDeviceQueueCreateInfo createInfo
				, std::vector< VkQueue > queues = {}  )
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
		VkAllocationCallbacks const * m_callbacks;
		VkDeviceCreateInfo m_createInfos;
		ID3D11Device * m_d3dDevice;
		ID3D11Query * m_waitIdleQuery;
		D3D_FEATURE_LEVEL m_featureLevel;
		QueueCreateCountMap m_queues;
#if !defined( NDEBUG )
		ID3D11Debug * m_debug;
#endif
		// Mimic the behavior in Vulkan, when no IBO nor VBO is bound.
		struct Buffer
		{
			VkBuffer buffer;
			VkDeviceMemory memory;
		};
		Buffer m_dummyIndexed;
		VkSampler m_sampler;
		std::unordered_map< size_t, std::pair< VkImage, VkDeviceMemory > > m_stagingTextures;
		mutable std::mutex m_mtxDeviceContext;
		ID3D11DeviceContext * m_deviceContext;
	};
}
