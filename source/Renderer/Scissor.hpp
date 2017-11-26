/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/Scissor.hpp>

namespace renderer
{
	/**
	*\brief
	*	Description d'un ciseau à utiliser lors de la création d'un pipeline ou à l'exécution.
	*/
	class Scissor
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] x, y
		*	La position du ciseau.
		*\param[in] width, height
		*	Les dimensions du ciseau.
		*/
		Scissor( int32_t x
			, int32_t y
			, uint32_t width
			, uint32_t height );
		/**
		*\return
		*	Le ciseau vulkan.
		*/
		inline vk::Scissor const & getScissor()const
		{
			return m_scissor;
		}

	private:
		vk::Scissor m_scissor;
	};
}
