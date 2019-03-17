/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_Surface_HPP___
#define ___Ashes_Surface_HPP___
#pragma once

#include "Ashes/Core/WindowHandle.hpp"
#include "Ashes/Miscellaneous/SurfaceCapabilities.hpp"
#include "Ashes/Miscellaneous/SurfaceFormat.hpp"

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
	protected:
		Surface( Surface const & ) = delete;
		Surface & operator=( Surface const & ) = delete;
		Surface( Surface && rhs ) = default;
		Surface & operator=( Surface && rhs ) = default;
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

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~Surface() = default;
		/**
		*\~english
		*\brief
		*	Checks whether a queue family of the physical device supports presentation to the surface.
		*\~french
		*\brief
		*	Vérifie si une famille de file du périphérique physique supporte la présentation à la surface.
		*/
		virtual bool getSupport( uint32_t queueFamilyIndex )const = 0;
		/**
		*\~english
		*\return
		*	The surface capabilities.
		*\~french
		*\brief
		*	Les capacités de la surface.
		*/
		virtual SurfaceCapabilities getCapabilities()const = 0;
		/**
		*\~english
		*\return
		*	The surface present modes.
		*\~french
		*\brief
		*	Les modes de présentation de la surface.
		*/
		virtual std::vector < PresentMode > getPresentModes()const = 0;
		/**
		*\~english
		*\return
		*	The surface supported formats.
		*\~french
		*\brief
		*	Les formats supportés par la surface.
		*/
		virtual std::vector< SurfaceFormat > getFormats()const = 0;
		/**
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		WindowHandle const & getHandle()const
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

	protected:
		WindowHandle m_handle;
		PhysicalDevice const & m_gpu;
		std::string m_type;
	};
}

#endif
