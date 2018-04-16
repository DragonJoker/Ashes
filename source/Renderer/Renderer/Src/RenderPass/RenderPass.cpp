/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "RenderPass/RenderPass.hpp"

#include "RenderPassCreateInfo.hpp"
#include "Core/Device.hpp"

namespace renderer
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
