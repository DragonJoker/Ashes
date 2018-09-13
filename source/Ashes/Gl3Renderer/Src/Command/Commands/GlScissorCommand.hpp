/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

#include <Pipeline/Scissor.hpp>

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
			, ashes::Scissor const & scissor );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		ashes::Scissor m_scissor;
	};
}
