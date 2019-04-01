/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_Instance_HPP___
#define ___Ashes_Instance_HPP___
#pragma once

#include "AshesRenderer/Core/PhysicalDevice.hpp"
#include "AshesRenderer/Core/WindowHandle.hpp"
#include "AshesRenderer/Miscellaneous/DebugReportCallbackCreateInfo.hpp"
#include "AshesRenderer/Miscellaneous/DeviceCreateInfo.hpp"
#include "AshesRenderer/Miscellaneous/InstanceCreateInfo.hpp"
#include "AshesRenderer/Miscellaneous/RendererFeatures.hpp"
#include "AshesRenderer/Miscellaneous/LayerProperties.hpp"

#include <array>

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
	protected:
		/**
		*\~french
		*\brief
		*	Constructor, initialises the rendering instance.
		*\~french
		*\brief
		*	Constructeur, initialise l'instance de rendu.
		*/
		Instance( ClipDirection clipDirection
			, std::string const & name
			, InstanceCreateInfo createInfo );

	public:
		/**
		*\~french
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~Instance();
		/**
		*\~french
		*\brief
		*	Enumère les GPU physiques disponibles.
		*\~english
		*\brief
		*	Lists the available physical GPUs.
		*/
		virtual PhysicalDevicePtrArray enumeratePhysicalDevices()const = 0;
		/**
		*\~french
		*\brief
		*	Creates a logical device.
		*\param[in] physicalDevice
		*	The physical device.
		*\param[in] createInfos
		*	The creation informations.
		*\~french
		*\brief
		*	Crée un périphérique logique.
		*\param[in] physicalDevice
		*	Le périphérique physique.
		*\param[in] createInfos
		*	Les informations de création.
		*/
		virtual DevicePtr createDevice( ashes::PhysicalDevice const & physicalDevice
			, DeviceCreateInfo createInfos )const = 0;
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
		virtual SurfacePtr createSurface( PhysicalDevice const & gpu
			, WindowHandle handle )const = 0;
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
		virtual DebugReportCallbackPtr createDebugReportCallback( DebugReportCallbackCreateInfo createInfo )const = 0;
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
		virtual std::array< float, 16 > frustum( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const = 0;
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
		virtual std::array< float, 16 > perspective( float radiansFovY
			, float aspect
			, float zNear
			, float zFar )const = 0;
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
		virtual std::array< float, 16 > ortho( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const = 0;
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
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		inline ClipDirection getClipDirection()const
		{
			return m_clipDirection;
		}

		inline uint32_t getApiVersion()const
		{
			return m_createInfo.applicationInfo.apiVersion;
		}

		inline std::string const & getName()const
		{
			return m_name;
		}

		inline RendererFeatures const & getFeatures()const
		{
			return m_features;
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

	protected:
		InstanceCreateInfo m_createInfo;
		RendererFeatures m_features;

	private:
		ClipDirection m_clipDirection;
		std::string m_name;
	};
}

#endif
