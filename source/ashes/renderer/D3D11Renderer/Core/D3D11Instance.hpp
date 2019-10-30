/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "renderer/D3D11Renderer/Core/D3D11Layer.hpp"

#include <array>

namespace ashes::d3d11
{
	D3D_FEATURE_LEVEL getSupportedFeatureLevel();

	class Instance
	{
	public:
		/**
		*\brief
		*	Constructeur, initialise l'instance de Vulkan.
		*/
		Instance( VkInstanceCreateInfo createInfo );
		/**
		*\brief
		*	Destructeur.
		*/
		~Instance();

		VkPhysicalDeviceArray enumeratePhysicalDevices()const;
		/**
		*\copydoc	ashes::Instance::frustum
		*/
		std::array< float, 16 > frustum( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const;
		/**
		*\copydoc	ashes::Instance::perspective
		*/
		std::array< float, 16 > perspective( float radiansFovY
			, float aspect
			, float zNear
			, float zFar )const;
		/**
		*\copydoc	ashes::Instance::ortho
		*/
		std::array< float, 16 > ortho( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const;
		/**
		*\~english
		*name
		*	Layers delegation.
		*\~french
		*name
		*	D�l�gation aux layers.
		*/
		/**@{*/
		void registerLayer( Layer * layer )const;
		void unregisterLayer( Layer * layer )const;
		bool onCopyToImageCommand( VkCommandBuffer cmd
			, VkBufferImageCopyArray const & copyInfo
			, VkBuffer src
			, VkImage dst )const;
		bool onCheckHResultCommand( HRESULT hresult
			, std::string message )const;
#if VK_EXT_debug_utils
		void onSubmitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const;
#endif
#if VK_EXT_debug_report
		void onReportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage );
#endif
		/**@}*/

		inline IDXGIFactory * getDXGIFactory()const
		{
			return m_factory;
		}

		static inline VkPhysicalDeviceMemoryProperties const & getMemoryProperties()
		{
			return m_memoryProperties;
		}

	private:
		void doLoadAdapters();
		void doInitialisePhysicalDevices();

	private:
		AshPluginFeatures m_features;
		VkInstanceCreateFlags m_flags;
		StringArray m_enabledLayerNames;
		StringArray m_enabledExtensions;
		VkPhysicalDeviceArray m_physicalDevices;
		IDXGIFactory * m_factory;
		std::vector< AdapterInfo > m_adapters;
		D3D_FEATURE_LEVEL m_maxFeatureLevel;
		mutable std::vector< Layer * > m_layers;
		static VkPhysicalDeviceMemoryProperties const m_memoryProperties;
	};
}
