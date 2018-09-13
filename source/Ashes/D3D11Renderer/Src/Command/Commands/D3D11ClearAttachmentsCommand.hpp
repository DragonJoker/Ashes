/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

#include <RenderPass/ClearAttachment.hpp>
#include <RenderPass/ClearRect.hpp>

namespace d3d11_renderer
{
	class ClearAttachmentsCommand
		: public CommandBase
	{
	public:
		ClearAttachmentsCommand( Device const & device
			, ashes::ClearAttachmentArray const & clearAttaches
			, ashes::ClearRectArray const & clearRects );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		ashes::ClearAttachmentArray m_clearAttaches;
		ashes::ClearRectArray m_clearRects;
	};
}
