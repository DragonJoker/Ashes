/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	/**
	*\brief
	*	Commande de fin d'une passe de rendu, unbind son framebuffer.
	*/
	class ResetQueryPoolCommand
		: public CommandBase
	{
	public:
		ResetQueryPoolCommand( VkDevice device
			, VkQueryPool pool
			, uint32_t firstQuery
			, uint32_t queryCount );
		void apply( Context const & context )const;
		CommandPtr clone()const;
	};
}
