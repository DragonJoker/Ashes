/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Surface_HPP___
#define ___AshesPP_Surface_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

#include "ashespp/Core/WindowHandle.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Presentation surface.
	*\~french
	*\brief
	*	Surface de présentation.
	*/
	class Surface
	{
	private:
		Surface( Surface const & ) = delete;
		Surface & operator=( Surface const & ) = delete;
		Surface( Surface && rhs ) = default;
		Surface & operator=( Surface && rhs ) = default;

	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] renderer
		*	The Instance instance.
		*\param[in] gpu
		*	The physical device.
		*\param[in] handle
		*	The window handle.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] renderer
		*	L'instance.
		*\param[in] gpu
		*	Le périphérique physique.
		*\param[in] handle
		*	Le descripteur de la fenêtre.
		*/
		Surface( Instance const & instance
			, PhysicalDevice const & gpu
			, WindowHandle handle );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~Surface();
		/**
		*\~english
		*\brief
		*	Checks whether a queue family of the physical device supports presentation to the surface.
		*\~french
		*\brief
		*	Vérifie si une famille de file du périphérique physique supporte la présentation à la surface.
		*/
		bool getSupport( uint32_t queueFamilyIndex )const;
		/**
		*\~english
		*\return
		*	The surface capabilities.
		*\~french
		*\brief
		*	Les capacités de la surface.
		*/
		VkSurfaceCapabilitiesKHR getCapabilities()const;
		/**
		*\~english
		*\return
		*	The surface present modes.
		*\~french
		*\brief
		*	Les modes de présentation de la surface.
		*/
		std::vector < VkPresentModeKHR > getPresentModes()const;
		/**
		*\~english
		*\return
		*	The surface supported formats.
		*\~french
		*\brief
		*	Les formats supportés par la surface.
		*/
		std::vector< VkSurfaceFormatKHR > getFormats()const;
		/**
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		inline WindowHandle const & getHandle()const
		{
			return m_handle;
		}

		inline PhysicalDevice const & getGpu()const
		{
			return m_gpu;
		}

		inline std::string const & getType()const
		{
			return m_type;
		}
		/**@}*/
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkPhysicalDevice.
		*\~english
		*\brief
		*	VkPhysicalDevice implicit cast operator.
		*/
		inline operator VkSurfaceKHR const & ( )const
		{
			return m_internal;
		}

	private:
		void doCreate();

	private:
		Instance const & m_instance;
		PhysicalDevice const & m_gpu;
		WindowHandle m_handle;
		VkSurfaceKHR m_internal;
		std::string m_type;
		uint32_t m_imageCount;
	};
}

#endif
