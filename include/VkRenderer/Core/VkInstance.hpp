/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Core/Instance.hpp>
#include <Utils/DynamicLibrary.hpp>

namespace vk_renderer
{
	class Instance
		: public ashes::Instance
	{
	public:
		/**
		*\brief
		*	Constructeur, initialise l'instance de Vulkan.
		*/
		Instance( std::unique_ptr< ashes::DynamicLibrary > library
			, PFN_vkGetInstanceProcAddr getInstanceProcAddr
			, ashes::InstanceCreateInfo createInfo );
		/**
		*\brief
		*	Destructeur.
		*/
		~Instance();
		/**
		*\copydoc	ashes::Instance::enumerateLayerProperties
		*/
		ashes::PhysicalDevicePtrArray enumeratePhysicalDevices()const override;
		/**
		*\copydoc	ashes::Instance::createDevice
		*/
		ashes::DevicePtr createDevice( ashes::SurfacePtr surface
			, ashes::DeviceCreateInfo createInfos )const override;
		/**
		*\copydoc	ashes::Instance::createSurface
		*/
		ashes::SurfacePtr createSurface( ashes::PhysicalDevice const & gpu
			, ashes::WindowHandle handle )const override;
		/**
		*\copydoc	ashes::Instance::createDebugReportCallback
		*/
		ashes::DebugReportCallbackPtr createDebugReportCallback( ashes::DebugReportCallbackCreateInfo createInfo )const override;
		/**
		*\copydoc	ashes::Instance::frustum
		*/
		std::array< float, 16 > frustum( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const override;
		/**
		*\copydoc	ashes::Instance::perspective
		*/
		std::array< float, 16 > perspective( float radiansFovY
			, float aspect
			, float zNear
			, float zFar )const override;
		/**
		*\copydoc	ashes::Instance::ortho
		*/
		std::array< float, 16 > ortho( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const override;
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkInstance.
		*\~english
		*\brief
		*	VkInstance implicit cast operator.
		*/
		inline operator VkInstance const & ()const
		{
			return m_instance;
		}

		PFN_vkGetInstanceProcAddr m_getInstanceProcAddr;
#define VK_LIB_GLOBAL_FUNCTION( fun ) PFN_##fun fun;
#define VK_LIB_INSTANCE_FUNCTION( fun ) PFN_##fun fun;
#	include "Miscellaneous/VulkanFunctionsList.inl"

	private:
		void doInitInstance();
		PFN_vkVoidFunction getInstanceProcAddr( char const * const name );

	private:
		std::unique_ptr< ashes::DynamicLibrary > m_library;
		VkInstance m_instance{ VK_NULL_HANDLE };
	};
}
