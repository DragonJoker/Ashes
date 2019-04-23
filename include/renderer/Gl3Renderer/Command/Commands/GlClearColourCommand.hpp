/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Command/Commands/GlCommandBase.hpp"

#include <Ashes/RenderPass/ClearValue.hpp>

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
			, ashes::ImageView const & image
			, VkClearColorValue const & colour );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		Image const & m_image;
		VkClearColorValue m_colour;
		GlInternal m_internal;
		GlFormat m_format;
		GlType m_type;
	};
}
