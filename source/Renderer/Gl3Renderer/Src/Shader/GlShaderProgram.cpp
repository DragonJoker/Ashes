/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "Shader/GlShaderProgram.hpp"

#include "Core/GlDevice.hpp"
#include "Shader/GlShaderModule.hpp"

#include <Pipeline/ShaderStageState.hpp>

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
					renderer::Logger::logError( compilerLog );
				}
				else
				{
					renderer::Logger::logWarning( compilerLog );
				}
			}
			else if ( !compiled )
			{
				renderer::Logger::logError( "Shader compilation failed" );
			}

			return compiled;
		}

		void doInitialiseState( ContextLock const & context
			, renderer::ShaderStageState const & stage )
		{
			auto & module = static_cast< ShaderModule const & >( *stage.module );
			auto shader = module.getShader();

			if ( module.isSpirV() )
			{
				if ( stage.specialisationInfo )
				{
					auto & specialisationInfo = *stage.specialisationInfo;
					auto count = GLuint( std::distance( specialisationInfo.begin(), specialisationInfo.end() ) );
					std::vector< GLuint > indices;
					indices.reserve( count );
					std::vector< GLuint > values;
					values.reserve( count );
					auto src = reinterpret_cast< GLuint const * >( specialisationInfo.getData() );
					auto dst = values.data();

					for ( auto & constant : specialisationInfo )
					{
						indices.push_back( constant.constantID );
						values.push_back( *src );
						++src;
					}

					glLogCall( context
						, glSpecializeShader_ARB
						, shader
						, stage.entryPoint.c_str()
						, count
						, indices.data()
						, values.data() );
				}
				else
				{
					glLogCall( context
						, glSpecializeShader_ARB
						, shader
						, stage.entryPoint.c_str()
						, 0u
						, nullptr
						, nullptr );
				}

				int compiled = 0;
				glLogCall( context
					, glGetShaderiv
					, shader
					, GL_INFO_COMPILE_STATUS
					, &compiled );

				if ( !doCheckCompileErrors( context, compiled != 0, shader ) )
				{
					throw std::runtime_error{ "Shader compilation failed." };
				}
			}
		}
	}

	ShaderProgram::ShaderProgram( Device const & device
		, std::vector< renderer::ShaderStageState > const & stages )
		: m_device{ device }
	{
		auto context = m_device.getContext();
		m_program = context->glCreateProgram();

		for ( auto & stage : stages )
		{
			auto & module = static_cast< ShaderModule const & >( *stage.module );
			m_shaders.push_back( module.getShader() );
			doInitialiseState( context, stage );
			glLogCall( context
				, glAttachShader
				, m_program
				, m_shaders.back() );
		}
	}

	ShaderProgram::ShaderProgram( Device const & device
		, renderer::ShaderStageState const & stage )
		: m_device{ device }
	{
		auto context = m_device.getContext();
		m_program = context->glCreateProgram();
		auto & module = static_cast< ShaderModule const & >( *stage.module );
		m_shaders.push_back( module.getShader() );
		doInitialiseState( context, stage );
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

	void ShaderProgram::link()const
	{
		auto context = m_device.getContext();
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

		if ( linked
			&& attached == int( m_shaders.size() )
			&& linkerLog.find( "ERROR" ) == std::string::npos )
		{
			if ( !linkerLog.empty() )
			{
				renderer::Logger::logWarning( "ShaderProgram::link - " + linkerLog );
			}

			int validated = 0;
			glLogCall( context
				, glGetProgramiv
				, m_program
				, GL_INFO_VALIDATE_STATUS
				, &validated );

			if ( !validated )
			{
				renderer::Logger::logError( "ShaderProgram::link - Not validated" );
			}
		}
		else
		{
			if ( !linkerLog.empty() )
			{
				renderer::Logger::logError( "ShaderProgram::link - " + linkerLog );
			}

			if ( attached != int( m_shaders.size() ) )
			{
				renderer::Logger::logError( "ShaderProgram::link - The linked shaders count doesn't match the active shaders count." );
			}
		}
	}
}
