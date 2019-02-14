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
	class ResetQueryPoolCommand
		: public CommandBase
	{
	public:
		ResetQueryPoolCommand( Device const & device
			, ashes::QueryPool const & pool
			, uint32_t firstQuery
			, uint32_t queryCount );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;
	};
}
