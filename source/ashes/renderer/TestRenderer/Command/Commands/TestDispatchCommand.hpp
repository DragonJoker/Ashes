/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class DispatchCommand
		: public CommandBase
	{
	public:
		DispatchCommand( VkDevice device
			, uint32_t groupCountX
			, uint32_t groupCountY
			, uint32_t groupCountZ );

		void apply()const;
		CommandPtr clone()const;

	private:
		uint32_t m_groupCountX;
		uint32_t m_groupCountY;
		uint32_t m_groupCountZ;
	};
}
