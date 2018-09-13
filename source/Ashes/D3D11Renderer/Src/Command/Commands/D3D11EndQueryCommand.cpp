/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11EndQueryCommand.hpp"

#include "Miscellaneous/D3D11QueryPool.hpp"

namespace d3d11_renderer
{
	EndQueryCommand::EndQueryCommand( Device const & device
		, ashes::QueryPool const & pool
		, uint32_t query )
		: CommandBase{ device }
		, m_query{ *( static_cast< QueryPool const & >( pool ).begin() + query ) }
	{
	}

	void EndQueryCommand::apply( Context const & context )const
	{
		context.context->End( m_query );
	}

	CommandPtr EndQueryCommand::clone()const
	{
		return std::make_unique< EndQueryCommand >( *this );
	}
}
