/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11WriteTimestampCommand.hpp"

#include "Miscellaneous/D3D11QueryPool.hpp"

namespace ashes::d3d11
{
	WriteTimestampCommand::WriteTimestampCommand( VkDevice device
		, VkPipelineStageFlagBits pipelineStage
		, ashes::QueryPool const & pool
		, uint32_t query )
		: CommandBase{ device }
		, m_query{ *( static_cast< QueryPool const & >( pool ).begin() + query ) }
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
