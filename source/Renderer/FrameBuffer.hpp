/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/FrameBuffer.hpp>

namespace renderer
{
	/**
	*\brief
	*	Classe encapsulant le concept de Framebuffer.
	*\remarks
	*	Contient les tampon de profondeur et de couleur.
	*/
	class FrameBuffer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le device logique.
		*\param[in] renderPass
		*	La passe de rendu avec laquelle ce tampon d'images sera compatible.
		*\param[in] views
		*	Les attaches.
		*\param[in] width, height
		*	Les dimensions du tampon d'images.
		*/
		FrameBuffer();
		/**
		*\return
		*	Le Framebuffer vulkan.
		*/
		inline vk::FrameBuffer const & getFrameBuffer()const
		{
			assert( m_framebuffer );
			return *m_framebuffer;
		}

	private:
		vk::FrameBufferPtr m_framebuffer;
	};
}
