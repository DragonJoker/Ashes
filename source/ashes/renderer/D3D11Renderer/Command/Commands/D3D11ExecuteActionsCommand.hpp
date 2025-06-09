/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::D3D11_NAMESPACE
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
