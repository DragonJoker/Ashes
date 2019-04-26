/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

#include "renderer/D3D11Renderer/Shader/D3D11ShaderDesc.hpp"

namespace ashes::d3d11
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
		void apply( Context const & context )const;
		void remove( Context const & context )const;
		CommandPtr clone()const;

	private:
		PushConstantsBuffer m_pcb;
	};
}
