/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/GlShaderProgram.hpp"

#include "Core/GlDevice.hpp"
#include "Miscellaneous/GlValidator.hpp"
#include "Shader/GlShaderModule.hpp"

#include "ashesgl4_api.hpp"

#include <iostream>

namespace ashes::gl4
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
					std::cerr << "Shader compilation failed" << std::endl;
				}

				std::cerr << compilerLog << std::endl;
			}
			else if ( !compiled )
			{
				std::cerr << "Shader compilation failed" << std::endl;
			}

			return compiled;
		}
	}

	ShaderProgram::ShaderProgram( VkDevice device
		, VkPipelineShaderStageCreateInfoArray const & stages )
		: m_device{ device }
	{
		auto context = get( m_device )->getContext();
		m_program = context->glCreateProgram();

		for ( auto & stage : stages )
		{
			m_stageFlags |= stage.stage;
			m_shaders.push_back( get( stage.module )->getInternal() );
			get( stage.module )->compile( stage );
			glLogCall( context
				, glAttachShader
				, m_program
				, m_shaders.back() );
		}
	}

	ShaderProgram::ShaderProgram( VkDevice device
		, VkPipelineShaderStageCreateInfo const & stage )
		: ShaderProgram{ device, { 1u, stage } }
	{
	}

	ShaderProgram::~ShaderProgram()
	{
		auto context = get( m_device )->getContext();

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
			int validated = 0;
			glLogCall( context
				, glGetProgramiv
				, m_program
				, GL_INFO_VALIDATE_STATUS
				, &validated );

			if ( !linkerLog.empty()
				|| !validated )
			{
				std::cerr << "ShaderProgram::link" << std::endl;

				if ( !validated )
				{
					std::cerr << "  Not validated" << std::endl;
				}

				if ( !linkerLog.empty() )
				{
					std::cerr << linkerLog << std::endl;
				}
			}

			result = getShaderDesc( context, m_program );
			result.stageFlags = m_stageFlags;
		}
		else
		{
			std::cerr << "ShaderProgram::link" << std::endl;

			if ( !linked )
			{
				std::cerr << "  Not linked" << std::endl;
			}

			if ( attached != int( m_shaders.size() ) )
			{
				std::cerr << "  The linked shaders count doesn't match the active shaders count." << std::endl;
			}

			if ( !linkerLog.empty() )
			{
				std::cerr << linkerLog << std::endl;
			}
		}

		return result;
	}
}
