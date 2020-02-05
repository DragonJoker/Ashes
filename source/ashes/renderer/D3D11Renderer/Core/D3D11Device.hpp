/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Core/D3D11Layer.hpp"

#include "renderer/D3D11Renderer/Buffer/D3D11Buffer.hpp"
#include "renderer/D3D11Renderer/Core/D3D11Surface.hpp"

namespace ashes::d3d11
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
	{
	public:
		Device( VkInstance instance
			, VkPhysicalDevice physicalDevice
			, VkDeviceCreateInfo createInfos );
		~Device();

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
			, const char * pMessage );
		void onReportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage );
#endif
		VkQueue getQueue( uint32_t familyIndex
			, uint32_t index )const;
		VkResult waitIdle()const;
		/**
		*name
		*	Layers delegation.
		*/
		/**@{*/
		bool onCopyToImageCommand( VkCommandBuffer cmd
			, ArrayView< VkBufferImageCopy const > const & copyInfo
			, VkBuffer src
			, VkImage dst )const;
		bool onCheckHResultCommand( HRESULT hresult
			, std::string message )const;
		/**@}*/
		/**
		*name
		*	Getters.
		*/
		/**@{*/
		inline VkInstance getInstance()const
		{
			return m_instance;
		}

		inline D3D_FEATURE_LEVEL getFeatureLevel()const
		{
			return m_featureLevel;
		}

		inline ID3D11Device * getDevice()const
		{
			return m_d3dDevice;
		}

		inline VkBuffer getEmptyIndexedVaoIdx()const
		{
			return m_dummyIndexed.buffer;
		}

		inline VkPhysicalDevice getPhysicalDevice()const
		{
			return m_physicalDevice;
		}

		inline VkSampler getSampler()const
		{
			return m_sampler;
		}

#if !defined( NDEBUG )

		inline ID3D11Debug * getDebug()
		{
			return m_debug;
		}

#endif
		/**@}*/

	private:
		void doCreateD3D11Device();
		void doCreateDummyIndexBuffer();
		void doCreateQueues();

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
		ID3D11Device * m_d3dDevice;
		ID3D11DeviceContext * m_deviceContext;
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
	};
}
