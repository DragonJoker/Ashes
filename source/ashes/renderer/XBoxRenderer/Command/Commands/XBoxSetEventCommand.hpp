/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
{
	class SetEventCommand
		: public CommandBase
	{
	public:
		SetEventCommand( VkDevice device
			, VkEvent event );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkEvent m_event;
	};
}
