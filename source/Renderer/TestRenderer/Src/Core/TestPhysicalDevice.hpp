/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Core/PhysicalDevice.hpp>

namespace test_renderer
{
	/**
	*\~french
	*\brief
	*	Classe contenant les informations liées au GPU physique.
	*\~english
	*\brief
	*	Wraps informations about physical GPU.
	*/
	class PhysicalDevice
		: public renderer::PhysicalDevice
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] vulkan
		*	L'instance Vulkan.
		*\param[in] gpu
		*	Le GPU physique.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] vulkan
		*	The Vulkan instance.
		*\param[in] gpu
		*	The Vulkan physical GPU.
		*/
		PhysicalDevice( Renderer & renderer );
		/**
		*\copydoc	renderer::PhysicalDevice::initialise
		*/
		void initialise()override;

	private:
		Renderer & m_renderer;
	};
}
