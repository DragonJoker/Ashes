/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlImageMemoryBarrierCommand.hpp"

namespace gl_renderer
{
	ImageMemoryBarrierCommand::ImageMemoryBarrierCommand( Device const & device
		, renderer::PipelineStageFlags after
		, renderer::PipelineStageFlags before
		, renderer::ImageMemoryBarrier const & transitionBarrier )
		: CommandBase{ device }
		, m_flags{ convert( before ) }
	{
	}

	void ImageMemoryBarrierCommand::apply()const
	{
		//glLogCommand( "ImageMemoryBarrierCommand" );
		//glLogCall( m_device.getContext(), glMemoryBarrier, m_flags );
	}

	CommandPtr ImageMemoryBarrierCommand::clone()const
	{
		return std::make_unique< ImageMemoryBarrierCommand >( *this );
	}
}
