/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
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
		ScissorCommand( VkDevice device
			, uint32_t firstScissor
			, VkScissorArray scissors );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkScissorArray m_scissors;
	};
}
