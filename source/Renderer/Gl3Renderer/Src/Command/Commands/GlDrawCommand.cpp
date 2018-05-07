/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlDrawCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Core/GlRenderer.hpp"

namespace gl_renderer
{
	DrawCommand::DrawCommand( Device const & device
		, uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance
		, renderer::PrimitiveTopology mode )
		: m_device{ device }
		, m_vtxCount{ vtxCount }
		, m_instCount{ instCount }
		, m_firstVertex{ firstVertex }
		, m_firstInstance{ firstInstance }
		, m_mode{ convert( mode ) }
	{
		if ( m_firstInstance > 0
			&& !m_device.getRenderer().getFeatures().hasBaseInstance )
		{
			throw std::runtime_error( "Base instance rendering is not supported" );
		}
	}

	void DrawCommand::apply()const
	{
		assert( m_instCount >= 1 );
		glLogCommand( "DrawCommand" );

		if ( m_device.getRenderer().getFeatures().hasBaseInstance )
		{
			glLogCall( gl::DrawArraysInstancedBaseInstance_ARB
				, m_mode
				, m_firstVertex
				, m_vtxCount
				, m_instCount
				, m_firstInstance );
		}
		else
		{
			glLogCall( gl::DrawArraysInstanced
				, m_mode
				, m_firstVertex
				, m_vtxCount
				, m_instCount );
		}
	}

	CommandPtr DrawCommand::clone()const
	{
		return std::make_unique< DrawCommand >( *this );
	}
}
