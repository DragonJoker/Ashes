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
		std::string doRetrieveLinkerLog( Device const & device
			, GLuint programName )
		{
			std::string log;
			int infologLength = 0;
			int charsWritten = 0;
			glLogCall( device.getContext(), glGetProgramiv, programName, GL_INFO_LOG_LENGTH, &infologLength );

			if ( infologLength > 0 )
			{
				std::vector< char > infoLog( infologLength + 1 );
				glLogCall( device.getContext(), glGetProgramInfoLog, programName, infologLength, &charsWritten, infoLog.data() );
				log = infoLog.data();
			}

			if ( !log.empty() )
			{
				log = log.substr( 0, log.size() - 1 );
			}

			return log;
		}

		std::string doRetrieveCompilerLog( Device const & device
			, GLuint shaderName )
		{
			std::string log;
			int infologLength = 0;
			int charsWritten = 0;
			glLogCall( device.getContext(), glGetShaderiv, shaderName, GL_INFO_LOG_LENGTH, &infologLength );

			if ( infologLength > 0 )
			{
				std::vector< char > infoLog( infologLength + 1 );
				glLogCall( device.getContext(), glGetShaderInfoLog, shaderName, infologLength, &charsWritten, infoLog.data() );
				log = infoLog.data();
			}

			if ( !log.empty() )
			{
				log = log.substr( 0, log.size() - 1 );
			}

			return log;
		}

		bool doCheckCompileErrors( Device const & device
			, bool compiled
			, GLuint shaderName )
		{
			auto compilerLog = doRetrieveCompilerLog( device
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

		void doInitialiseState( Device const & device
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

					glLogCall( device.getContext(), glSpecializeShader_ARB
						, shader
						, stage.entryPoint.c_str()
						, count
						, indices.data()
						, values.data() );
				}
				else
				{
					glLogCall( device.getContext(), glSpecializeShader_ARB
						, shader
						, stage.entryPoint.c_str()
						, 0u
						, nullptr
						, nullptr );
				}

				int compiled = 0;
				glLogCall( device.getContext(), glGetShaderiv, shader, GL_INFO_COMPILE_STATUS, &compiled );

				if ( !doCheckCompileErrors( device, compiled != 0, shader ) )
				{
					throw std::runtime_error{ "Shader compilation failed." };
				}
			}
		}
	}

	ShaderProgram::ShaderProgram( Device const & device
		, std::vector< renderer::ShaderStageState > const & stages )
		: m_device{ device }
		, m_program{ m_device.getContext().glCreateProgram() }
	{
		for ( auto & stage : stages )
		{
			auto & module = static_cast< ShaderModule const & >( *stage.module );
			m_shaders.push_back( module.getShader() );
			doInitialiseState( m_device, stage );
			glLogCall( m_device.getContext(), glAttachShader, m_program, m_shaders.back() );
		}
	}

	ShaderProgram::ShaderProgram( Device const & device
		, renderer::ShaderStageState const & stage )
		: m_device{ device }
		, m_program{ m_device.getContext().glCreateProgram() }
	{
		auto & module = static_cast< ShaderModule const & >( *stage.module );
		m_shaders.push_back( module.getShader() );
		doInitialiseState( m_device, stage );
		glLogCall( m_device.getContext(), glAttachShader, m_program, m_shaders.back() );
	}

	ShaderProgram::~ShaderProgram()
	{
		for ( auto shaderName : m_shaders )
		{
			glLogCall( m_device.getContext(), glDeleteShader, shaderName );
		}

		glLogCall( m_device.getContext(), glDeleteProgram, m_program );
	}

	void ShaderProgram::link()const
	{
		int attached = 0;
		glLogCall( m_device.getContext(), glGetProgramiv, m_program, GL_INFO_ATTACHED_SHADERS, &attached );
		glLogCall( m_device.getContext(), glLinkProgram, m_program );
		int linked = 0;
		glLogCall( m_device.getContext(), glGetProgramiv, m_program, GL_INFO_LINK_STATUS, &linked );
		auto linkerLog = doRetrieveLinkerLog( m_device, m_program );

		if ( linked
			&& attached == int( m_shaders.size() )
			&& linkerLog.find( "ERROR" ) == std::string::npos )
		{
			if ( !linkerLog.empty() )
			{
				renderer::Logger::logWarning( "ShaderProgram::link - " + linkerLog );
			}

			int validated = 0;
			glLogCall( m_device.getContext(), glGetProgramiv, m_program, GL_INFO_VALIDATE_STATUS, &validated );

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
