/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	class BeginRenderPassCommand
		: public CommandBase
	{
	public:
		BeginRenderPassCommand( VkDevice device
			, VkRenderPass renderPass
			, VkFramebuffer frameBuffer
			, VkClearValueArray clearValues
			, VkSubpassContents contents );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkRenderPass m_renderPass;
		VkFramebuffer m_frameBuffer;
		VkClearValueArray m_rtClearValues;
		VkClearValue m_dsClearValue;
		VkRect2D m_scissor;
	};
}
