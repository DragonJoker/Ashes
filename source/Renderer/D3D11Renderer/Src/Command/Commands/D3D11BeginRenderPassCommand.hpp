/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

#include <Pipeline/Scissor.hpp>
#include <RenderPass/ClearValue.hpp>

namespace d3d11_renderer
{
	class BeginRenderPassCommand
		: public CommandBase
	{
	public:
		BeginRenderPassCommand( Device const & device
			, renderer::RenderPass const & renderPass
			, renderer::FrameBuffer const & frameBuffer
			, renderer::ClearValueArray const & clearValues );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		RenderPass const & m_renderPass;
		FrameBuffer const & m_frameBuffer;
		renderer::ClearValueArray m_rtClearValues;
		renderer::ClearValue m_dsClearValue;
		RECT m_scissor;
	};
}
