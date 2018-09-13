/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

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
			, renderer::PipelineLayout const & layout
			, renderer::PushConstantsBufferBase const & pcb );
		void apply( Context const & context )const override;
		void remove( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		PushConstantsBuffer const & m_pcb;
		renderer::ByteArray m_data;
	};
}
