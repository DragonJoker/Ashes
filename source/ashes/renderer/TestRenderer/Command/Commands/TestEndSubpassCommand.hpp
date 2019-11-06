/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class EndSubpassCommand
		: public CommandBase
	{
	public:
		EndSubpassCommand( VkDevice device
			, VkFramebuffer frameBuffer
			, VkSubpassDescription const & subpass );

		void apply()const;
		CommandPtr clone()const;

	private:
		VkFramebuffer m_frameBuffer;
		VkSubpassDescription const & m_subpass;
	};
}
