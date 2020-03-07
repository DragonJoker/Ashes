/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Instance_HPP___
#define ___AshesPP_Instance_HPP___
#pragma once

#include "ashespp/Core/DeviceCreateInfo.hpp"
#include "ashespp/Core/InstanceCreateInfo.hpp"
#include "ashespp/Core/RendererList.hpp"
#include "ashespp/Core/PhysicalDevice.hpp"
#include "ashespp/Miscellaneous/RendererFeatures.hpp"

#include "ashespp/Core/WindowHandle.hpp"

namespace ashes
{
	/**
	*\brief
	*	The class initialising the rendering instance.
	*/
	class Instance
	{
	public:
		/**
		*\brief
		*	Constructor, initialises the rendering instance.
		*/
		Instance( AshPluginDescription plugin
			, ashes::InstanceCreateInfo createInfo );
		/**
		*\brief
		*	Destructor.
		*/
		~Instance();
		/**
		*\brief
		*	Lists the available physical GPUs.
		*/
		PhysicalDevicePtrArray enumeratePhysicalDevices()const;
		/**
		*\brief
		*	Creates a logical device.
		*\param[in] surface
		*	The presentation surface.
		*\param[in] createInfos
		*	The creation informations.
		*/
		DevicePtr createDevice( PhysicalDevice const & physicalDevice
			, ashes::DeviceCreateInfo createInfos )const;
		/**
		*\brief
		*	Creates a connection between a physical device and a window.
		*\param[in] deviceIndex
		*	The physical device.
		*\param[in] handle
		*	The window handle.
		*/
		SurfacePtr createSurface( PhysicalDevice const & gpu
			, ashes::WindowHandle handle )const;
#if VK_EXT_debug_utils
		/**
		*\brief
		*	Creates a debug messenger.
		*\param[in] createInfo
		*	The creation informations.
		*/
		VkDebugUtilsMessengerEXT createDebugUtilsMessenger( VkDebugUtilsMessengerCreateInfoEXT & createInfo )const;
		/**
		*\brief
		*	Destroys a debug messenger.
		*\param[in] messenger
		*	The messenger.
		*/
		void destroyDebugUtilsMessenger( VkDebugUtilsMessengerEXT messenger )const;
		/**
		*\brief
		*	Submits a debug messenger.
		*\param[in] messenger
		*	The messenger.
		*/
		void submitDebugMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageTypes
			, VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData )const;
#endif
#if VK_EXT_debug_report
		/**
		*\brief
		*	Creates a debug report callback.
		*\param[in] createInfo
		*	The creation informations.
		*/
		VkDebugReportCallbackEXT createDebugReportCallback( VkDebugReportCallbackCreateInfoEXT & createInfo )const;
#endif
		/**
		*\brief
		*	Computes an frustum projection matrix.
		*\param[in] left, right
		*	The left and right planes position.
		*\param[in] top, bottom
		*	The top and bottom planes position.
		*\param[in] zNear, zFar
		*	The near and far planes position.
		*/
		std::array< float, 16 > frustum( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const;
		/**
		*\brief
		*	Computes a perspective projection matrix.
		*\param[in] radiansFovY
		*	The vertical aperture angle.
		*\param[in] aspect
		*	The width / height ratio.
		*\param[in] zNear
		*	The near plane position.
		*\param[in] zFar
		*	The far plane position.
		*\return
		*	The computed matrix in column major order.
		*/
		std::array< float, 16 > perspective( float radiansFovY
			, float aspect
			, float zNear
			, float zFar )const;
		/**
		*\brief
		*	Computes an orthographic projection matrix.
		*\param[in] left, right
		*	The left and right planes position.
		*\param[in] top, bottom
		*	The top and bottom planes position.
		*\param[in] zNear, zFar
		*	The near and far planes position.
		*\return
		*	The computed matrix in column major order.
		*/
		std::array< float, 16 > ortho( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const;
		/**
		*\brief
		*	Computes a perspective projection matrix with no far plane clipping.
		*\param[in] radiansFovY
		*	The vertical aperture angle.
		*\param[in] aspect
		*	The width / height ratio.
		*\param[in] zNear
		*	The near plane position.
		*\return
		*	The computed matrix in column major order.
		*/
		std::array< float, 16 > infinitePerspective( float radiansFovY
			, float aspect
			, float zNear )const;
		/**
		*\brief
		*	Checks existence of extension name in enabled instance extensions.
		*/
		bool checkExtension( std::string const & name )const;
		/**
		*\brief
		*	VkInstance implicit cast operator.
		*/
		inline operator VkInstance const & ()const
		{
			return this->m_instance;
		}

#define VK_LIB_GLOBAL_FUNCTION( ver, fun ) PFN_vk##fun vk##fun{ nullptr };
#define VK_LIB_INSTANCE_FUNCTION( ver, fun ) PFN_vk##fun vk##fun{ nullptr };
#	include <ashes/ashes_functions_list.hpp>
		/**
		*\name
		*	Getters.
		*/
		/**@{*/
		inline PluginFeatures const & getFeatures()const
		{
			return m_features;
		}

		inline std::string getName()const
		{
			return m_plugin.name;
		}

		inline std::string getDescription()const
		{
			return m_plugin.description;
		}

		inline StringArray const & getEnabledLayerNames()const
		{
			return m_createInfo.enabledLayerNames;
		}

		inline StringArray const & getEnabledExtensionNames()const
		{
			return m_createInfo.enabledExtensionNames;
		}
		/**@}*/

	private:
		void doInitInstance();
		PFN_vkVoidFunction getInstanceProcAddr( char const * const name );

	private:
		AshPluginDescription m_plugin;
		ashes::InstanceCreateInfo m_createInfo;
		VkInstance m_instance{ VK_NULL_HANDLE };
		ashes::PluginFeatures m_features;
	};
}

#endif
