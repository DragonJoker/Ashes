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
	class ResetEventCommand
		: public CommandBase
	{
	public:
		ResetEventCommand( Device const & device
			, renderer::Event const & event
			, renderer::PipelineStageFlags stageFlags );
		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		renderer::Event const & m_event;
	};
}
