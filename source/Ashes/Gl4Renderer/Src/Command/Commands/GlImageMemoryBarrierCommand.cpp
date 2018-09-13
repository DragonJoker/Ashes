/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlImageMemoryBarrierCommand.hpp"

namespace gl_renderer
{
	ImageMemoryBarrierCommand::ImageMemoryBarrierCommand( Device const & device
		, ashes::PipelineStageFlags after
		, ashes::PipelineStageFlags before
		, ashes::ImageMemoryBarrier const & transitionBarrier )
		: CommandBase{ device }
		, m_flags{ convert( before ) }
	{
	}

	void ImageMemoryBarrierCommand::apply( ContextLock const & context )const
	{
		//glLogCommand( "ImageMemoryBarrierCommand" );
		//glLogCall( context
		//	, glMemoryBarrier
		//	, m_flags );
	}

	CommandPtr ImageMemoryBarrierCommand::clone()const
	{
		return std::make_unique< ImageMemoryBarrierCommand >( *this );
	}
}
