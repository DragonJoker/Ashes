/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ExecuteActionsCommand.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	ExecuteActionsCommand::ExecuteActionsCommand( VkDevice device
		, ActionArray actions )
		: CommandBase{ device }
		, m_actions{ std::move( actions ) }
	{
	}

	void ExecuteActionsCommand::apply( Context const & context )const
	{
		for ( auto & action : m_actions )
		{
			action( context );
		}
	}

	CommandPtr ExecuteActionsCommand::clone()const
	{
		return std::make_unique< ExecuteActionsCommand >( *this );
	}
}
