/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

#include <Pipeline/Scissor.hpp>
#include <RenderPass/ClearValue.hpp>

namespace gl_renderer
{
	class BeginRenderPassCommand
		: public CommandBase
	{
	public:
		BeginRenderPassCommand( Device const & device
			, ashes::RenderPass const & renderPass
			, ashes::FrameBuffer const & frameBuffer
			, ashes::ClearValueArray const & clearValues
			, ashes::SubpassContents contents );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		RenderPass const & m_renderPass;
		FrameBuffer const & m_frameBuffer;
		ashes::ClearValueArray m_rtClearValues;
		ashes::ClearValue m_dsClearValue;
		ashes::Scissor m_scissor;
	};
}
