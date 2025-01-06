/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxViewportCommand.hpp"

#include "Core/XBoxDevice.hpp"

namespace ashes::xbox
{
	ViewportCommand::ViewportCommand( VkDevice device
		, uint32_t first
		, ArrayView< VkViewport const > const & viewports )
		: CommandBase{ device }
		, m_viewports{ makeViewports( viewports.begin() + first, viewports.end() ) }
	{
	}

	void ViewportCommand::apply( Context const & context )const
	{
		context.context->RSSetViewports( UINT( m_viewports.size() )
			, m_viewports.data() );
	}

	CommandPtr ViewportCommand::clone()const
	{
		return std::make_unique< ViewportCommand >( *this );
	}
}
