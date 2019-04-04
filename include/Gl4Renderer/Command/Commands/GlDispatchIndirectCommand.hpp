/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	class DispatchIndirectCommand
		: public CommandBase
	{
	public:
		DispatchIndirectCommand( VkDevice device
			, VkBuffer buffer
			, VkDeviceSize offset );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkBuffer m_buffer;
		VkDeviceSize m_offset;
	};
}
