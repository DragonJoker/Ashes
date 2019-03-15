/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11SetDepthBiasCommand.hpp"

namespace d3d11_renderer
{
	SetDepthBiasCommand::SetDepthBiasCommand( Device const & device
		, float constantFactor
		, float clamp
		, float slopeFactor )
		: CommandBase{ device }
		, m_constantFactor{ constantFactor }
		, m_clamp{ clamp }
		, m_slopeFactor{ slopeFactor }
	{
	}

	void SetDepthBiasCommand::apply( Context const & context )const
	{
	}

	CommandPtr SetDepthBiasCommand::clone()const
	{
		return std::make_unique< SetDepthBiasCommand >( *this );
	}
}
