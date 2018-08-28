/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlDispatchCommand.hpp"

namespace gl_renderer
{
	DispatchCommand::DispatchCommand( Device const & device
		, uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ )
		: CommandBase{ device }
		, m_groupCountX{ groupCountX }
		, m_groupCountY{ groupCountY }
		, m_groupCountZ{ groupCountZ }
	{
	}

	void DispatchCommand::apply()const
	{
		glLogCommand( "DispatchCommand" );
		glLogCall( m_device.getContext()
			, glDispatchCompute
			, m_groupCountX
			, m_groupCountY
			, m_groupCountZ );
	}

	CommandPtr DispatchCommand::clone()const
	{
		return std::make_unique< DispatchCommand >( *this );
	}
}
