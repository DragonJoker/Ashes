/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_FrameBufferAttachment_HPP___
#define ___Ashes_FrameBufferAttachment_HPP___
#pragma once

#include "Ashes/Image/ImageView.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	An attach between a texture and a framebuffer.
	*\~french
	*\brief
	*	Une attache entre une image et un tampon d'image.
	*/
	class FrameBufferAttachment
	{
		friend class FrameBuffer;

	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] attach
		*	The associated render pass attach.
		*\param[in] view
		*	The texture view.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] attach
		*	L'attache de passe de rendu correspondante.
		*\param[in] view
		*	La vue sur la texture.
		*/
		FrameBufferAttachment( AttachmentDescription const & attach
			, ImageViewPtr view );
		/**
		*\~english
		*\return
		*	The pixel format of the attachment.
		*\~french
		*\return
		*	Le format des pixels de l'attache.
		*/
		inline ashes::Format getFormat()const
		{
			return m_view->getFormat();
		}
		/**
		*\~english
		*\return
		*	The texture view.
		*\~french
		*\return
		*	La vue sur la texture.
		*/
		inline ImageView const & getView()const
		{
			return *m_view;
		}
		/**
		*\~english
		*\return
		*	The texture.
		*\~french
		*\return
		*	La texture.
		*/
		inline Image const & getImage()const
		{
			return m_view->getImage();
		}
		/**
		*\~english
		*\return
		*	The framebuffer.
		*\~french
		*\return
		*	Le tampon d'image.
		*/
		inline FrameBuffer const & getFrameBuffer()const
		{
			return *m_frameBuffer;
		}
		/**
		*\~english
		*\return
		*	The associated render pass attach.
		*\~french
		*\return
		*	L'attache de passe de rendu correspondante.
		*/
		inline AttachmentDescription const & getAttachment()const
		{
			return m_attach;
		}

	private:
		ImageViewPtr m_view;
		FrameBuffer const * m_frameBuffer;
		AttachmentDescription const & m_attach;
	};
}

#endif
