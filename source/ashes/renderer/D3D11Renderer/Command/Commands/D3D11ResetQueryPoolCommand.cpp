/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ResetQueryPoolCommand.hpp"

namespace ashes::D3D11_NAMESPACE
{
	ResetQueryPoolCommand::ResetQueryPoolCommand( VkDevice device )
		: CommandBase{ device }
	{
	}

	void ResetQueryPoolCommand::apply( Context const & context )const
	{
	}

	CommandPtr ResetQueryPoolCommand::clone()const
	{
		return std::make_unique< ResetQueryPoolCommand >( *this );
	}
}
