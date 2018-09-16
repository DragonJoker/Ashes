/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/GlShaderModule.hpp"

#include "Core/GlDevice.hpp"
#include "Core/GlPhysicalDevice.hpp"
#include "Core/GlRenderer.hpp"

#include <Core/Renderer.hpp>

#include <iostream>
#include <regex>

#include "spirv_cpp.hpp"
#include "spirv_cross_util.hpp"
#include "spirv_glsl.hpp"

namespace gl_renderer
{
	namespace
	{
		template< typename CleanFunc >
		struct BlockGuard
		{
			template< typename InitFunc >
			BlockGuard( InitFunc init, CleanFunc clean )
				: m_clean{ std::move( clean ) }
			{
				init();
			}

			~BlockGuard()
			{
				m_clean();
			}

		private:
			CleanFunc m_clean;
		};

		template< typename InitFunc, typename CleanFunc >
		BlockGuard< CleanFunc > makeBlockGuard( InitFunc init, CleanFunc clean )
		{
			return BlockGuard< CleanFunc >{ std::move( init ), std::move( clean ) };
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

		std::string SpvToGlsl( Device const & device
			, ashes::UInt32Array const & shader
			, ashes::ShaderStageFlag stage )
		{
			auto prvLoc = std::locale( "" );

			auto guard = makeBlockGuard(
				[&prvLoc]()
				{
					if ( prvLoc.name() != "C" )
					{
						std::locale::global( std::locale{ "C" } );
					}
				},
				[&prvLoc]()
				{
					if ( prvLoc.name() != "C" )
					{
						std::locale::global( prvLoc );
					}
				} );

			auto compiler = std::make_unique< spirv_cross::CompilerGLSL >( shader );
			auto model = getExecutionModel( stage );
			std::string entryPoint;

			for ( auto & e : compiler->get_entry_points_and_stages() )
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

			compiler->set_entry_point( entryPoint, model );
			auto options = compiler->get_common_options();
			options.version = device.getShaderVersion();
			options.es = false;
			options.separate_shader_objects = true;
			options.enable_420pack_extension = true;
			options.vertex.fixup_clipspace = false;
			options.vertex.flip_vert_y = true;
			options.vertex.support_nonzero_base_instance = true;
			compiler->set_common_options( options );

			auto shaderResources = compiler->get_shader_resources();

			return compiler->compile();
		}
	}

	ShaderModule::ShaderModule( Device const & device
		, ashes::ShaderStageFlag stage )
		: ashes::ShaderModule{ device, stage }
		, m_device{ device }
		, m_shader{ m_device.getContext()->glCreateShader( convert( stage ) ) }
		, m_isSpirV{ false }
	{
	}

	ShaderModule::~ShaderModule()
	{
		// Shader object is destroyed by the ShaderProgram.
	}

	std::string SpvToGlsl( ashes::UInt32Array const & shader )
	{
		return std::string{};
	}

	void ShaderModule::loadShader( ashes::UInt32Array const & shader )
	{
		if ( static_cast< Renderer const & >( m_device.getRenderer() ).isSPIRVSupported() )
		{
			auto context = m_device.getContext();
			context->glShaderBinary_ARB( 1u
				, &m_shader
				, GL_SHADER_BINARY_FORMAT_SPIR_V
				, shader.data()
				, GLsizei( shader.size() * sizeof( uint32_t ) ) );
			m_isSpirV = true;
		}
		else
		{
			std::string source = SpvToGlsl( m_device, shader, m_stage );
			auto length = int( source.size() );
			char const * data = source.data();
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

			if ( !doCheckCompileErrors( context, compiled != 0, m_shader ) )
			{
				throw std::runtime_error{ "Shader compilation failed." };
			}
		}
	}
}
