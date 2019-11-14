/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class DispatchIndirectCommand
		: public CommandBase
	{
	public:
		DispatchIndirectCommand( VkDevice device
			, VkBuffer buffer
			, VkDeviceSize offset );

		void apply()const;
		CommandPtr clone()const;

	private:
		VkBuffer m_buffer;
		uint32_t m_offset;
	};
}
