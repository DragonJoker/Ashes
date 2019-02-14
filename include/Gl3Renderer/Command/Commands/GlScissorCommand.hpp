/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Command/Commands/GlCommandBase.hpp"

#include <Ashes/Pipeline/Scissor.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Commande d'application d'un scissor.
	*/
	class ScissorCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] scissor
		*	Le scissor.
		*/
		ScissorCommand( Device const & device
			, uint32_t firstScissor
			, ashes::ScissorArray const & scissors );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		ashes::ScissorArray m_scissors;
	};
}
