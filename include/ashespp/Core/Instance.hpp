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

#include <ashes/common/DeviceAllocator.hpp>

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
		*	Constructor, initialises the rendering instance, Ashes plugin mode.
		*/
		Instance( AshPluginDescription const & plugin
			, DeviceAllocatorPtr allocator
			, ashes::InstanceCreateInfo createInfo );
		/**
		*\brief
		*	Constructor, initialises the rendering instance, Vulkan mode.
		*/
		Instance( PFN_vkGetInstanceProcAddr getInstanceProcAddr
			, DeviceAllocatorPtr allocator
			, ashes::InstanceCreateInfo createInfo );
		/**
		*\brief
		*	Destructor.
		*/
		~Instance()noexcept;
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
		VkDebugUtilsMessengerEXT createDebugUtilsMessenger( VkDebugUtilsMessengerCreateInfoEXT const & createInfo )const;
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
		VkDebugReportCallbackEXT createDebugReportCallback( VkDebugReportCallbackCreateInfoEXT const & createInfo )const;
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
		operator VkInstance const & ()const noexcept
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
		VkAllocationCallbacks const * getAllocationCallbacks()const;

		PluginFeatures const & getFeatures()const noexcept
		{
			return m_features;
		}

		StringArray const & getEnabledLayerNames()const noexcept
		{
			return m_createInfo.enabledLayerNames;
		}

		StringArray const & getEnabledExtensionNames()const noexcept
		{
			return m_createInfo.enabledExtensionNames;
		}
		/**@}*/

	private:
		friend class PhysicalDevice;

		void doInitInstance();
		PFN_vkVoidFunction getInstanceProcAddr( char const * const name )const;

	private:
		PFN_vkGetInstanceProcAddr m_getInstanceProcAddr;
		DeviceAllocatorPtr m_allocator;
		ashes::InstanceCreateInfo m_createInfo;
		VkInstance m_instance{};
		ashes::PluginFeatures m_features;
	};
}

#endif
