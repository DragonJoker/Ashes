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
		ConstantsLayout mergeConstants( VkPipelineShaderStageCreateInfoArray const & stages )
		{
			ConstantsLayout result;

			for ( auto & stage : stages )
			{
				if ( auto & constants = get( stage.module )->getConstants();
					!constants.empty() )
				{
					if ( result.empty() )
					{
						result = constants;
					}
					else
					{
						for ( auto & constant : constants )
						{
							if ( auto it = std::find_if( result.begin()
									, result.end()
									, [&constant]( ConstantDesc const & lookup )
									{
										return lookup.name == constant.name;
									} );
								it == result.end() )
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
						if ( auto it = std::find_if( result.begin()
								, result.end()
								, [&constantBuffer]( ConstantBufferDesc const & lookup )
								{
									return lookup.binding == constantBuffer.binding;
								} );
							it == result.end() )
						{
							result.push_back( constantBuffer );
						}
					}
				}
			}

			std::sort( result.begin()
				, result.end()
				, []( ConstantBufferDesc const & tlhs, ConstantBufferDesc const & trhs )
				{
					return tlhs.binding < trhs.binding;
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
#pragma warning( push )
#pragma warning( disable:5233 )
						if ( auto it = std::find_if( result.begin()
								, result.end()
								, [&constant]( FormatDescT< FormatT > const & lookup )
								{
									return lookup.name == constant.name
										&& lookup.program == constant.program;
								} );
							it == result.end() )
						{
							result.push_back( constant );
						}
#pragma warning( pop )
					}
				}
			}

			std::sort( result.begin()
				, result.end()
				, []( FormatDescT< FormatT > const & tlhs, FormatDescT< FormatT > const & trhs )
				{
					return tlhs.offset < trhs.offset;
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
		, ContextState const * state
		, VkPipeline pipeline
		, VkPipelineShaderStageCreateInfoArray stages
		, VkPipelineLayout layout
		, VkPipelineCreateFlags createFlags
		, VkRenderPass renderPass
		, Optional< VkPipelineVertexInputStateCreateInfo > const & vertexInputState
		, bool invertY )
		: m_device{ device }
		, bindings{ get( layout )->getShaderBindings() }
		, stages{ std::move( stages ) }
	{
		auto context = get( m_device )->getContext();

		if ( state )
		{
			ContextStateStack stack{ device };
			stack.apply( context
				, *state );
		}

		std::vector< ShaderDesc > descs;
		VkPipelineShaderStageCreateInfo const * previousStage{ nullptr };

		for ( auto & stage : this->stages )
		{
			stageFlags |= stage.stage;
			ShaderDesc desc{};

			if ( auto result = get( stage.module )->compile( pipeline
					, previousStage
					, stage
					, layout
					, createFlags
					, invertY
					, desc );
				result != VK_SUCCESS )
			{
				throw Exception{ result, "ShaderModule compilation" };
			}

			descs.push_back( desc );
			previousStage = &stage;
		}

		if ( hasProgramPipelines( m_device ) )
		{
			doInitProgramPipeline( context
				, descs
				, layout
				, renderPass
				, vertexInputState );
		}
		else
		{
			doInitShaderProgram( context
				, pipeline
				, descs );
		}
	}

	ShaderProgram::~ShaderProgram()noexcept
	{
		auto context = get( m_device )->getContext();

		if ( hasProgramPipelines( m_device ) )
		{
			doCleanupProgramPipeline( context );
		}
		else
		{
			doCleanupShaderProgram( context );
		}
	}

	void ShaderProgram::doInitProgramPipeline( ContextLock const & context
		, std::vector< ShaderDesc > const & descs
		, VkPipelineLayout layout
		, VkRenderPass renderPass
		, Optional< VkPipelineVertexInputStateCreateInfo > const & vertexInputState )
	{
		program = merge( descs );
		glLogCreateCall( context
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
				, convert( stages[index].stage )
				, desc.program );
			modules.push_back( desc.program );
			++index;
		}

		glLogCall( context
			, glBindProgramPipeline
			, 0 );

		constantsPcb.stageFlags = program.stageFlags;
		uint32_t size = 0u;

		for ( auto const & constant : program.pcb )
		{
			constantsPcb.constants.push_back( constant );
			size += constant.size;
		}

		constantsPcb.size = size;

		if ( get( getInstance( m_device ) )->isValidationEnabled()
			&& renderPass != nullptr )
		{
			validatePipeline( context
				, program.program
				, vertexInputState.value()
				, renderPass );
		}
	}

	void ShaderProgram::doInitShaderProgram( ContextLock const & context
		, VkPipeline pipeline
		, std::vector< ShaderDesc > const & descs )
	{
		auto programObject = glLogNonVoidEmptyCall( context
			, glCreateProgram );
		bool isGlsl = true;

		for ( auto & desc : descs )
		{
			glLogCall( context
				, glAttachShader
				, programObject
				, desc.program );
			modules.push_back( desc.program );
			isGlsl = isGlsl && desc.isGlsl;
		}

		glLogCall( context
			, glLinkProgram
			, programObject );

		if ( checkLinkErrors( context
			, pipeline
			, programObject
			, int( modules.size() )
			, "Shader program link" ) )
		{
			auto constants = mergeConstants( stages );
			program = getShaderDesc( context
				, constants
				, VkShaderStageFlagBits( stageFlags )
				, programObject );
			program.program = programObject;
			program.stageFlags = stageFlags;
		}

		for ( auto & shaderName : modules )
		{
			if ( shaderName )
			{
				glLogCall( context
					, glDeleteShader
					, shaderName );
				shaderName = 0;
			}
		}
	}

	void ShaderProgram::doCleanupProgramPipeline( ContextLock const & context )noexcept
	{
		if ( program.program )
		{
			glLogCall( context
				, glDeleteProgramPipelines
				, 1u
				, &program.program );
			program.program = 0;
		}

		for ( auto & shaderModule : modules )
		{
			if ( shaderModule )
			{
				glLogCall( context
					, glDeleteProgram
					, shaderModule );
				shaderModule = 0;
			}
		}
	}

	void ShaderProgram::doCleanupShaderProgram( ContextLock const & context )noexcept
	{
		if ( program.program )
		{
			glLogCall( context
				, glDeleteProgram
				, program.program );
			program.program = 0;
		}
	}
}
