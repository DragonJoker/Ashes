/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/GlShaderProgram.hpp"

#include "Core/GlDevice.hpp"
#include "Miscellaneous/GlValidator.hpp"
#include "Shader/GlShaderModule.hpp"

#include <Ashes/Pipeline/ShaderStageState.hpp>

#include <iostream>

namespace gl_renderer
{
	namespace
	{
		std::string doRetrieveLinkerLog( ContextLock const & context
			, GLuint programName )
		{
			std::string log;
			int infologLength = 0;
			int charsWritten = 0;
			glLogCall( context
				, glGetProgramiv
				, programName
				, GL_INFO_LOG_LENGTH
				, &infologLength );

			if ( infologLength > 0 )
			{
				std::vector< char > infoLog( infologLength + 1 );
				glLogCall( context
					, glGetProgramInfoLog
					, programName
					, infologLength
					, &charsWritten
					, infoLog.data() );
				log = infoLog.data();
			}

			if ( !log.empty() )
			{
				log = log.substr( 0, log.size() - 1 );
			}

			return log;
		}

		std::string doRetrieveCompilerLog( ContextLock const & context
			, GLuint shaderName )
		{
			std::string log;
			int infologLength = 0;
			int charsWritten = 0;
			glLogCall( context
				, glGetShaderiv
				, shaderName
				, GL_INFO_LOG_LENGTH
				, &infologLength );

			if ( infologLength > 0 )
			{
				std::vector< char > infoLog( infologLength + 1 );
				glLogCall( context
					, glGetShaderInfoLog
					, shaderName
					, infologLength
					, &charsWritten
					, infoLog.data() );
				log = infoLog.data();
			}

			if ( !log.empty() )
			{
				log = log.substr( 0, log.size() - 1 );
			}

			return log;
		}

		bool doCheckCompileErrors( ContextLock const & context
			, bool compiled
			, GLuint shaderName )
		{
			auto compilerLog = doRetrieveCompilerLog( context
				, shaderName );

			if ( !compilerLog.empty() )
			{
				if ( !compiled )
				{
					ashes::Logger::logError( compilerLog );
				}
				else
				{
					ashes::Logger::logWarning( compilerLog );
				}
			}
			else if ( !compiled )
			{
				ashes::Logger::logError( "Shader compilation failed" );
			}

			return compiled;
		}
	}

	ShaderProgram::ShaderProgram( Device const & device
		, std::vector< ashes::ShaderStageState > const & stages )
		: m_device{ device }
	{
		auto context = m_device.getContext();
		m_program = context->glCreateProgram();

		for ( auto & stage : stages )
		{
			auto & module = static_cast< ShaderModule const & >( *stage.module );
			m_stageFlags |= module.getStage();
			m_shaders.push_back( module.getShader() );
			module.compile( stage );
			glLogCall( context
				, glAttachShader
				, m_program
				, m_shaders.back() );
		}
	}

	ShaderProgram::ShaderProgram( Device const & device
		, ashes::ShaderStageState const & stage )
		: m_device{ device }
		, m_stageFlags{ stage.module->getStage() }
	{
		auto context = m_device.getContext();
		m_program = context->glCreateProgram();
		auto & module = static_cast< ShaderModule const & >( *stage.module );
		m_shaders.push_back( module.getShader() );
		module.compile( stage );
		glLogCall( context
			, glAttachShader
			, m_program
			, m_shaders.back() );
	}

	ShaderProgram::~ShaderProgram()
	{
		auto context = m_device.getContext();

		for ( auto shaderName : m_shaders )
		{
			glLogCall( context
				, glDeleteShader
				, shaderName );
		}

		glLogCall( context
			, glDeleteProgram
			, m_program );
	}

	ShaderDesc ShaderProgram::link( ContextLock const & context )const
	{
		int attached = 0;
		glLogCall( context
			, glGetProgramiv
			, m_program
			, GL_INFO_ATTACHED_SHADERS
			, &attached );
		glLogCall( context
			, glLinkProgram
			, m_program );
		int linked = 0;
		glLogCall( context
			, glGetProgramiv
			, m_program
			, GL_INFO_LINK_STATUS
			, &linked );
		auto linkerLog = doRetrieveLinkerLog( context, m_program );
		ShaderDesc result;

		if ( linked
			&& attached == int( m_shaders.size() )
			&& linkerLog.find( "ERROR" ) == std::string::npos )
		{
			if ( !linkerLog.empty() )
			{
				ashes::Logger::logWarning( "ShaderProgram::link - " + linkerLog );
			}

			int validated = 0;
			glLogCall( context
				, glGetProgramiv
				, m_program
				, GL_INFO_VALIDATE_STATUS
				, &validated );

			if ( !validated )
			{
				ashes::Logger::logWarning( "ShaderProgram::link - Not validated" );
			}

			result = getShaderDesc( context, m_program );
			result.stageFlags = m_stageFlags;
		}
		else
		{
			if ( !linkerLog.empty() )
			{
				ashes::Logger::logError( "ShaderProgram::link - " + linkerLog );
			}

			if ( attached != int( m_shaders.size() ) )
			{
				ashes::Logger::logError( "ShaderProgram::link - The linked shaders count doesn't match the active shaders count." );
			}
		}

		return result;
	}
}
