/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/GlShaderModule.hpp"

#include "Core/GlDevice.hpp"
#include "Core/GlPhysicalDevice.hpp"
#include "Core/GlRenderer.hpp"

#include <Core/Renderer.hpp>
#include <Pipeline/ShaderStageState.hpp>

#include <iostream>
#include <regex>

#include "spirv_cpp.hpp"
#include "spirv_cross_util.hpp"
#include "spirv_glsl.hpp"

namespace gl_renderer
{
	namespace
	{
		static uint32_t constexpr OpCodeSPIRV = 0x07230203;

		struct BlockLocale
		{
			BlockLocale()
			{
				m_prvLoc = std::locale( "" );

				if ( m_prvLoc.name() != "C" )
				{
					std::locale::global( std::locale{ "C" } );
				}
			}

			~BlockLocale()
			{
				if ( m_prvLoc.name() != "C" )
				{
					std::locale::global( m_prvLoc );
				}
			}

		private:
			std::locale m_prvLoc;
		};

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
			, GLuint shaderName
			, std::string const & source )
		{
			auto compilerLog = doRetrieveCompilerLog( context
				, shaderName );

			if ( !compilerLog.empty() )
			{
				if ( !compiled )
				{
					ashes::Logger::logError( compilerLog );
					ashes::Logger::logError( source );
				}
				else
				{
					ashes::Logger::logWarning( compilerLog );
					ashes::Logger::logWarning( source );
				}
			}
			else if ( !compiled )
			{
				ashes::Logger::logError( "Shader compilation failed" );
				ashes::Logger::logError( source );
			}

			return compiled;
		}

		spv::ExecutionModel getExecutionModel( ashes::ShaderStageFlag stage )
		{
			spv::ExecutionModel result{};

			switch ( stage )
			{
			case ashes::ShaderStageFlag::eVertex:
				result = spv::ExecutionModelVertex;
				break;
			case ashes::ShaderStageFlag::eGeometry:
				result = spv::ExecutionModelGeometry;
				break;
			case ashes::ShaderStageFlag::eTessellationControl:
				result = spv::ExecutionModelTessellationControl;
				break;
			case ashes::ShaderStageFlag::eTessellationEvaluation:
				result = spv::ExecutionModelTessellationEvaluation;
				break;
			case ashes::ShaderStageFlag::eFragment:
				result = spv::ExecutionModelFragment;
				break;
			case ashes::ShaderStageFlag::eCompute:
				result = spv::ExecutionModelGLCompute;
				break;
			default:
				assert( false && "Unsupported shader stage flag" );
				break;
			}

			return result;
		}

		void doFillConstant( ashes::SpecialisationInfoBase const & specialisationInfo
			, ashes::SpecialisationMapEntry const & entry
			, spirv_cross::SPIRType const & type
			, spirv_cross::SPIRConstant & constant )
		{
			auto offset = entry.offset;
			auto size = type.width * type.vecsize;

			for ( auto col = 0u; col < type.vecsize; ++col )
			{
				for ( auto vec = 0u; vec < type.vecsize; ++vec )
				{
					std::memcpy( &constant.m.c[col].r[vec]
						, specialisationInfo.getData() + offset
						, size );
					offset += size;
				}
			}
		}

		void doProcessSpecializationConstants( ashes::ShaderStageState const & state
			, spirv_cross::CompilerGLSL & compiler )
		{
			if ( state.specialisationInfo )
			{
				auto constants = compiler.get_specialization_constants();

				for ( auto & spec : *state.specialisationInfo )
				{
					auto it = std::find_if( constants.begin()
						, constants.end()
						, [&spec]( spirv_cross::SpecializationConstant const & lookup )
						{
							return lookup.constant_id == spec.constantID;
						} );

					if ( it != constants.end() )
					{
						auto & constant = compiler.get_constant( it->id );
						auto & type = compiler.get_type( constant.constant_type );
						doFillConstant( *state.specialisationInfo
							, spec
							, type
							, constant );
					}
				}
			}
		}

		void doSetEntryPoint( ashes::ShaderStageFlag stage
			, spirv_cross::CompilerGLSL & compiler )
		{
			auto model = getExecutionModel( stage );
			std::string entryPoint;

			for ( auto & e : compiler.get_entry_points_and_stages() )
			{
				if ( entryPoint.empty() && e.execution_model == model )
				{
					entryPoint = e.name;
				}
			}

			if ( entryPoint.empty() )
			{
				throw std::runtime_error{ "Could not find an entry point with stage: " + getName( stage ) };
			}

			compiler.set_entry_point( entryPoint, model );
		}

		void doSetupOptions( Device const & device
			, spirv_cross::CompilerGLSL & compiler )
		{
			auto options = compiler.get_common_options();
			options.version = device.getShaderVersion();
			options.es = false;
			options.separate_shader_objects = true;
			options.enable_420pack_extension = true;
			options.vertex.fixup_clipspace = false;
			options.vertex.flip_vert_y = true;
			options.vertex.support_nonzero_base_instance = true;
			compiler.set_common_options( options );
		}

		std::string compileSpvToGlsl( Device const & device
			, ashes::UInt32Array const & shader
			, ashes::ShaderStageFlag stage
			, ashes::ShaderStageState const & state )
		{
			if ( shader[0] == OpCodeSPIRV )
			{
				BlockLocale guard;
				auto compiler = std::make_unique< spirv_cross::CompilerGLSL >( shader );
				doProcessSpecializationConstants( state, *compiler );
				doSetEntryPoint( stage, *compiler );
				doSetupOptions( device, *compiler );
				return compiler->compile();
			}

			std::vector< char > glslCode( shader.size() * sizeof( uint32_t ) );
			std::memcpy( glslCode.data(), shader.data(), glslCode.size() );
			return std::string( glslCode.data(), glslCode.data() + strlen( glslCode.data() ) );
		}
	}

	ShaderModule::ShaderModule( Device const & device
		, ashes::ShaderStageFlag stage )
		: ashes::ShaderModule{ device, stage }
		, m_device{ device }
		, m_shader{ m_device.getContext()->glCreateShader( convert( stage ) ) }
	{
	}

	ShaderModule::~ShaderModule()
	{
		// Shader object is destroyed by the ShaderProgram.
	}

	void ShaderModule::compile( ashes::ShaderStageState const & state )const
	{
		m_source = compileSpvToGlsl( m_device
			, m_spv
			, m_stage
			, state );

		if ( m_source.find( "samplerCubeArray" ) != std::string::npos )
		{
			std::regex regex{ R"(#version[ ]*\d*)" };
			m_source = std::regex_replace( m_source.data()
				, regex
				, R"($&
#extension GL_ARB_texture_cube_map_array: enable
)" );
			
		}

		auto length = int( m_source.size() );
		char const * data = m_source.data();
		auto context = m_device.getContext();
		glLogCall( context
			, glShaderSource
			, m_shader
			, 1
			, &data
			, &length );
		glLogCall( context
			, glCompileShader
			, m_shader );
		int compiled = 0;
		glLogCall( context
			, glGetShaderiv
			, m_shader
			, GL_INFO_COMPILE_STATUS
			, &compiled );

		if ( !doCheckCompileErrors( context, compiled != 0, m_shader, m_source ) )
		{
			throw std::runtime_error{ "Shader compilation failed." };
		}
	}

	void ShaderModule::loadShader( ashes::UInt32Array const & shader )
	{
		m_spv = shader;
	}
}
