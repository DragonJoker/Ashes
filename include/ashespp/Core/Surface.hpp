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
	*\brief
	*	Presentation surface.
	*/
	class Surface
	{
	private:
		Surface( Surface const & ) = delete;
		Surface & operator=( Surface const & ) = delete;
		Surface( Surface && rhs ) = default;
		Surface & operator=( Surface && rhs ) = delete;

	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] renderer
		*	The Instance instance.
		*\param[in] gpu
		*	The physical device.
		*\param[in] handle
		*	The window handle.
		*/
		Surface( Instance const & instance
			, PhysicalDevice const & gpu
			, WindowHandle handle );
		/**
		*\brief
		*	Destructor.
		*/
		~Surface()noexcept;
		/**
		*\brief
		*	Checks whether a queue family of the physical device supports presentation to the surface.
		*/
		bool getSupport( uint32_t queueFamilyIndex )const;
		/**
		*\return
		*	The surface capabilities.
		*/
		VkSurfaceCapabilitiesKHR getCapabilities()const;
		/**
		*\return
		*	The surface present modes.
		*/
		std::vector < VkPresentModeKHR > getPresentModes()const;
		/**
		*\return
		*	The surface supported formats.
		*/
		std::vector< VkSurfaceFormatKHR > getFormats()const;
		/**
		*\name
		*	Getters.
		*/
		/**@{*/
		WindowHandle const & getHandle()const noexcept
		{
			return m_handle;
		}

		PhysicalDevice const & getGpu()const noexcept
		{
			return m_gpu;
		}

		std::string const & getType()const noexcept
		{
			return m_type;
		}
		/**@}*/
		/**
		*\brief
		*	VkPhysicalDevice implicit cast operator.
		*/
		operator VkSurfaceKHR const & ()const noexcept
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
	};
}

#endif
