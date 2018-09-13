/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11ImageMemoryBarrierCommand.hpp"

namespace d3d11_renderer
{
	ImageMemoryBarrierCommand::ImageMemoryBarrierCommand( Device const & device
		, ashes::PipelineStageFlags after
		, ashes::PipelineStageFlags before
		, ashes::ImageMemoryBarrier const & transitionBarrier )
		: CommandBase{ device }
	{
	}

	void ImageMemoryBarrierCommand::apply( Context const & context )const
	{
	}

	CommandPtr ImageMemoryBarrierCommand::clone()const
	{
		return std::make_unique< ImageMemoryBarrierCommand >( *this );
	}
}
