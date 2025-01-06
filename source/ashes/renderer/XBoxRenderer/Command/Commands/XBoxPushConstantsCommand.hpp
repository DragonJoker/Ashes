/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

#include "renderer/XBoxRenderer/Shader/XBoxShaderDesc.hpp"

namespace ashes::xbox
{
	/**
	*\brief
	*	Commande de d�marrage d'une requ�te.
	*/
	class PushConstantsCommand
		: public CommandBase
	{
	public:
		PushConstantsCommand( VkDevice device
			, PushConstantsBuffer const & pcb );

		void apply( Context const & context )const override;
		void remove( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		PushConstantsBuffer m_pcb;
	};
}
