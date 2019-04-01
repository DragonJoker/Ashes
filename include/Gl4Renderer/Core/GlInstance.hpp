/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "Gl4Renderer/Core/GlExtensionsHandler.hpp"

#include <Ashes/Core/Instance.hpp>

namespace gl_renderer
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
		*\copydoc	ashes::Instance::enumerateLayerProperties
		*/
		PhysicalDevicePtrArray enumeratePhysicalDevices()const;
		/**
		*\copydoc	ashes::Instance::createDevice
		*/
		ashes::DevicePtr createDevice( ashes::PhysicalDevice const & physicalDevice
			, VkDeviceCreateInfo createInfos )const;
		/**
		*\copydoc	ashes::Instance::createSurface
		*/
		SurfacePtr createSurface( PhysicalDevice const & gpu
			, WindowHandle handle )const;
		/**
		*\copydoc	ashes::Instance::createDebugReportCallback
		*/
		DebugReportCallbackPtr createDebugReportCallback( VkDebugReportCallbackCreateInfoEXT createInfo )const;
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
		*\copydoc	ashes::Instance::infinitePerspective
		*/
		std::array< float, 16 > infinitePerspective( float radiansFovY
			, float aspect
			, float zNear )const;

		void registerDebugMessageCallback( PFNGLDEBUGPROC callback, void * userParam )const;
		void registerDebugMessageCallbackAMD( PFNGLDEBUGAMDPROC callback, void * userParam )const;

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
		AshRendererFeatures m_features;
		VkInstanceCreateInfo m_createInfo;
		mutable std::vector< DebugReportCallbackData > m_debugCallbacks;
		mutable std::vector< DebugReportAMDCallbackData > m_debugAMDCallbacks;
		ExtensionsHandler m_extensions;
		bool m_validationEnabled;
		RenderWindow * m_dummyWindow;
		ContextPtr m_context;
		static VkPhysicalDeviceMemoryProperties const m_memoryProperties;
	};
}
