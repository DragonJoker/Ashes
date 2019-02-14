/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

#include <Ashes/Pipeline/Scissor.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Commande d'application d'un scissor.
	*/
	class SetLineWidthCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] scissor
		*	Le scissor.
		*/
		SetLineWidthCommand( Device const & device
			, float width );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		float m_width;
	};
}
