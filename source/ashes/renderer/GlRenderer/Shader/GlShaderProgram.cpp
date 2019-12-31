/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/GlShaderProgram.hpp"

#include "Core/GlDevice.hpp"
#include "Miscellaneous/GlValidator.hpp"
#include "Shader/GlShaderModule.hpp"

#include "ashesgl_api.hpp"

#include <iostream>

namespace ashes::gl
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

		InterfaceBlocksLayout merge( InterfaceBlocksLayout const & lhs
			, InterfaceBlocksLayout const & rhs )
		{
			InterfaceBlocksLayout result{ lhs };

			if ( !rhs.empty() )
			{
				if ( result.empty() )
				{
					result = rhs;
				}
				else
				{
					for ( auto & constantBuffer : rhs )
					{
						auto it = std::find_if( result.begin()
							, result.end()
							, [&constantBuffer]( ConstantBufferDesc const & lookup )
							{
								return lookup.binding == constantBuffer.binding;
							} );

						if ( it == result.end() )
						{
							result.push_back( constantBuffer );
						}
					}
				}
			}

			std::sort( result.begin()
				, result.end()
				, []( ConstantBufferDesc const & lhs, ConstantBufferDesc const & rhs )
				{
					return lhs.binding < rhs.binding;
				} );

			return result;
		}

		template< typename FormatT >
		DescLayoutT< FormatT > merge( DescLayoutT< FormatT > const & lhs
			, DescLayoutT< FormatT > const & rhs )
		{
			DescLayoutT< FormatT > result{ lhs };

			if ( !rhs.empty() )
			{
				if ( result.empty() )
				{
					result = rhs;
				}
				else
				{
					for ( auto & constant : rhs )
					{
						auto it = std::find_if( result.begin()
							, result.end()
							, [&constant]( FormatDescT< FormatT > const & lookup )
							{
								return lookup.name == constant.name
									&& lookup.program == constant.program;
							} );

						if ( it == result.end() )
						{
							result.push_back( constant );
						}
					}
				}
			}

			std::sort( result.begin()
				, result.end()
				, []( FormatDescT< FormatT > const & lhs, FormatDescT< FormatT > const & rhs )
				{
					return lhs.offset < rhs.offset;
				} );

			return result;
		}

		ShaderDesc merge( std::vector< ShaderDesc > const & descs )
		{
			ShaderDesc result{};

			for ( auto & desc : descs )
			{
				result.isGlsl = result.isGlsl || desc.isGlsl;
				result.inputs.vertexAttributeDescriptions.insert( result.inputs.vertexAttributeDescriptions.end()
					, desc.inputs.vertexAttributeDescriptions.begin()
					, desc.inputs.vertexAttributeDescriptions.end() );
				result.inputs.vertexBindingDescriptions.insert( result.inputs.vertexBindingDescriptions.end()
					, desc.inputs.vertexBindingDescriptions.begin()
					, desc.inputs.vertexBindingDescriptions.end() );
				result.stageFlags |= desc.stageFlags;
				result.pcb = merge( result.pcb
					, desc.pcb );
				result.ubo = merge( result.ubo
					, desc.ubo );
				result.sbo = merge( result.sbo
					, desc.sbo );
				result.tbo = merge( result.tbo
					, desc.tbo );
				result.img = merge( result.img
					, desc.img );
				result.tex = merge( result.tex
					, desc.tex );
			}

			return result;
		}
	}

	ShaderProgram::ShaderProgram( VkDevice device
		, ContextState * state
		, VkPipeline pipeline
		, VkPipelineShaderStageCreateInfoArray stages
		, VkPipelineLayout layout
		, VkPipelineCreateFlags createFlags
		, VkRenderPass renderPass
		, Optional< VkPipelineVertexInputStateCreateInfo > const & vertexInputState
		, bool invertY )
		: m_device{ device }
		, stages{ std::move( stages ) }
		, bindings{ get( layout )->getShaderBindings() }
	{
		auto context = get( m_device )->getContext();

		if ( state )
		{
			ContextStateStack stack{ device };
			stack.apply( context
				, *state );
		}

		std::vector< ShaderDesc > descs;

		for ( auto & stage : this->stages )
		{
			stageFlags |= stage.stage;
			descs.push_back( get( stage.module )->compile( stage
				, layout
				, createFlags
				, invertY ) );
		}

		if ( isGl4( m_device ) )
		{
			doInitProgramPipeline( context
				, std::move( descs )
				, layout
				, createFlags
				, renderPass
				, vertexInputState
				, invertY );
		}
		else
		{
			doInitShaderProgram( context
				, std::move( descs )
				, layout
				, createFlags
				, invertY );
		}
	}

	ShaderProgram::~ShaderProgram()
	{
		auto context = get( m_device )->getContext();

		if ( isGl4( m_device ) )
		{
			doCleanupProgramPipeline( context );
		}
		else
		{
			doCleanupShaderProgram( context );
		}
	}

	void ShaderProgram::doInitProgramPipeline( ContextLock const & context
		, std::vector< ShaderDesc > descs
		, VkPipelineLayout layout
		, VkPipelineCreateFlags createFlags
		, VkRenderPass renderPass
		, Optional< VkPipelineVertexInputStateCreateInfo > const & vertexInputState
		, bool invertY )
	{
		program = merge( descs );
		glLogCall( context
			, glGenProgramPipelines
			, 1
			, &program.program );
		glLogCall( context
			, glBindProgramPipeline
			, program.program );
		uint32_t index = 0u;

		for ( auto & desc : descs )
		{
			glLogCall( context
				, glUseProgramStages
				, program.program
				, convertShaderStageFlag( stages[index].stage )
				, desc.program );
			modules.push_back( desc.program );
			++index;
		}

		glLogCall( context
			, glBindProgramPipeline
			, 0 );

		constantsPcb.stageFlags = program.stageFlags;
		uint32_t size = 0u;

		for ( auto & constant : program.pcb )
		{
			constantsPcb.constants.push_back( constant );
			size += constant.size;
		}

		constantsPcb.size = size;

		if ( get( getInstance( m_device ) )->isValidationEnabled()
			&& renderPass != VK_NULL_HANDLE )
		{
			validatePipeline( context
				, layout
				, program.program
				, vertexInputState.value()
				, renderPass );
		}
	}

	void ShaderProgram::doInitShaderProgram( ContextLock const & context
		, std::vector< ShaderDesc > descs
		, VkPipelineLayout layout
		, VkPipelineCreateFlags createFlags
		, bool invertY )
	{
		auto programObject = context->glCreateProgram();

		for ( auto & desc : descs )
		{
			glLogCall( context
				, glAttachShader
				, programObject
				, descs.back().program );
			modules.push_back( desc.program );
		}

		int attached = 0;
		glLogCall( context
			, glGetProgramiv
			, programObject
			, GL_INFO_ATTACHED_SHADERS
			, &attached );
		glLogCall( context
			, glLinkProgram
			, programObject );
		int linked = 0;
		glLogCall( context
			, glGetProgramiv
			, programObject
			, GL_INFO_LINK_STATUS
			, &linked );
		auto linkerLog = doRetrieveLinkerLog( context, programObject );
		ShaderDesc result;

		if ( linked
			&& attached == int( modules.size() )
			&& linkerLog.find( "ERROR" ) == std::string::npos )
		{
			int validated = 0;
			glLogCall( context
				, glGetProgramiv
				, programObject
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

				reportWarning( m_device
					, VK_ERROR_VALIDATION_FAILED_EXT
					, "Shader link"
					, stream.str() );
			}

			program = getShaderDesc( context
				, VkShaderStageFlagBits( stageFlags )
				, programObject );
			program.program = programObject;
			auto constants = mergeConstants( stages );

			if ( !constants.empty() )
			{
				assert( constants.size() >= program.pcb.size() );
				for ( auto & constant : program.pcb )
				{
					auto it = std::find_if( constants.begin()
						, constants.end()
						, [&constant]( ConstantDesc const & lookup )
						{
							return lookup.name == constant.name;
						} );
					assert( it != constants.end() );
					constant.offset = it->offset;
					constant.stageFlag = it->stageFlag;
				}
			}
			else
			{
				uint32_t offset = 0u;

				for ( auto & constant : program.pcb )
				{
					constant.offset = offset;
					offset += constant.size;
				}
			}
		}
		else
		{
			std::stringstream stream;
			stream << "Shader program link: " << std::endl;

			if ( !linked )
			{
				stream << "Not linked - " << std::endl;
			}

			if ( attached != int( modules.size() ) )
			{
				stream << "The linked shaders count doesn't match the active shaders count. - " << std::endl;
			}

			if ( !linkerLog.empty() )
			{
				stream << linkerLog << std::endl;
			}

			reportError( m_device
				, VK_ERROR_VALIDATION_FAILED_EXT
				, "Shader link"
				, stream.str() );
		}
	}

	void ShaderProgram::doCleanupProgramPipeline( ContextLock const & context )
	{
		glLogCall( context
			, glDeleteProgramPipelines
			, 1u
			, &program.program );

		for ( auto & module : modules )
		{
			glLogCall( context
				, glDeleteProgram
				, module );
		}
	}

	void ShaderProgram::doCleanupShaderProgram( ContextLock const & context )
	{
		for ( auto shaderName : modules )
		{
			glLogCall( context
				, glDeleteShader
				, shaderName );
		}

		glLogCall( context
			, glDeleteProgram
			, program.program );
	}
}
