/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "renderer/D3D11Renderer/Core/D3D11Layer.hpp"

#include <renderer/RendererCommon/IcdObject.hpp>

#include <array>

namespace ashes::d3d11
{
	D3D_FEATURE_LEVEL getSupportedFeatureLevel();

	class Instance
		: public ashes::IcdObject
		, public NonCopyable
	{
	public:
		explicit Instance( VkInstanceCreateInfo const & createInfo );
		~Instance()noexcept;

		uint32_t getApiVersion()const;
		bool hasExtension( std::string_view extension )const;
		VkPhysicalDeviceArray enumeratePhysicalDevices()const;
		std::array< float, 16 > frustum( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const;
		std::array< float, 16 > perspective( float radiansFovY
			, float aspect
			, float zNear
			, float zFar )const;
		std::array< float, 16 > ortho( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const;
		/**
		*\name
		*	Layers delegation.
		*/
		/**@{*/
		void registerLayer( Layer * layer )const;
		void unregisterLayer( Layer * layer )const noexcept;
		void onCopyToImageCommand( VkCommandBuffer cmd
			, ArrayView< VkBufferImageCopy const > const & copyInfo
			, VkBuffer src
			, VkImage dst )const noexcept;
		void onCheckHResultCommand( HRESULT hresult
			, std::string const & message )const noexcept;
#if VK_EXT_debug_utils
		void onSubmitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const noexcept;
		void submitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const noexcept;
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
		/**@}*/

		static VkPhysicalDeviceMemoryProperties const & getMemoryProperties()noexcept;
#ifdef VK_KHR_get_physical_device_properties2
		static VkPhysicalDeviceMemoryProperties2KHR const & getMemoryProperties2()noexcept;
#endif

		inline IDXGIFactory * getDXGIFactory()const
		{
			return m_factory;
		}

	private:
		void doLoadAdapters();
		void doInitialisePhysicalDevices();

	private:
		AshPluginFeatures m_features;
		VkInstanceCreateFlags m_flags;
		VkApplicationInfo m_applicationInfo;
		StringArray m_enabledLayerNames;
		StringArray m_enabledExtensions;
		VkPhysicalDeviceArray m_physicalDevices;
		IDXGIFactory * m_factory;
		std::vector< AdapterInfo > m_adapters;
		D3D_FEATURE_LEVEL m_maxFeatureLevel;
		mutable std::vector< Layer * > m_layers;
	};
}
