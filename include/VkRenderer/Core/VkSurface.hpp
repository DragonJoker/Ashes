/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Core/Surface.hpp>

namespace vk_renderer
{
	class Surface
		: public ashes::Surface
	{
	public:
		Surface( Instance const & instance
			, ashes::PhysicalDevice const & gpu
			, ashes::WindowHandle handle );
		/**
		*\brief
		*	Destructeur, détruit la surface KHR.
		*/
		~Surface();
		/**
		*\copydoc	ashes::Surface:getSupport
		*/
		bool getSupport( uint32_t queueFamilyIndex )const override;
		/**
		*\copydoc	ashes::Surface:getSurfaceCapabilities
		*/
		ashes::SurfaceCapabilities getCapabilities()const override;
		/**
		*\copydoc	ashes::Surface:getPresentModes
		*/
		std::vector < ashes::PresentMode > getPresentModes()const override;
		/**
		*\copydoc	ashes::Surface:getSurfaceFormats
		*/
		std::vector< ashes::SurfaceFormat > getFormats()const override;
		/**
		*\~french
		*\return
		*	La surface de présentation.
		*\~english
		*\return
		*	The presentation surface.
		*/
		inline operator VkSurfaceKHR()const
		{
			return m_surface;
		}
		/**
		*\~french
		*\return
		*	Le périphérique physique.
		*\~english
		*\return
		*	The physical device.
		*/
		inline PhysicalDevice const & getGpu()const
		{
			return m_gpu;
		}

	private:
		void doCreate();

	private:
		Instance const & m_instance;
		PhysicalDevice const & m_gpu;
		VkSurfaceKHR m_surface{ VK_NULL_HANDLE };
	};
}
