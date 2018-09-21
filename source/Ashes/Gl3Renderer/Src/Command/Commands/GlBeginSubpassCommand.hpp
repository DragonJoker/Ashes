/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

namespace gl_renderer
{
	/**
	*\brief
	*	Démarre une passe de rendu en bindant son framebuffer, et en le vidant au besoin.
	*/
	class BeginSubpassCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] renderPass
		*	La passe de rendu.
		*\param[in] frameBuffer
		*	Le tampon d'image affecté par le rendu.
		*\param[in] clearValues
		*	Les valeurs de vidage, une par attache de la passe de rendu.
		*\param[in] contents
		*	Indique la manière dont les commandes de la première sous-passe sont fournies.
		*/
		BeginSubpassCommand( Device const & device
			, ashes::RenderPass const & renderPass
			, ashes::FrameBuffer const & frameBuffer
			, ashes::SubpassDescription const & subpass );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		RenderPass const & m_renderPass;
		ashes::SubpassDescription const & m_subpass;
		FrameBuffer const & m_frameBuffer;
	};
}
