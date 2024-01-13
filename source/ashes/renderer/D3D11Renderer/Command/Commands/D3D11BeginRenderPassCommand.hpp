/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class BeginRenderPassCommand
		: public CommandBase
	{
	public:
		BeginRenderPassCommand( VkDevice device
			, VkRenderPass renderPass
			, VkFramebuffer frameBuffer
			, VkClearValueArray const & clearValues );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkRenderPass m_renderPass{};
		VkFramebuffer m_frameBuffer{};
		VkClearValueArray m_rtClearValues{};
		VkClearValue m_dsClearValue{};
		RECT m_scissor{};
	};
}
