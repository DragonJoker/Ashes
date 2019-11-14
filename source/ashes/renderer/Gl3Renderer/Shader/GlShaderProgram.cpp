/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/GlShaderProgram.hpp"

#include "Core/GlDevice.hpp"
#include "Miscellaneous/GlValidator.hpp"
#include "Shader/GlShaderModule.hpp"

#include "ashesgl3_api.hpp"

#include <iostream>

namespace ashes::gl3
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

		ConstantsLayout mergeConstants( VkPipelineShaderStageCreateInfoArray const & stages )
		{
			ConstantsLayout result;

			for ( auto & stage : stages )
			{
				auto & constants = get( stage.module )->getConstants();

				if ( !constants.empty() )
				{
					if ( result.empty() )
					{
						result = constants;
					}
					else
					{
						for ( auto & constant : constants )
						{
							auto it = std::find_if( result.begin()
								, result.end()
								, [&constant]( ConstantDesc const & lookup )
								{
									return lookup.name == constant.name;
								} );

							if ( it == result.end() )
							{
								result.push_back( constant );
							}
						}
					}
				}
			}

			std::sort( result.begin()
				, result.end()
				, []( ConstantDesc const & lhs, ConstantDesc const & rhs )
				{
					return lhs.offset < rhs.offset;
				} );

			return result;
		}
	}

	ShaderProgram::ShaderProgram( VkDevice device
		, VkPipeline pipeline
		, VkPipelineShaderStageCreateInfoArray stages
		, bool isRtot )
		: m_device{ device }
		, m_pipeline{ pipeline }
		, m_stages{ std::move( stages ) }
	{
		auto context = get( m_device )->getContext();
		m_program = context->glCreateProgram();

		for ( auto & stage : m_stages )
		{
			m_stageFlags |= stage.stage;
			m_shaders.push_back( get( stage.module )->compile( stage, get( pipeline )->getLayout(), isRtot ) );
			glLogCall( context
				, glAttachShader
				, m_program
				, m_shaders.back() );
		}
	}

	ShaderProgram::ShaderProgram( VkDevice device
		, VkPipeline pipeline
		, VkPipelineShaderStageCreateInfo const & stage )
		: ShaderProgram{ device, pipeline, { 1u, stage }, false }
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
				std::stringstream stream;
				stream << "Shader program link: " << std::endl;

				if ( !validated )
				{
					stream << "Not validated - " << std::endl;
				}

				if ( !linkerLog.empty() )
				{
					stream << linkerLog << std::endl;
				}

				get( get( m_device )->getInstance() )->reportMessage( VK_DEBUG_REPORT_WARNING_BIT_EXT
					, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT
					, uint64_t( m_pipeline )
					, 0u
					, 0u
					, "CORE"
					, stream.str().c_str() );
			}

			result = getShaderDesc( context, m_program );
			auto constants = mergeConstants( m_stages );

			if ( !constants.empty() )
			{
				assert( constants.size() == result.constantsLayout.size() );
				for ( auto & constant : result.constantsLayout )
				{
					auto it = std::find_if( constants.begin()
						, constants.end()
						, [&constant]( ConstantDesc const & lookup )
						{
							return lookup.name == constant.name;
						} );
					assert( it != constants.end() );
					constant.offset = it->offset;
				}
			}
			else
			{
				uint32_t offset = 0u;

				for ( auto & constant : result.constantsLayout )
				{
					constant.offset = offset;
					offset += constant.size;
				}
			}

			result.stageFlags = m_stageFlags;
		}
		else
		{
			std::stringstream stream;
			stream << "Shader program link: " << std::endl;

			if ( !linked )
			{
				stream << "Not linked - " << std::endl;
			}

			if ( attached != int( m_shaders.size() ) )
			{
				stream << "The linked shaders count doesn't match the active shaders count. - " << std::endl;
			}

			if ( !linkerLog.empty() )
			{
				stream << linkerLog << std::endl;
			}

			get( get( m_device )->getInstance() )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT
				, uint64_t( m_pipeline )
				, 0u
				, 0u
				, "CORE"
				, stream.str().c_str() );
		}

		return result;
	}
}
