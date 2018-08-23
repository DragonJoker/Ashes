/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlPushConstantsCommand.hpp"

#include "Buffer/PushConstantsBuffer.hpp"

namespace gl_renderer
{
	PushConstantsCommand::PushConstantsCommand( Device const & device
		, renderer::PipelineLayout const & layout
		, renderer::PushConstantsBufferBase const & pcb )
		: CommandBase{ device }
		, m_pcb{ pcb }
		, m_data{ pcb.getData(), pcb.getData() + pcb.getSize() }
	{
	}

	void PushConstantsCommand::apply()const
	{
		glLogCommand( "PushConstantsCommand" );
		auto buffer = m_data.data();

		for ( auto & constant : m_pcb )
		{
			switch ( constant.format )
			{
			case renderer::ConstantFormat::eFloat:
				glLogCall( m_device.getContext(), glUniform1fv, constant.location, constant.arraySize, reinterpret_cast< GLfloat const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eVec2f:
				glLogCall( m_device.getContext(), glUniform2fv, constant.location, constant.arraySize, reinterpret_cast< GLfloat const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eVec3f:
				glLogCall( m_device.getContext(), glUniform3fv, constant.location, constant.arraySize, reinterpret_cast< GLfloat const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eVec4f:
				glLogCall( m_device.getContext(), glUniform4fv, constant.location, constant.arraySize, reinterpret_cast< GLfloat const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eMat2f:
				glLogCall( m_device.getContext(), glUniformMatrix2fv, constant.location, constant.arraySize, GL_FALSE, reinterpret_cast< GLfloat const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eMat3f:
				glLogCall( m_device.getContext(), glUniformMatrix3fv, constant.location, constant.arraySize, GL_FALSE, reinterpret_cast< GLfloat const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eMat4f:
				glLogCall( m_device.getContext(), glUniformMatrix4fv, constant.location, constant.arraySize, GL_FALSE, reinterpret_cast< GLfloat const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eInt:
				glLogCall( m_device.getContext(), glUniform1iv, constant.location, constant.arraySize, reinterpret_cast< GLint const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eVec2i:
				glLogCall( m_device.getContext(), glUniform2iv, constant.location, constant.arraySize, reinterpret_cast< GLint const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eVec3i:
				glLogCall( m_device.getContext(), glUniform3iv, constant.location, constant.arraySize, reinterpret_cast< GLint const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eVec4i:
				glLogCall( m_device.getContext(), glUniform4iv, constant.location, constant.arraySize, reinterpret_cast< GLint const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eUInt:
				glLogCall( m_device.getContext(), glUniform1uiv, constant.location, constant.arraySize, reinterpret_cast< GLuint const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eVec2ui:
				glLogCall( m_device.getContext(), glUniform2uiv, constant.location, constant.arraySize, reinterpret_cast< GLuint const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eVec3ui:
				glLogCall( m_device.getContext(), glUniform3uiv, constant.location, constant.arraySize, reinterpret_cast< GLuint const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eVec4ui:
				glLogCall( m_device.getContext(), glUniform4uiv, constant.location, constant.arraySize, reinterpret_cast< GLuint const * >( buffer ) );
				break;

			case renderer::ConstantFormat::eColour:
				glLogCall( m_device.getContext(), glUniform4fv, constant.location, constant.arraySize, reinterpret_cast< GLfloat const * >( buffer ) );
				break;

			default:
				assert( false && "Unsupported constant format" );
				break;
			}

			buffer += getSize( constant.format );
		}
	}

	CommandPtr PushConstantsCommand::clone()const
	{
		return std::make_unique< PushConstantsCommand >( *this );
	}
}
