/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

#include <Miscellaneous/BufferImageCopy.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Commande de copie du contenu d'une image dans un tampon.
	*/
	class CopyImageToBufferCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] copyInfo
		*	Les informations de copie.
		*\param[in] src
		*	L'image source.
		*\param[in] dst
		*	Le tampon destination.
		*/
		CopyImageToBufferCommand( renderer::BufferImageCopy const & copyInfo
			, renderer::Texture const & src
			, renderer::BufferBase const & dst );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		renderer::BufferImageCopy m_copyInfo;
		Texture const & m_src;
		Buffer const & m_dst;
		GLenum m_format;
		GLenum m_type;
		GLenum m_target;
	};
}
