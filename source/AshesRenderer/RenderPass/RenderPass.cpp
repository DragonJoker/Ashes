/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/RenderPass/RenderPass.hpp"

#include "AshesRenderer/RenderPass/FrameBuffer.hpp"
#include "AshesRenderer/RenderPass/RenderPassCreateInfo.hpp"
#include "AshesRenderer/Core/Device.hpp"

namespace ashes
{
	RenderPass::RenderPass( Device const & device
		, RenderPassCreateInfo const & createInfo )
		: m_device{ device }
		, m_attachments{ createInfo.attachments }
		, m_subpasses{ createInfo.subpasses }
	{
		registerObject( m_device, "RenderPass", this );
	}

	RenderPass::~RenderPass()
	{
		unregisterObject( m_device, this );
	}
}
