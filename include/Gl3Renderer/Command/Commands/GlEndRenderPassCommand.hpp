/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace gl_renderer
{
	/**
	*\brief
	*	Commande de fin d'une passe de rendu, unbind son framebuffer.
	*/
	class EndRenderPassCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		EndRenderPassCommand( Device const & device );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;
	};
}
