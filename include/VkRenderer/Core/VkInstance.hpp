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
		Instance( Configuration const & configuration );
		/**
		*\brief
		*	Destructeur.
		*/
		~Instance();
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
		*	Ajoute les couches de l'instance aux noms déjà présents dans la liste donnée.
		*\param[in,out] names
		*	La liste à compléter.
		*\~english
		*\brief
		*	Adds the instance layers names to the given names.
		*\param[in,out] names
		*	The liste to fill.
		*/
		void completeLayerNames( ashes::StringArray & names )const;
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkInstance.
		*\~english
		*\brief
		*	VkInstance implicit cast operator.
		*/
		inline operator VkInstance const &( )const
		{
			return m_instance;
		}

		PFN_vkGetInstanceProcAddr GetInstanceProcAddr;
#define VK_LIB_GLOBAL_FUNCTION( fun ) PFN_##fun fun;
#define VK_LIB_INSTANCE_FUNCTION( fun ) PFN_##fun fun;
#	include "Miscellaneous/VulkanFunctionsList.inl"

	private:
		/**
		*\~french
		*\brief
		*	Enumère les couches disponibles sur le système.
		*\~english
		*\brief
		*	Lists all layers available on the system.
		*/
		void doInitLayersProperties();
		/**
		*\~french
		*\brief
		*	Enumère les extensions disponibles pour la couche donnée.
		*\param[out] layerProps
		*	Reçoit les propriétés des extensions de la couche.
		*\~english
		*\brief
		*	Lists all the extensions supported by given layer.
		*\param[out] layerProps
		*	Receives the extensions properties for the layer.
		*/
		void doInitLayerExtensionProperties( ashes::LayerProperties & layerProps );
		/**
		*\~french
		*\brief
		*	Crée l'instance Vulkan.
		*\~english
		*\brief
		*	Creates the Vulkan instance.
		*/
		void doInitInstance();
		/**
		*\~french
		*\brief
		*	Enumère les GPU physiques disponibles.
		*\~english
		*\brief
		*	Lists the available physical GPUs.
		*/
		void doEnumerateDevices();
		PFN_vkVoidFunction getInstanceProcAddr( char const * const name );

	private:
		ashes::DynamicLibrary m_library;
		VkInstance m_instance{ VK_NULL_HANDLE };
		VkDebugReportCallbackEXT m_msgCallback{ VK_NULL_HANDLE };
	};
}
