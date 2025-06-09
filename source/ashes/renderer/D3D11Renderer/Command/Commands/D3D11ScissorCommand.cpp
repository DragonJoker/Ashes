/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ScissorCommand.hpp"

#include "Core/D3D11Device.hpp"

namespace ashes::D3D11_NAMESPACE
{
	ScissorCommand::ScissorCommand( VkDevice device
		, uint32_t first
		, ArrayView< VkRect2D const > const & scissors )
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
