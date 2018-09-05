/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlDrawIndexedCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Core/GlRenderer.hpp"

namespace gl_renderer
{
	namespace
	{
		uint32_t getSize( renderer::IndexType type )
		{
			switch ( type )
			{
			case renderer::IndexType::eUInt16:
				return 2u;
			case renderer::IndexType::eUInt32:
				return 4u;
			default:
				assert( false && "Unsupported index type" );
				return 1u;
			}
		}
	}

	DrawIndexedCommand::DrawIndexedCommand( Device const & device
		, uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance
		, renderer::PrimitiveTopology mode
		, renderer::IndexType type )
		: CommandBase{ device }
		, m_indexCount{ indexCount }
		, m_instCount{ instCount }
		, m_firstIndex{ firstIndex }
		, m_vertexOffset{ vertexOffset }
		, m_firstInstance{ firstInstance }
		, m_mode{ convert( mode ) }
		, m_type{ convert( type ) }
		, m_size{ getSize( type ) }
	{
		if ( m_firstInstance > 0
			&& !m_device.getRenderer().getFeatures().hasBaseInstance )
		{
			throw std::runtime_error( "Base instance rendering is not supported" );
		}
	}

	void DrawIndexedCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "DrawIndexedCommand" );

		if ( m_device.getRenderer().getFeatures().hasBaseInstance )
		{
			glLogCall( context
				, glDrawElementsInstancedBaseVertexBaseInstance_ARB
				, m_mode
				, m_indexCount
				, m_type
				, ( ( GLvoid * )( m_firstIndex * m_size ) )
				, m_instCount
				, m_vertexOffset
				, m_firstInstance );
		}
		else
		{
			glLogCall( context
				, glDrawElementsInstancedBaseVertex
				, m_mode
				, m_indexCount
				, m_type
				, ( ( GLvoid * )( m_firstIndex * m_size ) )
				, m_instCount
				, m_vertexOffset );
		}
	}

	CommandPtr DrawIndexedCommand::clone()const
	{
		return std::make_unique< DrawIndexedCommand >( *this );
	}
}
