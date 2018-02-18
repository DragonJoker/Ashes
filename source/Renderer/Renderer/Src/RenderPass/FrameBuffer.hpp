/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_FrameBuffer_HPP___
#define ___Renderer_FrameBuffer_HPP___
#pragma once

#include "FrameBufferAttachment.hpp"

namespace renderer
{
	/**
	*\brief
	*	Classe encapsulant le concept de Framebuffer.
	*\remarks
	*	Contient les tampons de profondeur et de couleur.
	*/
	class FrameBuffer
	{
	protected:
		/**
		*\brief
		*	Crée un FrameBuffer.
		*/
		FrameBuffer();
		/**
		*\brief
		*	Crée un FrameBuffer compatible avec la passe de rendu donnée.
		*\remarks
		*	Si la compatibilité entre les textures voulues et les formats de la passe de rendu
		*	n'est pas possible, une std::runtime_error est lancée.
		*\param[in] dimensions
		*	Les dimensions du tampon d'images.
		*\param[in] textures
		*	Les textures voulues pour le tampon d'images à créer.
		*/
		FrameBuffer( RenderPass const & renderPass
			, UIVec2 const & dimensions
			, FrameBufferAttachmentArray && textures );

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~FrameBuffer() = default;
		/**
		*\~english
		*\return
		*	The beginning of the attaches array.
		*\~french
		*\return
		*	Le début du tableau des attaches.
		*/
		inline auto begin()const
		{
			return m_attachments.begin();
		}
		/**
		*\~english
		*\return
		*	The end of the attaches array.
		*\~french
		*\return
		*	La fin du tableau des attaches.
		*/
		inline auto end()const
		{
			return m_attachments.end();
		}
		/**
		*\~english
		*\return
		*	The number of attaches.
		*\~french
		*\return
		*	Le nombre d'attaches.
		*/
		inline size_t getSize()const
		{
			return m_attachments.size();
		}
		/**
		*\~english
		*\return
		*	The framebuffer dimensions.
		*\~french
		*\return
		*	Les dimensions du tampon d'image.
		*/
		inline renderer::UIVec2 const & getDimensions()const
		{
			return m_dimensions;
		}

	protected:
		UIVec2 m_dimensions;
		FrameBufferAttachmentArray m_attachments;
	};
}

#endif
