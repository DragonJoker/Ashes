/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlEndQueryCommand.hpp"

#include "Miscellaneous/GlQueryPool.hpp"

namespace gl_renderer
{
	EndQueryCommand::EndQueryCommand( Device const & device
		, renderer::QueryPool const & pool
		, uint32_t query )
		: CommandBase{ device }
		, m_target{ convert( pool.getType() ) }
	{
	}

	void EndQueryCommand::apply()const
	{
		glLogCommand( "EndQueryCommand" );
		glLogCall( m_device.getContext()
			, glEndQuery
			, m_target );
	}

	CommandPtr EndQueryCommand::clone()const
	{
		return std::make_unique< EndQueryCommand >( *this );
	}
}
