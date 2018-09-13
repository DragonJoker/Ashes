/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11SetLineWidthCommand.hpp"

namespace d3d11_renderer
{
	SetLineWidthCommand::SetLineWidthCommand( Device const & device
		, float width )
		: CommandBase{ device }
		, m_width{ width }
	{
	}

	void SetLineWidthCommand::apply( Context const & context )const
	{
	}

	CommandPtr SetLineWidthCommand::clone()const
	{
		return std::make_unique< SetLineWidthCommand >( *this );
	}
}
