/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#pragma once

#include "Vec2.hpp"

#include <VkLib/Viewport.hpp>

namespace renderer
{
	/**
	*\brief
	*	Description d'un viewport à utiliser lors de la création d'un pipeline ou à l'exécution.
	*/
	class Viewport
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] width, height
		*	Les dimensions du viewport.
		*\param[in] x, y
		*	La position du viewport.
		*/
		Viewport( uint32_t width
			, uint32_t height
			, int32_t x
			, int32_t y );
		/**
		*\return
		*	Le viewport Vulkan.
		*/
		inline vk::Viewport const & getViewport()const
		{
			return m_viewport;
		}

	private:
		vk::Viewport m_viewport;
	};
}
