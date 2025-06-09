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
	*	Commande de fin d'une passe de rendu, unbind son framebuffer.
	*/
	class ResetQueryPoolCommand
		: public CommandBase
	{
	public:
		explicit ResetQueryPoolCommand( VkDevice device );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;
	};
}
