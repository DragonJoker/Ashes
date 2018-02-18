/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_RenderSubpassAttachment_HPP___
#define ___Renderer_RenderSubpassAttachment_HPP___
#pragma once

#include "RenderPassAttachment.hpp"

namespace renderer
{
	/**
	*\~english
	*\brief
	*	An attach to a render subpass.
	*\~french
	*\brief
	*	Une attache à une sous-passe de rendu.
	*/
	class RenderSubpassAttachment
	{
	public:
		inline RenderSubpassAttachment( RenderPassAttachment const & attach
			, ImageLayout layout )
			: m_attach{ attach }
			, m_layout{ layout }
		{
		}
		/**
		*\~english
		*\return
		*	The attach index.
		*\~french
		*\return
		*	L'indice de l'attache.
		*/
		inline uint32_t getIndex()const
		{
			return m_attach.getIndex();
		}
		/**
		*\~english
		*\return
		*	The attach pixel format.
		*\~french
		*\return
		*	Le format des pixels de l'attache.
		*/
		inline PixelFormat getFormat()const
		{
			return m_attach.getFormat();
		}
		/**
		*\~english
		*\return
		*	The attach image layout.
		*\~french
		*\return
		*	Le layout de l'image.
		*/
		inline ImageLayout getLayout()const
		{
			return m_layout;
		}

	private:
		RenderPassAttachment m_attach;
		ImageLayout m_layout;
	};
}

#endif
