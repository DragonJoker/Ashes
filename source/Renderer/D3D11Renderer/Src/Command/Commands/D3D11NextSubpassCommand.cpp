/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11NextSubpassCommand.hpp"

#include "RenderPass/D3D11FrameBuffer.hpp"
#include "RenderPass/D3D11RenderPass.hpp"

#include <RenderPass/ClearValue.hpp>
#include <RenderPass/AttachmentDescription.hpp>

namespace d3d11_renderer
{
	NextSubpassCommand::NextSubpassCommand( Device const & device
		, renderer::RenderPass const & renderPass
		, renderer::FrameBuffer const & frameBuffer
		, renderer::SubpassDescription const & subpass )
		: CommandBase{ device }
		, m_renderPass{ static_cast< RenderPass const & >( renderPass ) }
		, m_subpass{ subpass }
		, m_frameBuffer{ static_cast< FrameBuffer const & >( frameBuffer ) }
	{
	}

	void NextSubpassCommand::apply( Context const & context )const
	{
	}

	CommandPtr NextSubpassCommand::clone()const
	{
		return std::make_unique< NextSubpassCommand >( *this );
	}
}
