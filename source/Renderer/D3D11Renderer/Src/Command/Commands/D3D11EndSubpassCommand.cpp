/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11EndSubpassCommand.hpp"

#include "Command/D3D11CommandBuffer.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"

#include <RenderPass/ClearValue.hpp>
#include <RenderPass/AttachmentDescription.hpp>
#include <RenderPass/SubpassDescription.hpp>

namespace d3d11_renderer
{
	EndSubpassCommand::EndSubpassCommand( Device const & device
		, renderer::FrameBuffer const & frameBuffer
		, renderer::SubpassDescription const & subpass )
		: CommandBase{ device }
		, m_frameBuffer{ static_cast< FrameBuffer const & >( frameBuffer ) }
		, m_subpass{ subpass }
	{
		assert( m_subpass.resolveAttachments.empty()
			|| m_subpass.resolveAttachments.size() == m_subpass.colorAttachments.size() );
	}

	void EndSubpassCommand::apply( Context const & context )const
	{
	}

	CommandPtr EndSubpassCommand::clone()const
	{
		return std::make_unique< EndSubpassCommand >( *this );
	}
}
