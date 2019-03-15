/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_Connection_HPP___
#define ___Ashes_Connection_HPP___
#pragma once

#include "Ashes/Core/WindowHandle.hpp"
#include "Ashes/Miscellaneous/SurfaceCapabilities.hpp"
#include "Ashes/Miscellaneous/SurfaceFormat.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Connection between a physical device and a window.
	*\~french
	*\brief
	*	Connection entre un périphérique physique et une fenêtre.
	*/
	class Connection
	{
	protected:
		Connection( Connection const & ) = delete;
		Connection & operator=( Connection const & ) = delete;
		Connection( Connection && rhs ) = default;
		Connection & operator=( Connection && rhs ) = default;
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
		Connection( Instance const & instance
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
		virtual ~Connection() = default;
		/**
		*\~english
		*\brief
		*	Checks whether a queue family of the physical device supports presentation to the surface.
		*\~french
		*\brief
		*	Vérifie si une famille de file du périphérique physique supporte la présentation à la surface.
		*/
		virtual bool getSurfaceSupport( uint32_t queueFamilyIndex )const = 0;
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

		inline std::string const & getSurfaceType()const
		{
			return m_surfaceType;
		}

		inline std::vector< SurfaceFormat > const & getSurfaceFormats()const
		{
			return m_surfaceFormats;
		}

		inline SurfaceCapabilities const & getSurfaceCapabilities()const
		{
			return m_surfaceCapabilities;
		}

		inline std::vector< PresentMode > const & getPresentModes()const
		{
			return m_presentModes;
		}
		/**@}*/

	protected:
		WindowHandle m_handle;
		PhysicalDevice const & m_gpu;
		std::vector< SurfaceFormat > m_surfaceFormats;
		SurfaceCapabilities m_surfaceCapabilities;
		std::vector< PresentMode > m_presentModes;
		std::string m_surfaceType;
	};
}

#endif
