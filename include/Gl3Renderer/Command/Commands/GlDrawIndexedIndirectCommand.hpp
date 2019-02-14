/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace gl_renderer
{
	class DrawIndexedIndirectCommand
		: public CommandBase
	{
	public:
		DrawIndexedIndirectCommand( Device const & device
			, ashes::BufferBase const & buffer
			, uint32_t offset
			, uint32_t drawCount
			, uint32_t stride
			, ashes::PrimitiveTopology mode
			, ashes::IndexType type );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		Buffer const & m_buffer;
		uint32_t m_offset;
		uint32_t m_drawCount;
		uint32_t m_stride;
		GlPrimitiveTopology m_mode;
		GlIndexType m_type;
	};
}
