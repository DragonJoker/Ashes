/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

#include <Utils/Vec4.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Commande de vidage d'une image.
	*/
	class ClearCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] image
		*	L'image à vider.
		*\param[in] colour
		*	La couleur de vidage.
		*/
		ClearCommand( renderer::Texture const & image
			, renderer::RgbaColour const & colour );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		Texture const & m_image;
		renderer::RgbaColour m_colour;
		GLenum m_format;
		GLenum m_type;
	};
}
