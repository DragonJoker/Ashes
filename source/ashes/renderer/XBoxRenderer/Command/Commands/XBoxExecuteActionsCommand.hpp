/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
{
	/**
	*\brief
	*	Classe de base d'une commande.
	*/
	class ExecuteActionsCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		ExecuteActionsCommand( VkDevice device
			, ActionArray actions );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		ActionArray m_actions;
	};
}
