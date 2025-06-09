/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::D3D11_NAMESPACE
{
	class WaitEventsCommand
		: public CommandBase
	{
	public:
		WaitEventsCommand( VkDevice device
			, ArrayView< VkEvent const > const & events );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkEventArray m_events;
	};
}
