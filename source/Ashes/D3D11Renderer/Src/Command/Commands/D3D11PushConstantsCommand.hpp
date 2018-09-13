/*
This file belongs to Ashes.
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
			, ashes::PipelineLayout const & layout
			, ashes::PushConstantsBufferBase const & pcb );
		void apply( Context const & context )const override;
		void remove( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		PushConstantsBuffer const & m_pcb;
		ashes::ByteArray m_data;
	};
}
