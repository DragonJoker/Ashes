/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ExecuteActionsCommand.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::D3D11_NAMESPACE
{
	ExecuteActionsCommand::ExecuteActionsCommand( VkDevice device
		, ActionArray actions )
		: CommandBase{ device }
		, m_actions{ std::move( actions ) }
	{
	}

	void ExecuteActionsCommand::apply( Context const & context )const
	{
		for ( auto const & action : m_actions )
		{
			action( context );
		}
	}

	CommandPtr ExecuteActionsCommand::clone()const
	{
		return std::make_unique< ExecuteActionsCommand >( *this );
	}
}
