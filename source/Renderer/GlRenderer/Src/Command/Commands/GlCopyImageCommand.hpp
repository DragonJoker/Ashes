/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

#include <Miscellaneous/ImageCopy.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Commande de copie du contenu d'une image dans une autre.
	*/
	class CopyImageCommand
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
		*	L'image destination.
		*/
		CopyImageCommand( renderer::ImageCopy const & copyInfo
			, renderer::Texture const & src
			, renderer::Texture const & dst );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		Texture const & m_src;
		Texture const & m_dst;
		renderer::ImageCopy m_copyInfo;
		GLenum m_srcFormat;
		GLenum m_srcType;
		GLenum m_srcTarget;
		GLenum m_dstFormat;
		GLenum m_dstType;
		GLenum m_dstTarget;
	};
}
