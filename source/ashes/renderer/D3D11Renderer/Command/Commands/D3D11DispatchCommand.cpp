/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11DispatchCommand.hpp"

namespace ashes::d3d11
{
	DispatchCommand::DispatchCommand( VkDevice device
		, uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ )
		: CommandBase{ device }
		, m_groupCountX{ groupCountX }
		, m_groupCountY{ groupCountY }
		, m_groupCountZ{ groupCountZ }
	{
	}

	void DispatchCommand::apply( Context const & context )const
	{
		context.context->Dispatch( m_groupCountX, m_groupCountY, m_groupCountZ );
	}

	CommandPtr DispatchCommand::clone()const
	{
		return std::make_unique< DispatchCommand >( *this );
	}
}
