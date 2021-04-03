/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/GlShaderModule.hpp"

#include "Core/GlDevice.hpp"
#include "Core/GlPhysicalDevice.hpp"
#include "Core/GlInstance.hpp"
#include "Miscellaneous/GlValidator.hpp"

#include <iostream>
#include <regex>

#if GlRenderer_USE_SPIRV_CROSS
#	include "spirv_cpp.hpp"
#	include "spirv_cross_util.hpp"
#	include "spirv_glsl.hpp"
#endif

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	//*************************************************************************

	namespace
	{
		static uint32_t constexpr OpCodeSPIRV = 0x07230203;

		struct BlockLocale
		{
			BlockLocale()
			{
				m_prvLoc = std::locale::global( std::locale{ "C" } );
			}

			~BlockLocale()
			{
				std::locale::global( m_prvLoc );
			}

		private:
			std::locale m_prvLoc;
		};
	}

	//*************************************************************************

	namespace gl3
	{
		std::string retrieveCompilerLog( ContextLock const & context
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

		bool checkCompileErrors( ContextLock const & context
			, VkShaderModule module
			, GLuint shaderName
			, std::string const & source )
		{
			int compiled = 0;
			glLogCall( context
				, glGetShaderiv
				, shaderName
				, GL_INFO_COMPILE_STATUS
				, &compiled );
			auto compilerLog = retrieveCompilerLog( context
				, shaderName );

			if ( !compiled || !compilerLog.empty() )
			{
				std::stringstream stream;

				if ( !compilerLog.empty() )
				{
					stream << compilerLog << std::endl;
				}

				stream << source << std::endl;
				report( ( compiled
						? reportWarning< VkShaderModule >
						: reportError< VkShaderModule > )
					, module
					, VK_ERROR_VALIDATION_FAILED_EXT
					, "Shader compilation"
					, stream.str() );
			}

			return compiled != 0;
		}

		void updateUboNames( spirv_cross::CompilerGLSL & compiler
			, std::string const & postfix )
		{
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();

			for ( auto & ubo : resources.uniform_buffers )
			{
				auto name = compiler.get_name( ubo.base_type_id );
				compiler.set_name( ubo.base_type_id, name + "_" + postfix );
			}

			for ( auto & ssbo : resources.storage_buffers )
			{
				auto name = compiler.get_name( ssbo.base_type_id );
				compiler.set_name( ssbo.base_type_id, name + "_" + postfix );
			}
		}
	}

	//*************************************************************************

	namespace common
	{
#if GlRenderer_USE_SPIRV_CROSS

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
			auto size = type.width;
			auto entrySize = uint32_t( entry.size / ( type.columns * type.vecsize ) );

			for ( auto col = 0u; col < type.columns; ++col )
			{
				for ( auto vec = 0u; vec < type.vecsize; ++vec )
				{
					std::memcpy( &constant.m.c[col].r[vec]
						, reinterpret_cast< uint8_t const * >( specialisationInfo.pData ) + offset
						, entrySize );
					offset += entrySize;
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
				throw std::runtime_error{ "Could not find an entry point with stage: "
					+ getShaderStageFlagsName( convert( stage ) ) };
			}

			compiler.set_entry_point( entryPoint, model );
		}

		void doSetupOptions( VkDevice device
			, spirv_cross::CompilerGLSL & compiler
			, bool invertY
			, bool isVertexShader )
		{
			auto options = compiler.get_common_options();
			options.version = get( getInstance( device ) )->getExtensions().getShaderVersion();
			options.es = false;
			options.separate_shader_objects = hasProgramPipelines( device );
			options.enable_420pack_extension = true;// has420PackExtensions( device );
			options.vertex.fixup_clipspace = isVertexShader;
			options.vertex.flip_vert_y = invertY && isVertexShader;
			options.vertex.support_nonzero_base_instance = get( getInstance( device ) )->getFeatures ().hasBaseInstance;
			compiler.set_common_options( options );
		}

		ConstantFormat getIntFormat( uint32_t size )
		{
			switch ( size )
			{
			case 1:
				return ConstantFormat::eInt;
			case 2:
				return ConstantFormat::eVec2i;
			case 3:
				return ConstantFormat::eVec3i;
			case 4:
				return ConstantFormat::eVec4i;
			default:
				assert( false && "Unsupported row count" );
				return ConstantFormat::eInt;
			}
		}

		ConstantFormat getUIntFormat( uint32_t size )
		{
			switch ( size )
			{
			case 1:
				return ConstantFormat::eUInt;
			case 2:
				return ConstantFormat::eVec2ui;
			case 3:
				return ConstantFormat::eVec3ui;
			case 4:
				return ConstantFormat::eVec4ui;
			default:
				assert( false && "Unsupported row count" );
				return ConstantFormat::eUInt;
			}
		}

		ConstantFormat getFloatFormat( uint32_t size )
		{
			switch ( size )
			{
			case 1:
				return ConstantFormat::eFloat;
			case 2:
				return ConstantFormat::eVec2f;
			case 3:
				return ConstantFormat::eVec3f;
			case 4:
				return ConstantFormat::eVec4f;
			default:
				assert( false && "Unsupported row count" );
				return ConstantFormat::eFloat;
			}
		}

		ConstantFormat getFloatFormat( uint32_t cols
			, uint32_t rows )
		{
			switch ( cols )
			{
			case 1:
				return getFloatFormat( rows );
			case 2:
				return ConstantFormat::eMat2f;
			case 3:
				return ConstantFormat::eMat3f;
			case 4:
				return ConstantFormat::eMat4f;
			default:
				assert( false && "Unsupported column count" );
				return ConstantFormat::eFloat;
			}
		}

		ConstantFormat getFormat( spirv_cross::SPIRType const & type )
		{
			switch ( type.basetype )
			{
			case spirv_cross::SPIRType::Int:
				return getIntFormat( type.vecsize );

			case spirv_cross::SPIRType::UInt:
				return getUIntFormat( type.vecsize );

			case spirv_cross::SPIRType::Float:
				return getFloatFormat( type.columns, type.vecsize );

			default:
				assert( false && "Unsupported SPIRType" );
				return ConstantFormat::eVec4f;
			}
		}

		uint32_t getArraySize( spirv_cross::CompilerGLSL & compiler
			, spirv_cross::SPIRType const & type )
		{
			return !type.array.empty()
				? ( type.array_size_literal[0]
					? type.array[0]
					: uint32_t( compiler.get_constant( type.array[0] ).scalar_u64() ) )
				: 0u;
		}

		ConstantsLayout doRetrievePushConstants( spirv_cross::CompilerGLSL & compiler
			, VkShaderStageFlagBits shaderStage )
		{
			ConstantsLayout result;
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();

			for ( auto & pcb : resources.push_constant_buffers )
			{
				auto & pcbType = compiler.get_type( pcb.type_id );
				assert( pcbType.parent_type );
				auto & structType = compiler.get_type( pcbType.parent_type );
				auto baseName = compiler.get_name( pcb.id );

				if ( baseName.empty() )
				{
					baseName = "_" + std::to_string( pcb.id );
				}

				uint32_t index = 0u;

				for ( auto & mbrTypeId : structType.member_types )
				{
					spirv_cross::SPIRType const & mbrType = compiler.get_type( mbrTypeId );
					uint32_t offset = compiler.get_member_decoration( structType.self, index, spv::Decoration::DecorationOffset );
					auto memberName = compiler.get_member_name( structType.self, index );

					if ( memberName.empty() )
					{
						memberName = "_m" + std::to_string( index );
					}

					auto name = baseName + "." + memberName;

					result.push_back( ConstantDesc
						{
							0u,
							shaderStage,
							name,
							~( 0u ),
							getFormat( mbrType ),
							getSize( getFormat( mbrType ) ),
							getArraySize( compiler, mbrType ),
							offset,
						} );

					if ( result.back().arraySize > 0 )
					{
						result.back().name += "[0]";
					}

					++index;
				}
			}

			return result;
		}

		void reportMissingBinding( VkDevice device
			, VkShaderModule module
			, std::string const & typeName
			, uint32_t binding
			, uint32_t set )
		{
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
			stream << typeName << ", binding=" << binding << ", set=" << set;
			reportError( module
				, VK_ERROR_VALIDATION_FAILED_EXT
				, "Missing binding"
				, stream.str() );
		}

		void doReworkBindings( VkDevice device
			, VkShaderModule module
			, std::string const & typeName
			, spirv_cross::CompilerGLSL & compiler
			, spirv_cross::SmallVector< spirv_cross::Resource > & resources
			, ShaderBindingMap const & bindings
			, bool failOnError
			, ShaderBindingMap const * fallback = nullptr )
		{
			for ( auto & obj : resources )
			{
				auto binding = compiler.get_decoration( obj.id, spv::DecorationBinding );
				auto set = compiler.get_decoration( obj.id, spv::DecorationDescriptorSet );
				compiler.unset_decoration( obj.id, spv::DecorationDescriptorSet );
				auto it = bindings.find( makeShaderBindingKey( set, binding ) );

				if ( it != bindings.end() )
				{
					compiler.set_decoration( obj.id, spv::DecorationBinding, it->second );
				}
				else if ( fallback )
				{
					it = fallback->find( makeShaderBindingKey( set, binding ) );

					if ( it != fallback->end() )
					{
						compiler.set_decoration( obj.id, spv::DecorationBinding, it->second );
					}
					else if ( failOnError )
					{
						reportMissingBinding( device, module, typeName, binding, set );
					}
				}
				else if ( failOnError )
				{
					reportMissingBinding( device, module, typeName, binding, set );
				}
			}
		}

		void doReworkBindings( VkPipelineLayout pipelineLayout
			, VkPipelineCreateFlags createFlags
			, VkShaderModule module
			, spirv_cross::CompilerGLSL & compiler
			, spirv_cross::ShaderResources & resources )
		{
			uint32_t const ssboMask = ( 1u << 16u );
			auto & bindings = get( pipelineLayout )->getShaderBindings();
			auto device = get( pipelineLayout )->getDevice();
			auto failOnError = !( checkFlag( createFlags, VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT )
				|| checkFlag( createFlags, VK_PIPELINE_CREATE_DERIVATIVE_BIT ) );
			doReworkBindings( device, module, "UniformBuffer", compiler, resources.uniform_buffers, bindings.ubo, failOnError );
			doReworkBindings( device, module, "StorageBuffer", compiler, resources.storage_buffers, bindings.sbo, failOnError );
			doReworkBindings( device, module, "CombinedSamplerImage", compiler, resources.sampled_images, bindings.tex, failOnError, &bindings.tbo );
			doReworkBindings( device, module, "SampledImage", compiler, resources.separate_images, bindings.tex, failOnError, &bindings.tbo );
			doReworkBindings( device, module, "Sampler", compiler, resources.separate_samplers, bindings.tex, failOnError );
			doReworkBindings( device, module, "StorageImage", compiler, resources.storage_images, bindings.img, failOnError, &bindings.ibo );
			doReworkBindings( device, module, "SubpassInput", compiler, resources.subpass_inputs, bindings.tex, failOnError );
		}

		std::string getStagePrefix( VkShaderStageFlagBits stage )
		{
			switch ( stage )
			{
			case VK_SHADER_STAGE_VERTEX_BIT:
				return "vtx";
			case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
				return "tessctl";
			case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
				return "tesseval";
			case VK_SHADER_STAGE_GEOMETRY_BIT:
				return "geom";
			case VK_SHADER_STAGE_FRAGMENT_BIT:
				return "frag";
			case VK_SHADER_STAGE_COMPUTE_BIT:
				return "comp";
			case VK_SHADER_STAGE_RAYGEN_BIT_NV:
				return "raygen";
			case VK_SHADER_STAGE_ANY_HIT_BIT_NV:
				return "hit";
			case VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV:
				return "closest";
			case VK_SHADER_STAGE_MISS_BIT_NV:
				return "miss";
			case VK_SHADER_STAGE_INTERSECTION_BIT_NV:
				return "inter";
			case VK_SHADER_STAGE_CALLABLE_BIT_NV:
				return "call";
			case VK_SHADER_STAGE_TASK_BIT_NV:
				return "task";
			case VK_SHADER_STAGE_MESH_BIT_NV:
				return "mesh";
			default:
				assert( false && "Unsupported VkShaderStageFlagBits" );
				return "unk";
			}
		}

		void doReworkAbsoluteInOut( VkShaderStageFlagBits stage
			, spirv_cross::CompilerGLSL & compiler
			, spirv_cross::ShaderResources & resources )
		{
			auto & options = compiler.get_common_options();

			if ( options.version < 400 )
			{
				if ( stage == VK_SHADER_STAGE_VERTEX_BIT )
				{
					auto inputPrefix = getStagePrefix( stage );

					for ( auto & input : resources.stage_inputs )
					{
						auto location = compiler.get_decoration( input.id, spv::DecorationLocation );
						auto name = inputPrefix + "Input" + std::to_string( location );
						compiler.set_name( input.id, name );
					}
				}
				else if ( stage == VK_SHADER_STAGE_FRAGMENT_BIT )
				{
					auto outputPrefix = getStagePrefix( stage );

					for ( auto & output : resources.stage_outputs )
					{
						auto location = compiler.get_decoration( output.id, spv::DecorationLocation );
						auto name = outputPrefix + "Output" + std::to_string( location );
						compiler.set_name( output.id, name );
					}
				}
			}
		}

		void doReworkIntermediateInOut( VkShaderStageFlagBits previousStage
			, VkShaderStageFlagBits currentStage
			, spirv_cross::CompilerGLSL & compiler
			, spirv_cross::ShaderResources & resources )
		{
			auto & options = compiler.get_common_options();

			if ( options.version < 400 )
			{
				if ( previousStage != currentStage )
				{
					auto inputPrefix = getStagePrefix( previousStage );

					for ( auto & input : resources.stage_inputs )
					{
						auto location = compiler.get_decoration( input.id, spv::DecorationLocation );
						auto name = inputPrefix + "Output" + std::to_string( location );
						compiler.set_name( input.id, name );
					}
				}

				if ( currentStage == VK_SHADER_STAGE_VERTEX_BIT
					|| currentStage == VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT
					|| currentStage == VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT
					|| currentStage == VK_SHADER_STAGE_GEOMETRY_BIT )
				{
					auto outputPrefix = getStagePrefix( currentStage );

					for ( auto & output : resources.stage_outputs )
					{
						auto location = compiler.get_decoration( output.id, spv::DecorationLocation );
						auto name = outputPrefix + "Output" + std::to_string( location );
						compiler.set_name( output.id, name );
					}
				}
			}
		}

		void doReworkFrontFace( bool invertY
			, std::string & shader )
		{
			if ( !invertY )
			{
				static std::string const glFrontFacing = "gl_FrontFacing";
				auto it = shader.find( glFrontFacing );

				while ( it != std::string::npos )
				{
					shader.insert( shader.begin() + it, '!' );
					it = shader.find( glFrontFacing, it + 2u );
				}
			}
		}

		template< typename CompileT >
		VkResult compileChecked( CompileT comp )
		{
#if _WIN32
			__try
			{
				comp();
				return VK_SUCCESS;
			}
			__except ( GetExceptionCode() == EXCEPTION_STACK_OVERFLOW )
			{
				return VK_ERROR_OUT_OF_HOST_MEMORY;
			}

#else

			comp();
			return VK_SUCCESS;

#endif
		}

#endif

		VkResult compileSpvToGlsl( VkDevice device
			, VkPipelineLayout pipelineLayout
			, VkPipelineCreateFlags createFlags
			, VkShaderModule module
			, UInt32Array const & shader
			, VkShaderStageFlagBits previousStage
			, VkShaderStageFlagBits currentStage
			, VkPipelineShaderStageCreateInfo const & state
			, bool invertY
			, ConstantsLayout & constants
			, bool & isGlsl
			, std::string & result )
		{
			if ( shader[0] == OpCodeSPIRV )
			{
				isGlsl = false;
#if GlRenderer_USE_SPIRV_CROSS
				BlockLocale guard;
				spirv_cross::CompilerGLSL compiler{ shader };
				spirv_cross::ShaderResources resources = compiler.get_shader_resources();
				doProcessSpecializationConstants( state, compiler );
				doSetEntryPoint( currentStage, compiler );
				doSetupOptions( device, compiler, invertY, currentStage == VK_SHADER_STAGE_VERTEX_BIT );
				constants = doRetrievePushConstants( compiler, currentStage );

				if ( !hasProgramPipelines( device ) )
				{
					gl3::updateUboNames( compiler, std::to_string( currentStage ) );
				}

				doReworkBindings( pipelineLayout, createFlags, module, compiler, resources );
				doReworkIntermediateInOut( previousStage, currentStage, compiler, resources );
				doReworkAbsoluteInOut( currentStage, compiler, resources );
				compiler.build_combined_image_samplers();
				auto vkres = compileChecked( [&compiler, &result]()
					{
						result = compiler.compile();
					} );
				doReworkFrontFace( invertY, result );
#else
				throw std::runtime_error{ "Can't parse SPIR-V shaders, pull submodule SpirvCross" };
#endif
				return vkres;
			}

			isGlsl = true;
			std::vector< char > glslCode( shader.size() * sizeof( uint32_t ) );
			std::memcpy( glslCode.data(), shader.data(), glslCode.size() );
			result = std::string( glslCode.data(), glslCode.data() + strlen( glslCode.data() ) );
			return VK_SUCCESS;
		}

		std::string retrieveLinkerLog( ContextLock const & context
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
	}

	//*************************************************************************

	bool checkLinkErrors( ContextLock const & context
		, VkPipeline pipeline
		, GLuint programName
		, int modulesCount
		, std::string const & from
		, std::string const & source )
	{
		int attached = 0;
		glLogCall( context
			, glGetProgramiv
			, programName
			, GL_INFO_ATTACHED_SHADERS
			, &attached );
		attached = modulesCount > 1
			? attached
			: modulesCount;
		int linked = 0;
		glLogCall( context
			, glGetProgramiv
			, programName
			, GL_INFO_LINK_STATUS
			, &linked );
		auto linkerLog = common::retrieveLinkerLog( context
			, programName );
		bool usable = linked
			&& modulesCount == attached
			&& linkerLog.find( "ERROR" ) == std::string::npos;
		int validated = 0;

		if ( usable )
		{
			glLogCall( context
				, glGetProgramiv
				, programName
				, GL_INFO_VALIDATE_STATUS
				, &validated );
		}

		if ( !usable
			|| !linkerLog.empty()
			|| !validated )
		{
			std::stringstream stream;

			if ( attached != modulesCount )
			{
				stream << "The linked shaders count doesn't match the active shaders count." << std::endl;
			}

			if ( validated )
			{
				stream << "The shader is not validated." << std::endl;
			}

			if ( !linkerLog.empty() )
			{
				stream << linkerLog << std::endl;
			}

			stream << source << std::endl;
			report( ( usable
					? reportWarning< VkPipeline >
					: reportError< VkPipeline > )
				, pipeline
				, VK_ERROR_VALIDATION_FAILED_EXT
				, from
				, stream.str() );
		}

		return usable;
	}

	//*************************************************************************

	ShaderModule::ShaderModule( VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, VkShaderModuleCreateInfo createInfo )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_code{ UInt32Array( createInfo.pCode, createInfo.pCode + ( createInfo.codeSize / sizeof( uint32_t ) ) ) }
	{
		registerObject( m_device, *this );
	}

	ShaderModule::~ShaderModule()
	{
		unregisterObject( m_device, *this );
	}

	VkResult ShaderModule::compile( VkPipeline pipeline
		, VkPipelineShaderStageCreateInfo const * previousState
		, VkPipelineShaderStageCreateInfo const & currentState
		, VkPipelineLayout pipelineLayout
		, VkPipelineCreateFlags createFlags
		, bool invertY
		, ShaderDesc & result )
	{
		auto context = get( m_device )->getContext();
		bool isGlsl;
		auto res = common::compileSpvToGlsl( m_device
			, pipelineLayout
			, createFlags
			, get( this )
			, m_code
			, ( previousState
				? previousState->stage
				: currentState.stage )
			, currentState.stage
			, currentState
			, invertY
			, m_constants
			, isGlsl
			, m_source );

		if ( res == VK_SUCCESS )
		{
			if ( !hasProgramPipelines( m_device ) )
			{
				result = compileCombined( context
					, currentState );
			}
			else
			{
				result = compileSeparate( context
					, pipeline
					, currentState
					, isGlsl );
			}
		}

		return res;
	}

	ShaderDesc ShaderModule::compileCombined( ContextLock const & context
		, VkPipelineShaderStageCreateInfo const & state )
	{
		auto shader = glLogNonVoidCall( context
			, glCreateShader
			, getShaderStage( state.stage ) );

		if ( m_source.find( "samplerCubeArray" ) != std::string::npos )
		{
			std::regex regex{ R"(#version[ ]*\d*)" };
			m_source = std::regex_replace( m_source.data()
				, regex
				, R"($&
#extension GL_ARB_texture_cube_map_array: enable
)" );
		}

		if ( m_source.find( "gl_ViewportIndex" ) != std::string::npos )
		{
			std::regex regex{ R"(#version[ ]*\d*)" };
			m_source = std::regex_replace( m_source.data()
				, regex
				, R"($&
#extension GL_ARB_viewport_array: enable
)" );
		}

		auto length = int( m_source.size() );
		char const * data = m_source.data();
		glLogCall( context
			, glShaderSource
			, shader
			, 1
			, &data
			, &length );
		glLogCall( context
			, glCompileShader
			, shader );
		ShaderDesc result{};

		if ( gl3::checkCompileErrors( context
			, get( this )
			, shader
			, m_source ) )
		{
			result.program = shader;
		}

		return result;
	}

	ShaderDesc ShaderModule::compileSeparate( ContextLock const & context
		, VkPipeline pipeline
		, VkPipelineShaderStageCreateInfo const & state
		, bool isGlsl )
	{
		char const * data = m_source.data();
		auto programObject = glLogNonVoidCall( context
			, glCreateShaderProgramv
			, getShaderStage( state.stage )
			, 1u
			, &data );
		bool usable = checkLinkErrors( context
			, pipeline
			, programObject
			, 1u
			, "Separate shader link"
			, m_source );
		ShaderDesc result{};

		if ( usable )
		{
			for ( auto & constant : m_constants )
			{
				constant.program = programObject;
			}

			auto result = getShaderDesc( context
				, m_constants
				, state.stage
				, programObject
				, true );
			result.program = programObject;
			result.isGlsl = isGlsl;
			result.stageFlags = state.stage;
			return result;
		}

		glLogCall( context
			, glDeleteProgram
			, programObject );

		return ShaderDesc{ isGlsl };
	}

	//*************************************************************************
}
