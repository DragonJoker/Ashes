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
	class ClearDepthStencilFboCommand
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
		ClearDepthStencilFboCommand( Device const & device
			, ashes::ImageView const & image
			, ashes::DepthStencilClearValue const & value );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		ImageView const & m_image;
		ashes::DepthStencilClearValue m_value;
		GlInternal m_internal;
		GlFormat m_format;
		GlType m_type;
	};
}
