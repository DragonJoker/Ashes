/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

namespace gl_renderer
{
	/**
	*\brief
	*	Commande de copie du contenu d'un tampon dans une image.
	*/
	class CopyImageCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	L'image destination.
		*/
		CopyImageCommand( renderer::BufferBase const & src
			, renderer::Texture const & dst );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		Buffer const & m_src;
		Texture const & m_dst;
		GLenum m_format;
		GLenum m_type;
		GLenum m_target;
	};
}
