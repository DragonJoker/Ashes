/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class SetLineWidthCommand
		: public CommandBase
	{
	public:
		SetLineWidthCommand( VkDevice device
			, float width );

		void apply()const;
		CommandPtr clone()const;

	private:
		float m_width;
	};
}
