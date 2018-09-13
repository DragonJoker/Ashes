/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

namespace d3d11_renderer
{
	class SetEventCommand
		: public CommandBase
	{
	public:
		SetEventCommand( Device const & device
			, ashes::Event const & event
			, ashes::PipelineStageFlags stageFlags );
		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		ashes::Event const & m_event;
	};
}
