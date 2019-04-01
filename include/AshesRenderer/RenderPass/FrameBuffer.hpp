/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_FrameBuffer_HPP___
#define ___Ashes_FrameBuffer_HPP___
#pragma once

#include "AshesRenderer/RenderPass/FrameBufferAttachment.hpp"
#include "AshesRenderer/Miscellaneous/Extent2D.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Class wrapping the concept of a Framebuffer.
	*\~french
	*\brief
	*	Classe encapsulant le concept de Framebuffer.
	*/
	class FrameBuffer
	{
	protected:
		/**
		*\~english
		*\brief
		*	Creates a framebuffer compatible with the given render pass.
		*\param[in] renderPass
		*	The render pass, containing the attachments.
		*\param[in] dimensions
		*	The frame buffer dimensions.
		*\param[in] textures
		*	The attachments wanted for the framebuffer.
		*\~french
		*\brief
		*	Crée un FrameBuffer compatible avec la passe de rendu donnée.
		*\param[in] renderPass
		*	La passe de rendu, contenant les attaches.
		*\param[in] dimensions
		*	Les dimensions du tampon d'images.
		*\param[in] textures
		*	Les textures voulues pour le tampon d'images à créer.
		*/
		FrameBuffer( RenderPass const & renderPass
			, Extent2D const & dimensions
			, FrameBufferAttachmentArray textures );

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~FrameBuffer();
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
		inline Extent2D const & getDimensions()const
		{
			return m_dimensions;
		}

	protected:
		Device const & m_device;
		Extent2D m_dimensions;
		FrameBufferAttachmentArray m_attachments;
	};
}

#endif
