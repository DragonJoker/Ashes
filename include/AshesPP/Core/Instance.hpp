/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Instance_HPP___
#define ___AshesPP_Instance_HPP___
#pragma once

#include "AshesPP/Core/Renderer.hpp"
#include "AshesPP/Core/PhysicalDevice.hpp"
#include "AshesPP/Miscellaneous/RendererFeatures.hpp"

#include <AshesCommon/WindowHandle.hpp>

namespace ashes
{
	/**
	*\~english
	*\brief
	*	The class initialising the rendering instance.
	*\~french
	*\brief
	*	Classe initialisant l'instance de rendu.
	*/
	class Instance
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructor, initialises the rendering instance.
		*\~french
		*\brief
		*	Constructeur, initialise l'instance de rendu.
		*/
		Instance( Renderer const & renderer
			, VkInstanceCreateInfo createInfo );
		/**
		*\~french
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~Instance();
		/**
		*\~french
		*\brief
		*	Enumère les GPU physiques disponibles.
		*\~english
		*\brief
		*	Lists the available physical GPUs.
		*/
		PhysicalDevicePtrArray enumeratePhysicalDevices()const;
		/**
		*\~french
		*\brief
		*	Creates a logical device.
		*\param[in] surface
		*	The presentation surface.
		*\param[in] createInfos
		*	The creation informations.
		*\~french
		*\brief
		*	Crée un périphérique logique.
		*\param[in] surface
		*	La surface de présentation.
		*\param[in] createInfos
		*	Les informations de création.
		*/
		DevicePtr createDevice( SurfacePtr surface
			, VkDeviceCreateInfo createInfos )const;
		/**
		*\~french
		*\brief
		*	Crée une connexion entre un périphérique physique et une fenêtre.
		*\param[in] deviceIndex
		*	Le périphérique physique.
		*\param[in] handle
		*	Le descripteur de la fenêtre.
		*\~french
		*\brief
		*	Creates a connection between a physical device and a window.
		*\param[in] deviceIndex
		*	The physical device.
		*\param[in] handle
		*	The window handle.
		*/
		SurfacePtr createSurface( PhysicalDevice const & gpu
			, ashes::WindowHandle handle )const;
		/**
		*\~french
		*\brief
		*	Crée un callback de rapport de debug.
		*\param[in] createInfo
		*	Les informations de création.
		*\~french
		*\brief
		*	Creates a debug report callback.
		*\param[in] createInfo
		*	The creation informations.
		*/
		VkDebugReportCallbackEXT createDebugReportCallback( VkDebugReportCallbackCreateInfoEXT & createInfo )const;
		/**
		*\~english
		*\brief
		*	Computes an frustum projection matrix.
		*\param[in] left, right
		*	The left and right planes position.
		*\param[in] top, bottom
		*	The top and bottom planes position.
		*\param[in] zNear, zFar
		*	The near and far planes position.
		*\~french
		*\brief
		*	Calcule une matrice de projection frustum.
		*\param[in] left, right
		*	La position des plans gauche et droite.
		*\param[in] top, bottom
		*	La position des plans haut et bas.
		*\param[in] zNear, zFar
		*	La position des premier et arrière plans.
		*\return
		*	La matrice calculée, column major.
		*/
		std::array< float, 16 > frustum( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const;
		/**
		*\~english
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
		*\~french
		*\brief
		*	Calcule une matrice de projection en perspective.
		*\param[in] radiansFovY
		*	L'angle d'ouverture verticale.
		*\param[in] aspect
		*	Le ratio largeur / hauteur.
		*\param[in] zNear
		*	La position du premier plan (pour le clipping).
		*\param[in] zFar
		*	La position de l'arrière plan (pour le clipping).
		*\return
		*	La matrice calculée, column major.
		*/
		std::array< float, 16 > perspective( float radiansFovY
			, float aspect
			, float zNear
			, float zFar )const;
		/**
		*\~english
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
		*\~french
		*\brief
		*	Calcule une matrice de projection orthographique.
		*\param[in] left, right
		*	La position des plans gauche et droite.
		*\param[in] top, bottom
		*	La position des plans haut et bas.
		*\param[in] zNear, zFar
		*	La position des premier et arrière plans.
		*\return
		*	La matrice calculée, column major.
		*/
		std::array< float, 16 > ortho( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const;
		/**
		*\~english
		*	Computes a perspective projection matrix with no far plane clipping.
		*\param[in] radiansFovY
		*	The vertical aperture angle.
		*\param[in] aspect
		*	The width / height ratio.
		*\param[in] zNear
		*	The near plane position.
		*\return
		*	The computed matrix in column major order.
		*\~french
		*\brief
		*	Calcule une matrice de projection en perspective sans clipping
		*	d'arrière plan.
		*\param[in] radiansFovY
		*	L'angle d'ouverture verticale.
		*\param[in] aspect
		*	Le ratio largeur / hauteur.
		*\param[in] zNear
		*	La position du premier plan (pour le clipping).
		*\return
		*	La matrice calculée, column major.
		*/
		std::array< float, 16 > infinitePerspective( float radiansFovY
			, float aspect
			, float zNear )const;
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

#define VK_LIB_GLOBAL_FUNCTION( fun ) PFN_vk##fun vk##fun;
#define VK_LIB_INSTANCE_FUNCTION( fun ) PFN_vk##fun vk##fun;
#	include <AshesCommon/VulkanFunctionsList.inl>
		/**
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		inline uint32_t getApiVersion()const
		{
			return m_createInfo.pApplicationInfo->apiVersion;
		}

		inline RendererFeatures const & getFeatures()const
		{
			return m_features;
		}

		inline StringArray getEnabledLayerNames()const
		{
			return StringArray{ m_createInfo.ppEnabledLayerNames
				, m_createInfo.ppEnabledLayerNames + m_createInfo.enabledLayerCount };
		}

		inline StringArray getEnabledExtensionNames()const
		{
			return StringArray{ m_createInfo.ppEnabledExtensionNames
				, m_createInfo.ppEnabledExtensionNames + m_createInfo.enabledExtensionCount };
		}
		/**@}*/

	private:
		void doInitInstance();
		PFN_vkVoidFunction getInstanceProcAddr( char const * const name );

	private:
		Renderer const & m_renderer;
		VkInstanceCreateInfo m_createInfo;
		VkInstance m_instance{ VK_NULL_HANDLE };
		RendererFeatures m_features;
	};
}

#endif
