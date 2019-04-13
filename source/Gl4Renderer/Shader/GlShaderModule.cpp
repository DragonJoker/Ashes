/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/GlShaderModule.hpp"

#include "Core/GlDevice.hpp"
#include "Core/GlPhysicalDevice.hpp"
#include "Core/GlInstance.hpp"

#include <iostream>
#include <regex>

#if Gl4Renderer_USE_SPIRV_CROSS
#	include "spirv_cpp.hpp"
#	include "spirv_cross_util.hpp"
#	include "spirv_glsl.hpp"
#endif

#include "ashesgl4_api.hpp"

namespace ashes::gl4
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

		std::string doRetrieveCompilerLog( VkDevice device
			, GLuint shaderName )
		{
			auto context = get( device )->getContext();
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

		bool doCheckCompileErrors( VkDevice device
			, bool compiled
			, GLuint shaderName
			, std::string const & source )
		{
			auto compilerLog = doRetrieveCompilerLog( device
				, shaderName );

			if ( !compilerLog.empty() )
			{
				if ( !compiled )
				{
					std::cerr << compilerLog << std::endl;
					std::cerr << source << std::endl;
				}
				else
				{
					std::cout << compilerLog << std::endl;
					std::cout << source << std::endl;
				}
			}
			else if ( !compiled )
			{
				std::cerr << "Shader compilation failed" << std::endl;
				std::cerr << source << std::endl;
			}

			return compiled;
		}

#if Gl4Renderer_USE_SPIRV_CROSS

		spv::ExecutionModel getExecutionModel( VkShaderStageFlagBits stage )
		{
			spv::ExecutionModel result{};

			switch ( stage )
			{
			case VK_SHADER_STAGE_VERTEX_BIT:
				result = spv::ExecutionModelVertex;
				break;
			case VK_SHADER_STAGE_GEOMETRY_BIT:
				result = spv::ExecutionModelGeometry;
				break;
			case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
				result = spv::ExecutionModelTessellationControl;
				break;
			case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
				result = spv::ExecutionModelTessellationEvaluation;
				break;
			case VK_SHADER_STAGE_FRAGMENT_BIT:
				result = spv::ExecutionModelFragment;
				break;
			case VK_SHADER_STAGE_COMPUTE_BIT:
				result = spv::ExecutionModelGLCompute;
				break;
			default:
				assert( false && "Unsupported shader stage flag" );
				break;
			}

			return result;
		}

		void doFillConstant( VkSpecializationInfo const & specialisationInfo
			, VkSpecializationMapEntry const & entry
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
						, reinterpret_cast< uint8_t const * >( specialisationInfo.pData ) + offset
						, size );
					offset += size;
				}
			}
		}

		void doProcessSpecializationConstants( VkPipelineShaderStageCreateInfo const & state
			, spirv_cross::CompilerGLSL & compiler )
		{
			if ( state.pSpecializationInfo )
			{
				auto constants = compiler.get_specialization_constants();

				for ( auto itEntry = state.pSpecializationInfo->pMapEntries;
					itEntry != state.pSpecializationInfo->pMapEntries + state.pSpecializationInfo->mapEntryCount;
					++itEntry )
				{
					auto it = std::find_if( constants.begin()
						, constants.end()
						, [itEntry]( spirv_cross::SpecializationConstant const & lookup )
						{
							return lookup.constant_id == itEntry->constantID;
						} );

					if ( it != constants.end() )
					{
						auto & constant = compiler.get_constant( it->id );
						auto & type = compiler.get_type( constant.constant_type );
						doFillConstant( *state.pSpecializationInfo
							, *itEntry
							, type
							, constant );
					}
				}
			}
		}

		void doSetEntryPoint( VkShaderStageFlagBits stage
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
				throw std::runtime_error{ "Could not find an entry point with stage: " + getShaderStageFlagName( stage ) };
			}

			compiler.set_entry_point( entryPoint, model );
		}

		void doSetupOptions( VkDevice device
			, spirv_cross::CompilerGLSL & compiler
			, bool isRtot )
		{
			auto options = compiler.get_common_options();
			options.version = get( get( device )->getInstance() )->getExtensions().getShaderVersion();
			options.es = false;
			options.separate_shader_objects = true;
			options.enable_420pack_extension = true;
			options.vertex.fixup_clipspace = false;
			options.vertex.flip_vert_y = !isRtot;
			options.vertex.support_nonzero_base_instance = true;
			compiler.set_common_options( options );
		}

#endif

		std::string compileSpvToGlsl( VkDevice device
			, UInt32Array const & shader
			, VkShaderStageFlagBits stage
			, VkPipelineShaderStageCreateInfo const & state
			, bool isRtot )
		{
			if ( shader[0] == OpCodeSPIRV )
			{
#if Gl4Renderer_USE_SPIRV_CROSS

				BlockLocale guard;
				auto compiler = std::make_unique< spirv_cross::CompilerGLSL >( shader );
				doProcessSpecializationConstants( state, *compiler );
				doSetEntryPoint( stage, *compiler );
				doSetupOptions( device, *compiler, isRtot );
				return compiler->compile();

#else

				throw std::runtime_error{ "Can't parse SPIR-V shaders, pull submodule SpirvCross" };

#endif
			}

			std::vector< char > glslCode( shader.size() * sizeof( uint32_t ) );
			std::memcpy( glslCode.data(), shader.data(), glslCode.size() );
			return std::string( glslCode.data(), glslCode.data() + strlen( glslCode.data() ) );
		}

		void getSpecializationInfo( VkSpecializationInfo const * pSpecializationInfo
			, std::vector< GLuint > & indices
			, std::vector< GLuint > & values )
		{
			if ( pSpecializationInfo )
			{
				auto & specializationInfo = *pSpecializationInfo;
				auto count = GLuint( specializationInfo.mapEntryCount );
				indices.reserve( count );
				values.reserve( count );
				auto src = reinterpret_cast< GLuint const * >( specializationInfo.pData );

				for ( auto itEntry = specializationInfo.pMapEntries;
					itEntry != specializationInfo.pMapEntries + specializationInfo.mapEntryCount;
					++itEntry )
				{
					indices.push_back( itEntry->constantID );
					values.push_back( *src );
					++src;
				}
			}
		}
	}

	ShaderModule::ShaderModule( VkDevice device
		, VkShaderModuleCreateInfo createInfo )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_code{ UInt32Array( createInfo.pCode, createInfo.pCode + ( createInfo.codeSize / sizeof( uint32_t ) ) ) }
	{
	}

	GLuint ShaderModule::compile( VkPipelineShaderStageCreateInfo const & state
		, bool isRtot )const
	{
		auto result = get( m_device )->getContext()->glCreateShader( convertShaderStageFlag( state.stage ) );
		auto context = get( m_device )->getContext();

		if ( get( get( m_device )->getInstance() )->isSPIRVSupported() )
		{
			context->glShaderBinary( 1u
				, &result
				, GL_SHADER_BINARY_FORMAT_SPIR_V
				, m_code.data()
				, GLsizei( m_code.size() * sizeof( uint32_t ) ) );
			std::vector< GLuint > indices;
			std::vector< GLuint > values;
			getSpecializationInfo( state.pSpecializationInfo
				, indices
				, values );
			glLogCall( context
				, glSpecializeShader
				, result
				, state.pName
				, GLuint( indices.size() )
				, indices.data()
				, values.data() );
		}
		else
		{
			m_source = compileSpvToGlsl( m_device
				, m_code
				, state.stage
				, state
				, isRtot );
			auto length = int( m_source.size() );
			char const * data = m_source.data();
			glLogCall( context
				, glShaderSource
				, result
				, 1
				, &data
				, &length );
			glLogCall( context
				, glCompileShader
				, result );
		}

		int compiled = 0;
		glLogCall( context
			, glGetShaderiv
			, result
			, GL_INFO_COMPILE_STATUS
			, &compiled );

		if ( !doCheckCompileErrors( m_device, compiled != 0, result, m_source ) )
		{
			throw std::runtime_error{ "Shader compilation failed." };
		}

		return result;
	}
}
