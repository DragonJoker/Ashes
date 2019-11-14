/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class CommandBase
	{
	public:
		CommandBase( VkDevice device );
		virtual ~CommandBase()noexcept;

		virtual CommandPtr clone()const = 0;
		virtual void apply()const = 0;
		virtual void remove()const
		{
		}

	protected:
		VkDevice m_device;
	};
}
