/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

#include <Ashes/Pipeline/Scissor.hpp>
#include <Ashes/RenderPass/ClearValue.hpp>

namespace ashes::d3d11
{
	class BeginRenderPassCommand
		: public CommandBase
	{
	public:
		BeginRenderPassCommand( VkDevice device
			, ashes::RenderPass const & renderPass
			, ashes::FrameBuffer const & frameBuffer
			, ashes::ClearValueArray const & clearValues );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		RenderPass const & m_renderPass;
		FrameBuffer const & m_frameBuffer;
		ashes::ClearValueArray m_rtClearValues;
		ashes::ClearValue m_dsClearValue;
		RECT m_scissor;
	};
}