/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BeginQueryCommand.hpp"

#include "Miscellaneous/D3D11QueryPool.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	BeginQueryCommand::BeginQueryCommand( VkDevice device
		, VkQueryPool pool
		, uint32_t query
		, VkQueryControlFlags flags )
		: CommandBase{ device }
		, m_query{ *( get( pool )->begin() + query ) }
	{
	}

	void BeginQueryCommand::apply( Context const & context )const
	{
		context.context->Begin( m_query );
	}

	CommandPtr BeginQueryCommand::clone()const
	{
		return std::make_unique< BeginQueryCommand >( *this );
	}
}
