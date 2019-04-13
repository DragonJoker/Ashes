/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "Gl4Renderer/Core/GlExtensionsHandler.hpp"

namespace ashes::gl4
{
	class RenderWindow;

	class Instance
	{
	public:
		/**
		*\brief
		*	Constructeur, initialise l'instance de Vulkan.
		*/
		Instance( VkInstanceCreateInfo createInfo );
		~Instance();
		/**
		*\copydoc	Instance::enumerateLayerProperties
		*/
		VkPhysicalDeviceArray enumeratePhysicalDevices()const;
		/**
		*\copydoc	Instance::frustum
		*/
		std::array< float, 16 > frustum( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const;
		/**
		*\copydoc	Instance::perspective
		*/
		std::array< float, 16 > perspective( float radiansFovY
			, float aspect
			, float zNear
			, float zFar )const;
		/**
		*\copydoc	Instance::ortho
		*/
		std::array< float, 16 > ortho( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const;
		/**
		*\copydoc	Instance::infinitePerspective
		*/
		std::array< float, 16 > infinitePerspective( float radiansFovY
			, float aspect
			, float zNear )const;

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

		inline bool isSPIRVSupported()const
		{
			return m_extensions.isSPIRVSupported();
		}

		inline bool isValidationEnabled()const
		{
			return m_validationEnabled;
		}

		inline std::vector< DebugReportCallbackData > const & getDebugCallbacks()const
		{
			return m_debugCallbacks;
		}

		inline std::vector< DebugReportAMDCallbackData > const & getDebugAMDCallbacks()const
		{
			return m_debugAMDCallbacks;
		}

		inline ExtensionsHandler const & getExtensions()const
		{
			return m_extensions;
		}

		inline Context & getContext()const
		{
			return *m_context;
		}

		static inline VkPhysicalDeviceMemoryProperties const & getMemoryProperties()
		{
			return m_memoryProperties;
		}

	private:
		AshPluginFeatures m_features;
		VkInstanceCreateFlags m_flags;
		StringArray m_enabledLayerNames;
		StringArray m_enabledExtensions;
		VkPhysicalDeviceArray m_physicalDevices;
		mutable std::vector< DebugReportCallbackData > m_debugCallbacks;
		mutable std::vector< DebugReportAMDCallbackData > m_debugAMDCallbacks;
		ExtensionsHandler m_extensions;
		bool m_validationEnabled;
		RenderWindow * m_dummyWindow;
		ContextPtr m_context;
		static VkPhysicalDeviceMemoryProperties const m_memoryProperties;
	};
}
