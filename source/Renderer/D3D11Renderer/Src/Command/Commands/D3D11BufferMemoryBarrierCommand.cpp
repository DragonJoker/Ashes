/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11BufferMemoryBarrierCommand.hpp"

namespace d3d11_renderer
{
	BufferMemoryBarrierCommand::BufferMemoryBarrierCommand( Device const & device
		, renderer::PipelineStageFlags after
		, renderer::PipelineStageFlags before
		, renderer::BufferMemoryBarrier const & transitionBarrier )
		: CommandBase{ device }
	{
	}

	void BufferMemoryBarrierCommand::apply( Context const & context )const
	{
	}

	CommandPtr BufferMemoryBarrierCommand::clone()const
	{
		return std::make_unique< BufferMemoryBarrierCommand >( *this );
	}
}
