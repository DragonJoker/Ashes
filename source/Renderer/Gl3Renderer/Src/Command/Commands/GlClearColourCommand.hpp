/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

#include <RenderPass/ClearValue.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Commande de vidage d'une image.
	*/
	class ClearColourCommand
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
		ClearColourCommand( Device const & device
			, renderer::TextureView const & image
			, renderer::ClearColorValue const & colour );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		Texture const & m_image;
		renderer::ClearColorValue m_colour;
		GlInternal m_internal;
		GlFormat m_format;
		GlType m_type;
	};
}
