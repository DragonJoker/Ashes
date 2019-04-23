/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlPushConstantsCommand.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{	
	void apply( ContextLock const & context
		, CmdUniform1fv const & cmd )
	{
		glLogCall( context
			, glUniform1fv
			, cmd.location
			, cmd.arraySize
			, &cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdUniform2fv const & cmd )
	{
		glLogCall( context
			, glUniform2fv
			, cmd.location
			, cmd.arraySize
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdUniform3fv const & cmd )
	{
		glLogCall( context
			, glUniform3fv
			, cmd.location
			, cmd.arraySize
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdUniform4fv const & cmd )
	{
		glLogCall( context
			, glUniform4fv
			, cmd.location
			, cmd.arraySize
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdUniformMatrix2fv const & cmd )
	{
		glLogCall( context
			, glUniformMatrix2fv
			, cmd.location
			, cmd.arraySize
			, cmd.transpose
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdUniformMatrix3fv const & cmd )
	{
		glLogCall( context
			, glUniformMatrix3fv
			, cmd.location
			, cmd.arraySize
			, cmd.transpose
			, cmd.buffer );
	}
	
	void apply( ContextLock const & context
		, CmdUniformMatrix4fv const & cmd )
	{
		glLogCall( context
			, glUniformMatrix4fv
			, cmd.location
			, cmd.arraySize
			, cmd.transpose
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform1iv const & cmd )
	{
		glLogCall( context
			, glUniform1iv
			, cmd.location
			, cmd.arraySize
			, &cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform2iv const & cmd )
	{
		glLogCall( context
			, glUniform2iv
			, cmd.location
			, cmd.arraySize
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform3iv const & cmd )
	{
		glLogCall( context
			, glUniform3iv
			, cmd.location
			, cmd.arraySize
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform4iv const & cmd )
	{
		glLogCall( context
			, glUniform4iv
			, cmd.location
			, cmd.arraySize
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform1uiv const & cmd )
	{
		glLogCall( context
			, glUniform1uiv
			, cmd.location
			, cmd.arraySize
			, &cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform2uiv const & cmd )
	{
		glLogCall( context
			, glUniform2uiv
			, cmd.location
			, cmd.arraySize
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform3uiv const & cmd )
	{
		glLogCall( context
			, glUniform3uiv
			, cmd.location
			, cmd.arraySize
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform4uiv const & cmd )
	{
		glLogCall( context
			, glUniform4uiv
			, cmd.location
			, cmd.arraySize
			, cmd.buffer );
	}

	void buildPushConstantsCommand( PushConstantsDesc const & pcb
		, CmdList & list )
	{
		glLogCommand( "PushConstantsCommand" );

		for ( auto & constant : pcb.constants )
		{
			auto buffer = pcb.data.data() + constant.offset;

			switch ( constant.format )
			{
			case ConstantFormat::eFloat:
				list.push_back( makeCmd< OpType::eUniform1fv >( constant.location
					, constant.arraySize
					, reinterpret_cast< GLfloat const * >( buffer ) ) );
				break;

			case ConstantFormat::eVec2f:
				list.push_back( makeCmd< OpType::eUniform2fv >( constant.location
					, constant.arraySize
					, reinterpret_cast< GLfloat const * >( buffer ) ) );
				break;

			case ConstantFormat::eVec3f:
				list.push_back( makeCmd< OpType::eUniform3fv >( constant.location
					, constant.arraySize
					, reinterpret_cast< GLfloat const * >( buffer ) ) );
				break;

			case ConstantFormat::eVec4f:
				list.push_back( makeCmd< OpType::eUniform4fv >( constant.location
					, constant.arraySize
					, reinterpret_cast< GLfloat const * >( buffer ) ) );
				break;

			case ConstantFormat::eMat2f:
				list.push_back( makeCmd< OpType::eUniformMatrix2fv >( constant.location
					, constant.arraySize
					, GL_FALSE
					, reinterpret_cast< GLfloat const * >( buffer ) ) );
				break;

			case ConstantFormat::eMat3f:
				list.push_back( makeCmd< OpType::eUniformMatrix3fv >( constant.location
					, constant.arraySize
					, GL_FALSE
					, reinterpret_cast< GLfloat const * >( buffer ) ) );
				break;

			case ConstantFormat::eMat4f:
				list.push_back( makeCmd< OpType::eUniformMatrix4fv >( constant.location
					, constant.arraySize
					, GL_FALSE
					, reinterpret_cast< GLfloat const * >( buffer ) ) );
				break;

			case ConstantFormat::eInt:
				list.push_back( makeCmd< OpType::eUniform1iv >( constant.location
					, constant.arraySize
					, reinterpret_cast< GLint const * >( buffer ) ) );
				break;

			case ConstantFormat::eVec2i:
				list.push_back( makeCmd< OpType::eUniform2iv >( constant.location
					, constant.arraySize
					, reinterpret_cast< GLint const * >( buffer ) ) );
				break;

			case ConstantFormat::eVec3i:
				list.push_back( makeCmd< OpType::eUniform3iv >( constant.location
					, constant.arraySize
					, reinterpret_cast< GLint const * >( buffer ) ) );
				break;

			case ConstantFormat::eVec4i:
				list.push_back( makeCmd< OpType::eUniform4iv >( constant.location
					, constant.arraySize
					, reinterpret_cast< GLint const * >( buffer ) ) );
				break;

			case ConstantFormat::eUInt:
				list.push_back( makeCmd< OpType::eUniform1uiv >( constant.location
					, constant.arraySize
					, reinterpret_cast< GLuint const * >( buffer ) ) );
				break;

			case ConstantFormat::eVec2ui:
				list.push_back( makeCmd< OpType::eUniform2uiv >( constant.location
					, constant.arraySize
					, reinterpret_cast< GLuint const * >( buffer ) ) );
				break;

			case ConstantFormat::eVec3ui:
				list.push_back( makeCmd< OpType::eUniform3uiv >( constant.location
					, constant.arraySize
					, reinterpret_cast< GLuint const * >( buffer ) ) );
				break;

			case ConstantFormat::eVec4ui:
				list.push_back( makeCmd< OpType::eUniform4uiv >( constant.location
					, constant.arraySize
					, reinterpret_cast< GLuint const * >( buffer ) ) );
				break;

			case ConstantFormat::eColour:
				list.push_back( makeCmd< OpType::eUniform4fv >( constant.location
					, constant.arraySize
					, reinterpret_cast< GLfloat const * >( buffer ) ) );
				break;

			default:
				assert( false && "Unsupported constant format" );
				break;
			}
		}
	}
}
