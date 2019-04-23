/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ScissorCommand.hpp"

#include "Core/D3D11Device.hpp"

namespace ashes::d3d11
{
	ScissorCommand::ScissorCommand( Device const & device
		, uint32_t first
		, ashes::VkScissorArray const & scissors )
		: CommandBase{ device }
		, m_scissors{ makeScissors( scissors.begin() + first, scissors.end() ) }
	{
	}

	void ScissorCommand::apply( Context const & context )const
	{
		context.context->RSSetScissorRects( UINT( m_scissors.size() )
			, m_scissors.data() );
	}

	CommandPtr ScissorCommand::clone()const
	{
		return std::make_unique< ScissorCommand >( *this );
	}
}
