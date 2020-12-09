/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

#include "renderer/GlRenderer/Miscellaneous/GlExtensionsHandler.hpp"

#include <set>

namespace ashes::gl
{
	class RenderWindow;

	class Instance
		: public ashes::IcdObject
	{
	public:
		Instance( VkInstanceCreateInfo createInfo );
		~Instance();

		uint32_t getApiVersion()const;
		bool hasExtension( std::string_view extension )const;
		void unregisterDevice( VkDevice device );
		Context * registerDevice( VkDevice device );
		void unregisterSurface( VkSurfaceKHR surface );
		ContextPtr registerSurface( VkSurfaceKHR surface );
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
		std::array< float, 16 > infinitePerspective( float radiansFovY
			, float aspect
			, float zNear )const;

#if VK_EXT_debug_utils
		void registerDebugMessenger( VkDebugUtilsMessengerEXT messenger
			, PFNGLDEBUGPROC callback
			, void * userParam )const;
		void registerDebugMessengerAMD( VkDebugUtilsMessengerEXT messenger
			, PFNGLDEBUGAMDPROC callback
			, void * userParam )const;
		void submitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const;
#endif
#if VK_EXT_debug_report
		void registerDebugMessageCallback( VkDebugReportCallbackEXT report
			, PFNGLDEBUGPROC callback
			, void * userParam )const;
		void registerDebugMessageCallbackAMD( VkDebugReportCallbackEXT report
			, PFNGLDEBUGAMDPROC callback
			, void * userParam )const;
		void reportMessage( VkDebugReportFlagsEXT flags
			, VkDebugReportObjectTypeEXT objectType
			, uint64_t object
			, size_t location
			, int32_t messageCode
			, const char * pLayerPrefix
			, const char * pMessage );
#endif
		void registerContext( Context & context );

		static VkPhysicalDeviceMemoryProperties const & getMemoryProperties();
#ifdef VK_KHR_get_physical_device_properties2
		static VkPhysicalDeviceMemoryProperties2KHR const & getMemoryProperties2();
#endif

		inline bool isSPIRVSupported()const
		{
			return m_extensions.isSPIRVSupported();
		}

		inline bool isValidationEnabled()const
		{
			return m_validationEnabled;
		}

		inline bool hasViewportArray()const
		{
			return m_hasViewportArray;
		}

#if VK_EXT_debug_utils

		inline std::vector< DebugUtilsMessengerData > const & getDebugMessengers()const
		{
			return m_debugMessengers;
		}

		inline std::vector< DebugUtilsAMDMessengerData > const & getDebugAMDMessengers()const
		{
			return m_debugAMDMessengers;
		}

#endif
#if VK_EXT_debug_report

		inline std::vector< DebugReportCallbackData > const & getDebugCallbacks()const
		{
			return m_debugCallbacks;
		}

		inline std::vector< DebugReportAMDCallbackData > const & getDebugAMDCallbacks()const
		{
			return m_debugAMDCallbacks;
		}

#endif

		inline ExtensionsHandler const & getExtensions()const
		{
			return m_extensions;
		}

		inline AshPluginFeatures const & getFeatures()const
		{
			return m_features;
		}

		inline Context & getCurrentContext()const
		{
			assert( m_context || m_firstSurfaceContext );
			return m_context
				? *m_context.get()
				: *m_firstSurfaceContext;
		}

		inline bool hasClearTexImage()const
		{
			return m_features.hasClearTexImage;
		}

	private:
		VkInstanceCreateFlags m_flags;
		VkApplicationInfo m_applicationInfo;
		StringArray m_enabledLayerNames;
		StringArray m_enabledExtensions;
		gl::RenderWindow * m_window{ nullptr };
		VkPhysicalDeviceArray m_physicalDevices;
		bool m_hasViewportArray;
		ExtensionsHandler m_extensions;
		AshPluginFeatures m_features;
		bool m_validationEnabled;
		ContextPtr m_context;
		Context * m_firstSurfaceContext{ nullptr };
		std::set< VkSurfaceKHR > m_surfaces;
		std::set< VkDevice > m_devices;
#if VK_EXT_debug_utils
		mutable std::vector< DebugUtilsMessengerData > m_debugMessengers;
		mutable std::vector< DebugUtilsAMDMessengerData > m_debugAMDMessengers;
#endif
#if VK_EXT_debug_report
		mutable std::vector< DebugReportCallbackData > m_debugCallbacks;
		mutable std::vector< DebugReportAMDCallbackData > m_debugAMDCallbacks;
#endif
	};
}
