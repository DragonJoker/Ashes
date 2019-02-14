/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

#include "D3D11Renderer/Shader/D3D11ShaderDesc.hpp"

namespace d3d11_renderer
{
	/**
	*\brief
	*	Commande de d�marrage d'une requ�te.
	*/
	class PushConstantsCommand
		: public CommandBase
	{
	public:
		PushConstantsCommand( Device const & device
			, PushConstantsBuffer const & pcb );
		void apply( Context const & context )const override;
		void remove( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		PushConstantsBuffer m_pcb;
	};
}
