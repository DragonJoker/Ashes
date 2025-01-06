/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
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
