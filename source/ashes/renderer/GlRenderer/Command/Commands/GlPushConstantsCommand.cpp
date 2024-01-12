/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlPushConstantsCommand.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	namespace
	{
		template< OpType OpT, typename T >
		void buildPushProgramUniformMtxCommand( ConstantDesc const & constant
			, uint8_t const *& buffer
			, CmdList & list )
		{
			auto arraySize = std::max( 1u, constant.arraySize );

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

		template< OpType OpTypeT, typename DataT >
		void buildPushUniformMtxCommand( ConstantDesc const & constant
			, uint8_t const *& buffer
			, CmdList & list )
		{
			auto arraySize = std::max( 1u, constant.arraySize );

			for ( auto layer = 0u; layer < arraySize; ++layer )
			{
				list.push_back( makeCmd< OpTypeT >( constant.location + layer
					, GL_FALSE
					, reinterpret_cast< DataT const * >( buffer ) ) );
				buffer += constant.size;
			}
		}

		template< OpType OpTypeT, typename DataT >
		void buildPushUniformCommand( ConstantDesc const & constant
			, uint8_t const *& buffer
			, CmdList & list )
		{
			auto arraySize = std::max( 1u, constant.arraySize );

			for ( auto layer = 0u; layer < arraySize; ++layer )
			{
				list.push_back( makeCmd< OpTypeT >( constant.location + layer
					, reinterpret_cast< DataT const * >( buffer ) ) );
				buffer += constant.size;
			}
		}

		void buildPushProgramUniformsCommands( VkShaderStageFlags stageFlags
			, ConstantsLayout const & constants
			, ByteArray const & data
			, CmdList & list )
		{
			for ( auto & constant : constants )
			{
				if ( ( constant.stageFlag & stageFlags ) != 0
					&& constant.location != ~0u )
				{
					auto offset = int32_t( constant.offset );
					assert( offset >= 0 && offset + constant.size <= data.size() );
					auto buffer = data.data() + offset;

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

		void buildPushUniformsCommands( VkShaderStageFlags stageFlags
			, ConstantsLayout const & constants
			, ByteArray const & data
			, CmdList & list )
		{
			for ( auto & constant : constants )
			{
				if ( ( constant.stageFlag & stageFlags ) != 0
					&& constant.location != ~0u )
				{
					auto offset = int32_t( constant.offset );
					assert( offset >= 0 && offset + constant.size <= data.size() );
					auto buffer = data.data() + offset;

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

	void buildPushConstantsCommand( VkDevice device
		, VkShaderStageFlags stageFlags
		, [[maybe_unused]] VkPipelineLayout pipelineLayout
		, ConstantsLayout const & constants
		, ByteArray const & data
		, CmdList & list )
	{
		glLogCommand( list, "PushConstantsCommand" );

		if ( hasProgramPipelines( device ) )
		{
			buildPushProgramUniformsCommands( stageFlags, constants, data, list );
		}
		else
		{
			buildPushUniformsCommands( stageFlags, constants, data, list );
		}
	}
}
