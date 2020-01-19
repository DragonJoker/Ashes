/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlPushConstantsCommand.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void apply( ContextLock const & context
		, CmdProgramUniform1fv const & cmd )
	{
		glLogCall( context
			, glProgramUniform1fv
			, cmd.program
			, cmd.location
			, 1u
			, &cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdProgramUniform2fv const & cmd )
	{
		glLogCall( context
			, glProgramUniform2fv
			, cmd.program
			, cmd.location
			, 1u
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdProgramUniform3fv const & cmd )
	{
		glLogCall( context
			, glProgramUniform3fv
			, cmd.program
			, cmd.location
			, 1u
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdProgramUniform4fv const & cmd )
	{
		glLogCall( context
			, glProgramUniform4fv
			, cmd.program
			, cmd.location
			, 1u
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdProgramUniformMatrix2fv const & cmd )
	{
		glLogCall( context
			, glProgramUniformMatrix2fv
			, cmd.program
			, cmd.location
			, 1u
			, cmd.transpose
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdProgramUniformMatrix3fv const & cmd )
	{
		glLogCall( context
			, glProgramUniformMatrix3fv
			, cmd.program
			, cmd.location
			, 1u
			, cmd.transpose
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdProgramUniformMatrix4fv const & cmd )
	{
		glLogCall( context
			, glProgramUniformMatrix4fv
			, cmd.program
			, cmd.location
			, 1u
			, cmd.transpose
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform1iv const & cmd )
	{
		glLogCall( context
			, glProgramUniform1iv
			, cmd.program
			, cmd.location
			, 1u
			, &cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform2iv const & cmd )
	{
		glLogCall( context
			, glProgramUniform2iv
			, cmd.program
			, cmd.location
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform3iv const & cmd )
	{
		glLogCall( context
			, glProgramUniform3iv
			, cmd.program
			, cmd.location
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform4iv const & cmd )
	{
		glLogCall( context
			, glProgramUniform4iv
			, cmd.program
			, cmd.location
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform1uiv const & cmd )
	{
		glLogCall( context
			, glProgramUniform1uiv
			, cmd.program
			, cmd.location
			, 1u
			, &cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform2uiv const & cmd )
	{
		glLogCall( context
			, glProgramUniform2uiv
			, cmd.program
			, cmd.location
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform3uiv const & cmd )
	{
		glLogCall( context
			, glProgramUniform3uiv
			, cmd.program
			, cmd.location
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform4uiv const & cmd )
	{
		glLogCall( context
			, glProgramUniform4uiv
			, cmd.program
			, cmd.location
			, 1u
			, cmd.buffer );
	}

	template< OpType OpT, typename T >
	void buildPushProgramUniformMtxCommand( ConstantDesc const & constant
		, uint8_t const *& buffer
		, CmdList & list )
	{
		auto arraySize = std::min( 1u, constant.arraySize );

		for ( auto layer = 0u; layer < arraySize; ++layer )
		{
			list.push_back( makeCmd< OpT >( constant.program
				, constant.location + layer
				, GL_FALSE
				, reinterpret_cast< T const * >( buffer ) ) );
			buffer += constant.size;
		}
	}

	template< OpType OpT, typename T >
	void buildPushProgramUniformCommand( ConstantDesc const & constant
		, uint8_t const *& buffer
		, CmdList & list )
	{
		auto arraySize = std::max( 1u, constant.arraySize );

		for ( auto layer = 0u; layer < arraySize; ++layer )
		{
			list.push_back( makeCmd< OpT >( constant.program
				, constant.location + layer
				, reinterpret_cast< T const * >( buffer ) ) );
			buffer += constant.size;
		}
	}

	void apply( ContextLock const & context
		, CmdUniform1fv const & cmd )
	{
		glLogCall( context
			, glUniform1fv
			, cmd.location
			, 1u
			, &cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdUniform2fv const & cmd )
	{
		glLogCall( context
			, glUniform2fv
			, cmd.location
			, 1u
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdUniform3fv const & cmd )
	{
		glLogCall( context
			, glUniform3fv
			, cmd.location
			, 1u
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdUniform4fv const & cmd )
	{
		glLogCall( context
			, glUniform4fv
			, cmd.location
			, 1u
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdUniformMatrix2fv const & cmd )
	{
		glLogCall( context
			, glUniformMatrix2fv
			, cmd.location
			, 1u
			, cmd.transpose
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdUniformMatrix3fv const & cmd )
	{
		glLogCall( context
			, glUniformMatrix3fv
			, cmd.location
			, 1u
			, cmd.transpose
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdUniformMatrix4fv const & cmd )
	{
		glLogCall( context
			, glUniformMatrix4fv
			, cmd.location
			, 1u
			, cmd.transpose
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform1iv const & cmd )
	{
		glLogCall( context
			, glUniform1iv
			, cmd.location
			, 1u
			, &cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform2iv const & cmd )
	{
		glLogCall( context
			, glUniform2iv
			, cmd.location
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform3iv const & cmd )
	{
		glLogCall( context
			, glUniform3iv
			, cmd.location
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform4iv const & cmd )
	{
		glLogCall( context
			, glUniform4iv
			, cmd.location
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform1uiv const & cmd )
	{
		glLogCall( context
			, glUniform1uiv
			, cmd.location
			, 1u
			, &cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform2uiv const & cmd )
	{
		glLogCall( context
			, glUniform2uiv
			, cmd.location
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform3uiv const & cmd )
	{
		glLogCall( context
			, glUniform3uiv
			, cmd.location
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform4uiv const & cmd )
	{
		glLogCall( context
			, glUniform4uiv
			, cmd.location
			, 1u
			, cmd.buffer );
	}

	template< OpType OpT, typename T >
	void buildPushUniformMtxCommand( ConstantDesc const & constant
		, uint8_t const *& buffer
		, CmdList & list )
	{
		auto arraySize = std::min( 1u, constant.arraySize );

		for ( auto layer = 0u; layer < arraySize; ++layer )
		{
			list.push_back( makeCmd< OpT >( constant.location + layer
				, GL_FALSE
				, reinterpret_cast< T const * >( buffer ) ) );
			buffer += constant.size;
		}
	}

	template< OpType OpT, typename T >
	void buildPushUniformCommand( ConstantDesc const & constant
		, uint8_t const *& buffer
		, CmdList & list )
	{
		auto arraySize = std::max( 1u, constant.arraySize );

		for ( auto layer = 0u; layer < arraySize; ++layer )
		{
			list.push_back( makeCmd< OpT >( constant.location + layer
				, reinterpret_cast< T const * >( buffer ) ) );
			buffer += constant.size;
		}
	}

	void buildPushConstantsCommand( VkDevice device
		, VkShaderStageFlags stageFlags
		, PushConstantsDesc const & pcb
		, CmdList & list )
	{
		glLogCommand( list, "PushConstantsCommand" );

		if ( isGl4( device ) )
		{
			for ( auto & constant : pcb.constants )
			{
				if ( ( constant.stageFlag & stageFlags ) != 0 )
				{
					int32_t offset = int32_t( constant.offset ) - int32_t( pcb.offset );
					assert( offset >= 0 );
					auto buffer = pcb.data.data() + offset;

					switch ( constant.format )
					{
					case ConstantFormat::eFloat:
						buildPushProgramUniformCommand< OpType::eProgramUniform1fv, GLfloat >( constant, buffer, list );
						break;

					case ConstantFormat::eVec2f:
						buildPushProgramUniformCommand< OpType::eProgramUniform2fv, GLfloat >( constant, buffer, list );
						break;

					case ConstantFormat::eVec3f:
						buildPushProgramUniformCommand< OpType::eProgramUniform3fv, GLfloat >( constant, buffer, list );
						break;

					case ConstantFormat::eVec4f:
						buildPushProgramUniformCommand< OpType::eProgramUniform4fv, GLfloat >( constant, buffer, list );
						break;

					case ConstantFormat::eMat2f:
						buildPushProgramUniformMtxCommand< OpType::eProgramUniformMatrix2fv, GLfloat >( constant, buffer, list );
						break;

					case ConstantFormat::eMat3f:
						buildPushProgramUniformMtxCommand< OpType::eProgramUniformMatrix3fv, GLfloat >( constant, buffer, list );
						break;

					case ConstantFormat::eMat4f:
						buildPushProgramUniformMtxCommand< OpType::eProgramUniformMatrix4fv, GLfloat >( constant, buffer, list );
						break;

					case ConstantFormat::eInt:
						buildPushProgramUniformCommand< OpType::eProgramUniform1iv, GLint >( constant, buffer, list );
						break;

					case ConstantFormat::eVec2i:
						buildPushProgramUniformCommand< OpType::eProgramUniform2iv, GLint >( constant, buffer, list );
						break;

					case ConstantFormat::eVec3i:
						buildPushProgramUniformCommand< OpType::eProgramUniform3iv, GLint >( constant, buffer, list );
						break;

					case ConstantFormat::eVec4i:
						buildPushProgramUniformCommand< OpType::eProgramUniform4iv, GLint >( constant, buffer, list );
						break;

					case ConstantFormat::eUInt:
						buildPushProgramUniformCommand< OpType::eProgramUniform1uiv, GLuint >( constant, buffer, list );
						break;

					case ConstantFormat::eVec2ui:
						buildPushProgramUniformCommand< OpType::eProgramUniform2uiv, GLuint >( constant, buffer, list );
						break;

					case ConstantFormat::eVec3ui:
						buildPushProgramUniformCommand< OpType::eProgramUniform3uiv, GLuint >( constant, buffer, list );
						break;

					case ConstantFormat::eVec4ui:
						buildPushProgramUniformCommand< OpType::eProgramUniform4uiv, GLuint >( constant, buffer, list );
						break;

					case ConstantFormat::eColour:
						buildPushProgramUniformCommand< OpType::eProgramUniform4fv, GLfloat >( constant, buffer, list );
						break;

					default:
						assert( false && "Unsupported constant format" );
						break;
					}
				}
			}
		}
		else
		{
			for ( auto & constant : pcb.constants )
			{
				if ( ( constant.stageFlag & stageFlags ) != 0 )
				{
					int32_t offset = int32_t( constant.offset ) - int32_t( pcb.offset );
					assert( offset >= 0 );
					auto buffer = pcb.data.data() + offset;

					switch ( constant.format )
					{
					case ConstantFormat::eFloat:
						buildPushUniformCommand< OpType::eUniform1fv, GLfloat >( constant, buffer, list );
						break;

					case ConstantFormat::eVec2f:
						buildPushUniformCommand< OpType::eUniform2fv, GLfloat >( constant, buffer, list );
						break;

					case ConstantFormat::eVec3f:
						buildPushUniformCommand< OpType::eUniform3fv, GLfloat >( constant, buffer, list );
						break;

					case ConstantFormat::eVec4f:
						buildPushUniformCommand< OpType::eUniform4fv, GLfloat >( constant, buffer, list );
						break;

					case ConstantFormat::eMat2f:
						buildPushUniformMtxCommand< OpType::eUniformMatrix2fv, GLfloat >( constant, buffer, list );
						break;

					case ConstantFormat::eMat3f:
						buildPushUniformMtxCommand< OpType::eUniformMatrix3fv, GLfloat >( constant, buffer, list );
						break;

					case ConstantFormat::eMat4f:
						buildPushUniformMtxCommand< OpType::eUniformMatrix4fv, GLfloat >( constant, buffer, list );
						break;

					case ConstantFormat::eInt:
						buildPushUniformCommand< OpType::eUniform1iv, GLint >( constant, buffer, list );
						break;

					case ConstantFormat::eVec2i:
						buildPushUniformCommand< OpType::eUniform2iv, GLint >( constant, buffer, list );
						break;

					case ConstantFormat::eVec3i:
						buildPushUniformCommand< OpType::eUniform3iv, GLint >( constant, buffer, list );
						break;

					case ConstantFormat::eVec4i:
						buildPushUniformCommand< OpType::eUniform4iv, GLint >( constant, buffer, list );
						break;

					case ConstantFormat::eUInt:
						buildPushUniformCommand< OpType::eUniform1uiv, GLuint >( constant, buffer, list );
						break;

					case ConstantFormat::eVec2ui:
						buildPushUniformCommand< OpType::eUniform2uiv, GLuint >( constant, buffer, list );
						break;

					case ConstantFormat::eVec3ui:
						buildPushUniformCommand< OpType::eUniform3uiv, GLuint >( constant, buffer, list );
						break;

					case ConstantFormat::eVec4ui:
						buildPushUniformCommand< OpType::eUniform4uiv, GLuint >( constant, buffer, list );
						break;

					case ConstantFormat::eColour:
						buildPushUniformCommand< OpType::eUniform4fv, GLfloat >( constant, buffer, list );
						break;

					default:
						assert( false && "Unsupported constant format" );
						break;
					}
				}
			}
		}
	}
}
