/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11WriteTimestampCommand.hpp"

#include "Miscellaneous/D3D11QueryPool.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	WriteTimestampCommand::WriteTimestampCommand( VkDevice device
		, VkPipelineStageFlagBits pipelineStage
		, VkQueryPool pool
		, uint32_t query )
		: CommandBase{ device }
		, m_query{ *( get( pool )->begin() + query ) }
	{
	}

	void WriteTimestampCommand::apply( Context const & context )const
	{
		context.context->End( m_query );
	}

	CommandPtr WriteTimestampCommand::clone()const
	{
		return std::make_unique< WriteTimestampCommand >( *this );
	}
}
