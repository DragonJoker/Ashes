/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class PushConstantsCommand
		: public CommandBase
	{
	public:
		PushConstantsCommand( VkDevice device
			, PushConstantsBuffer const & pcb );
		PushConstantsCommand( VkDevice device
			, PushConstantsDesc const & pcb );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		PushConstantsBuffer m_pcb;
	};
}
