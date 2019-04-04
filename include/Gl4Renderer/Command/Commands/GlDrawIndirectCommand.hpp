/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	class DrawIndirectCommand
		: public CommandBase
	{
	public:
		DrawIndirectCommand( VkDevice device
			, VkBuffer buffer
			, VkDeviceSize offset
			, uint32_t drawCount
			, uint32_t stride
			, VkPrimitiveTopology mode );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkBuffer m_buffer;
		VkDeviceSize m_offset;
		uint32_t m_drawCount;
		uint32_t m_stride;
		GlPrimitiveTopology m_mode;
	};
}
