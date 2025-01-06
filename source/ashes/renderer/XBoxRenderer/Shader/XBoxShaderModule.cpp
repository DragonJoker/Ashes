/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/XBoxShaderModule.hpp"

#include "Core/XBoxDevice.hpp"

#if D3D11Renderer_USE_SPIRV_CROSS
#	include "spirv_cpp.hpp"
#	include "spirv_cross_util.hpp"
#	include "spirv_hlsl.hpp"
#endif

#include "ashesxbox_api.hpp"


#pragma warning( push )
#pragma warning( disable: 4191 )
#pragma warning( disable: 4365 )
#pragma warning( disable: 5204 )
#pragma warning( disable: 5262 )
#if defined( _UNICODE )
#	define CreateEvent CreateEventW
#else
#	define CreateEvent CreateEventA
#endif
#include <atlbase.h>
#undef CreateEvent

#include <iostream>
#include <locale>
#include <regex>
#include <sstream>
#pragma warning( pop )

#include <D3DCompiler.h>

namespace ashes::xbox
{
	//*************************************************************************

	namespace
	{
		inline uint32_t constexpr OpCodeSPIRV = 0x07230203;

		struct BlockLocale
			: public NonCopyable
		{
			BlockLocale()
			{
				m_prvLoc = std::locale( "" );

				if ( m_prvLoc.name() != "C" )
				{
					std::locale::global( std::locale{ "C" } );
				}
			}

			~BlockLocale()noexcept
			{
				try
				{
					if ( m_prvLoc.name() != "C" )
					{
						std::locale::global( m_prvLoc );
					}
				}
				catch ( ... )
				{
					// Nothing to do here
				}
			}

		private:
			std::locale m_prvLoc;
		};

#if D3D11Renderer_USE_SPIRV_CROSS

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
			auto entrySize = uint32_t( entry.size / ( size_t( type.columns ) * type.vecsize ) );

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
			, spirv_cross::CompilerHLSL & compiler )
		{
			if ( state.pSpecializationInfo )
			{
				auto constants = compiler.get_specialization_constants();

				for ( auto & spec : makeArrayView( state.pSpecializationInfo->pMapEntries, state.pSpecializationInfo->mapEntryCount ) )
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
						doFillConstant( *state.pSpecializationInfo
							, spec
							, type
							, constant );
					}
				}
			}
		}

		void doSetEntryPoint( VkShaderStageFlagBits stage
			, spirv_cross::CompilerHLSL & compiler )
		{
			auto model = getExecutionModel( stage );
			std::string entryPoint;

			for ( auto const & e : compiler.get_entry_points_and_stages() )
			{
				if ( entryPoint.empty() && e.execution_model == model )
				{
					entryPoint = e.name;
				}
			}

			if ( entryPoint.empty() )
			{
				throw ashes::BaseException{ "Could not find an entry point." };
			}

			compiler.set_entry_point( entryPoint, model );
		}

		void doSetupOptions( spirv_cross::CompilerHLSL & compiler )
		{
			spirv_cross::CompilerGLSL::Options options = compiler.get_common_options();
			options.es = false;
			options.separate_shader_objects = true;
			options.enable_420pack_extension = true;
			options.vertex.fixup_clipspace = false;
			options.vertex.flip_vert_y = true;
			options.vertex.support_nonzero_base_instance = true;
			compiler.set_common_options( options );
		}

		void doSetupHlslOptions( spirv_cross::CompilerHLSL & compiler )
		{
			spirv_cross::CompilerHLSL::Options hlslOptions = compiler.get_hlsl_options();
			hlslOptions.shader_model = 50;
			hlslOptions.point_coord_compat = true;
			hlslOptions.point_size_compat = true;
			compiler.set_hlsl_options( hlslOptions );
		}

		void reportMissingBinding( VkShaderModule shaderModule
			, std::string const & typeName
			, uint32_t binding
			, uint32_t set )
		{
			assert( false );
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
			stream << typeName << ", binding=" << binding << ", set=" << set;
			reportError( shaderModule
				, VK_ERROR_VALIDATION_FAILED_EXT
				, "Missing binding"
				, stream.str() );
		}

		void doReworkBindings( VkShaderModule shaderModule
			, std::string const & typeName
			, spirv_cross::CompilerGLSL & compiler
			, spirv_cross::SmallVector< spirv_cross::Resource > const & resources
			, ShaderBindingMap const & bindings
			, bool failOnError
			, ShaderBindingMap const * fallback = nullptr )
		{
			for ( auto const & obj : resources )
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
						reportMissingBinding( shaderModule, typeName, binding, set );
					}
				}
				else if ( failOnError )
				{
					reportMissingBinding( shaderModule, typeName, binding, set );
				}
			}
		}

		void doReworkUavBindings( VkShaderModule shaderModule
			, std::string const & typeName
			, spirv_cross::CompilerGLSL & compiler
			, spirv_cross::SmallVector< spirv_cross::Resource > const & resources
			, ShaderBindingMap const & bindings
			, uint32_t uavStart
			, bool failOnError
			, ShaderBindingMap const * fallback = nullptr )
		{
			for ( auto const & obj : resources )
			{
				auto binding = compiler.get_decoration( obj.id, spv::DecorationBinding );
				auto set = compiler.get_decoration( obj.id, spv::DecorationDescriptorSet );
				compiler.unset_decoration( obj.id, spv::DecorationDescriptorSet );
				auto it = bindings.find( makeShaderBindingKey( set, binding ) );

				if ( it != bindings.end() )
				{
					// UAV and fragment outputs share the same namespace,
					// hence we add UAV start offset (which is just the outputs count).
					compiler.set_decoration( obj.id, spv::DecorationBinding, uavStart + it->second );
				}
				else if ( fallback )
				{
					it = fallback->find( makeShaderBindingKey( set, binding ) );

					if ( it != fallback->end() )
					{
						// Fallback is used only for storage texel buffers,
						// which are handled the same way as uniform texel buffers,
						// hence we don't add the UAV start offset.
						compiler.set_decoration( obj.id, spv::DecorationBinding, it->second );
					}
					else if ( failOnError )
					{
						reportMissingBinding( shaderModule, typeName, binding, set );
					}
				}
				else if ( failOnError )
				{
					reportMissingBinding( shaderModule, typeName, binding, set );
				}
			}
		}

		void doReworkOutputs( spirv_cross::CompilerGLSL const & compiler
			, spirv_cross::SmallVector< spirv_cross::Resource > const & resources
			, uint32_t & index )
		{
			for ( auto const & obj : resources )
			{
				auto location = compiler.get_decoration( obj.id, spv::DecorationLocation );
				index = std::max( index, location );
			}
		}

		void doReworkBindings( VkPipelineLayout pipelineLayout
			, VkPipelineCreateFlags createFlags
			, VkShaderModule shaderModule
			, VkShaderStageFlagBits stage
			, spirv_cross::CompilerGLSL & compiler )
		{
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();
			auto & bindings = get( pipelineLayout )->getShaderBindings();
			auto failOnError = !( checkFlag( createFlags, VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT )
				|| checkFlag( createFlags, VK_PIPELINE_CREATE_DERIVATIVE_BIT ) );
			doReworkBindings( shaderModule, "UniformBuffer", compiler, resources.uniform_buffers, bindings.ubo, failOnError );
			doReworkBindings( shaderModule, "CombinedSamplerImage", compiler, resources.sampled_images, bindings.tex, failOnError, &bindings.tbo );
			doReworkBindings( shaderModule, "SampledImage", compiler, resources.separate_images, bindings.tex, failOnError );
			doReworkBindings( shaderModule, "Sampler", compiler, resources.separate_samplers, bindings.tex, failOnError );
			doReworkBindings( shaderModule, "SubpassInput", compiler, resources.subpass_inputs, bindings.tex, failOnError );

			uint32_t uav{};

			if ( stage == VK_SHADER_STAGE_FRAGMENT_BIT )
			{
				// Tightly pack outputs indices.
				doReworkOutputs( compiler, resources.stage_outputs, uav );
			}

			doReworkUavBindings( shaderModule, "StorageBuffer", compiler, resources.storage_buffers, bindings.uav, uav, failOnError );
			doReworkUavBindings( shaderModule, "StorageImage", compiler, resources.storage_images, bindings.uav, uav, failOnError, &bindings.ibo );
		}

#endif

		std::string compileSpvToHlsl( VkPipelineLayout pipelineLayout
			, VkPipelineCreateFlags createFlags
			, VkShaderModule shaderModule
			, UInt32Array const & shader
			, VkShaderStageFlagBits stage
			, VkPipelineShaderStageCreateInfo const & state )
		{
			if ( shader[0] == OpCodeSPIRV )
			{
#if D3D11Renderer_USE_SPIRV_CROSS

				try
				{
					BlockLocale guard;
					spirv_cross::CompilerHLSL compiler{ shader };
					doProcessSpecializationConstants( state, compiler );
					doSetEntryPoint( stage, compiler );
					doSetupOptions( compiler );
					doSetupHlslOptions( compiler );
					doReworkBindings( pipelineLayout, createFlags, shaderModule, stage, compiler );
					return compiler.compile();
				}
				catch ( std::exception & exc )
				{
					std::cerr << "SPIR-V to HLSL compilation failed: " << exc.what() << std::endl;
					throw;
				}

#else

				throw ashes::BaseException{ "Can't parse SPIR-V shaders, pull submodule SpirvCross" };

#endif
			}

			std::vector< char > hlslCode( shader.size() * sizeof( uint32_t ) );
			std::memcpy( hlslCode.data(), shader.data(), hlslCode.size() );
			return std::string( hlslCode.data(), hlslCode.data() + strnlen( hlslCode.data(), hlslCode.size() ) );
		}

		uint32_t extractLocation( InputElementDesc const & desc )
		{
			uint32_t result{ 0u };

			if ( desc.InputSlotClass == D3D11_INPUT_PER_INSTANCE_DATA )
			{
				std::string name = desc.SemanticName;
				name = name.substr( 0u, name.size() - 1u );
				auto index = name.find_last_not_of( "0123456789" );
				name = name.substr( index + 1 );
				result = uint32_t( std::stoi( name ) );
			}

			return result;
		}

		void normalise( InputElementDesc & desc )
		{
			if ( desc.InputSlotClass == D3D11_INPUT_PER_INSTANCE_DATA )
			{
				desc.BaseSemanticIndex = desc.SemanticIndex + extractLocation( desc );
			}
		}
	}

	//*************************************************************************

	CompiledShaderModule::CompiledShaderModule( CompiledShaderModule && rhs )noexcept
		: m_shader{ std::move( rhs.m_shader ) }
		, m_stage{ std::move( rhs.m_stage ) }
		, m_source{ std::move( rhs.m_source ) }
		, m_compiled{ std::move( rhs.m_compiled ) }
		, m_layout{ std::move( rhs.m_layout ) }
	{
		m_layout.shaderModule = this;
		rhs.m_shader.compute = nullptr;
		rhs.m_compiled = nullptr;
		rhs.m_layout.shaderModule = nullptr;
	}

	CompiledShaderModule & CompiledShaderModule::operator=( CompiledShaderModule && rhs )noexcept
	{
		if ( this != &rhs )
		{
			m_shader = std::move( rhs.m_shader );
			m_stage = std::move( rhs.m_stage );
			m_source = std::move( rhs.m_source );
			m_compiled = std::move( rhs.m_compiled );
			m_layout = std::move( rhs.m_layout );
			m_layout.shaderModule = this;
			rhs.m_shader.compute = nullptr;
			rhs.m_compiled = nullptr;
			rhs.m_layout.shaderModule = nullptr;
		}

		return *this;
	}

	CompiledShaderModule::CompiledShaderModule( VkDevice device
		, VkPipelineLayout pipelineLayout
		, VkPipelineCreateFlags createFlags
		, VkShaderModule shaderModule
		, UInt32Array const & spv
		, VkPipelineShaderStageCreateInfo const & state )
		: m_stage{ state.stage }
	{
		static std::map< D3D_FEATURE_LEVEL, std::map< VkShaderStageFlagBits, std::string > > Profiles
		{
			{ 
				D3D_FEATURE_LEVEL_11_1,
				{
					{ VK_SHADER_STAGE_VERTEX_BIT, "vs_5_0" },
					{ VK_SHADER_STAGE_GEOMETRY_BIT, "gs_5_0" },
					{ VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, "hs_5_0" },
					{ VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, "ds_5_0" },
					{ VK_SHADER_STAGE_FRAGMENT_BIT, "ps_5_0" },
					{ VK_SHADER_STAGE_COMPUTE_BIT, "cs_5_0" },
				}
			},
			{ 
				D3D_FEATURE_LEVEL_11_0,
				{
					{ VK_SHADER_STAGE_VERTEX_BIT, "vs_5_0" },
					{ VK_SHADER_STAGE_GEOMETRY_BIT, "gs_5_0" },
					{ VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, "hs_5_0" },
					{ VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, "ds_5_0" },
					{ VK_SHADER_STAGE_FRAGMENT_BIT, "ps_5_0" },
					{ VK_SHADER_STAGE_COMPUTE_BIT, "cs_5_0" },
				}
			},
			{ 
				D3D_FEATURE_LEVEL_10_1,
				{
					{ VK_SHADER_STAGE_VERTEX_BIT, "vs_4_1" },
					{ VK_SHADER_STAGE_GEOMETRY_BIT, "gs_4_1" },
					{ VK_SHADER_STAGE_FRAGMENT_BIT, "ps_4_1" },
				}
			},
			{ 
				D3D_FEATURE_LEVEL_10_0,
				{
					{ VK_SHADER_STAGE_VERTEX_BIT, "vs_4_0" },
					{ VK_SHADER_STAGE_GEOMETRY_BIT, "gs_4_0" },
					{ VK_SHADER_STAGE_FRAGMENT_BIT, "ps_4_0" },
				}
			},
			{ 
				D3D_FEATURE_LEVEL_9_3,
				{
					{ VK_SHADER_STAGE_VERTEX_BIT, "vs_3_0" },
					{ VK_SHADER_STAGE_FRAGMENT_BIT, "ps_3_0" },
				}
			},
			{ 
				D3D_FEATURE_LEVEL_9_2,
				{
					{ VK_SHADER_STAGE_VERTEX_BIT, "vs_2_0" },
					{ VK_SHADER_STAGE_FRAGMENT_BIT, "ps_2_0" },
				}
			},
			{ 
				D3D_FEATURE_LEVEL_9_1,
				{
					{ VK_SHADER_STAGE_VERTEX_BIT, "vs_1_1" },
				}
			}
		};

		m_source = compileSpvToHlsl( pipelineLayout
			, createFlags
			, shaderModule
			, spv
			, m_stage
			, state );
		std::string profile = Profiles[get( device )->getFeatureLevel()][m_stage];
		assert( !profile.empty() && "Unsupported shader stage for currently supported feature level" );
		CComPtr< ID3DBlob > errors;
		UINT flags = 0;

#if !defined( NDEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif

		auto hr = D3DCompile( m_source.c_str()
			, UINT( m_source.size() )
			, nullptr
			, nullptr
			, nullptr
			, state.pName
			, profile.c_str()
			, flags
			, 0
			, &m_compiled
			, &errors );

		if ( hr == S_OK )
		{
			doRetrieveShader( device );
			m_layout = doRetrieveShaderDesc( device );

			if ( errors )
			{
				std::stringstream stream;
				stream << reinterpret_cast< char * >( errors->GetBufferPointer() ) << std::endl;
				stream << m_source << std::endl;
				reportWarning( shaderModule
					, VK_ERROR_VALIDATION_FAILED_EXT
					, "Shader compilation succeeded with messages"
					, stream.str() );
			}
		}
		else if ( errors )
		{
			std::stringstream stream;
			stream << reinterpret_cast< char * >( errors->GetBufferPointer() ) << std::endl;
			stream << m_source << std::endl;
			reportError( shaderModule
				, VK_ERROR_VALIDATION_FAILED_EXT
				, "Shader compilation failed"
				, stream.str() );
		}
	}

	CompiledShaderModule::~CompiledShaderModule()noexcept
	{
		safeRelease( m_compiled );

		switch ( m_stage )
		{
		case VK_SHADER_STAGE_VERTEX_BIT:
			safeRelease( m_shader.vertex );
			break;
		case VK_SHADER_STAGE_GEOMETRY_BIT:
			safeRelease( m_shader.geometry );
			break;
		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
			safeRelease( m_shader.hull );
			break;
		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
			safeRelease( m_shader.domain );
			break;
		case VK_SHADER_STAGE_FRAGMENT_BIT:
			safeRelease( m_shader.pixel );
			break;
		case VK_SHADER_STAGE_COMPUTE_BIT:
			safeRelease( m_shader.compute );
			break;
		default:
			break;
		}
	}

	void CompiledShaderModule::doRetrieveShader( VkDevice device )
	{
		auto dxDevice = get( device )->getDevice();
		HRESULT hr{};

		switch ( m_stage )
		{
		case VK_SHADER_STAGE_VERTEX_BIT:
			hr = dxDevice->CreateVertexShader( reinterpret_cast< DWORD * >( m_compiled->GetBufferPointer() )
				, m_compiled->GetBufferSize()
				, nullptr
				, &m_shader.vertex );
			dxDebugName( m_shader.vertex, VertexShader );
			break;

		case VK_SHADER_STAGE_GEOMETRY_BIT:
			hr = dxDevice->CreateGeometryShader( reinterpret_cast< DWORD * >( m_compiled->GetBufferPointer() )
				, m_compiled->GetBufferSize()
				, nullptr
				, &m_shader.geometry );
			dxDebugName( m_shader.geometry, GeometryShader );
			break;

		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
			hr = dxDevice->CreateHullShader( reinterpret_cast< DWORD * >( m_compiled->GetBufferPointer() )
				, m_compiled->GetBufferSize()
				, nullptr
				, &m_shader.hull );
			dxDebugName( m_shader.hull, HullShader );
			break;

		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
			hr = dxDevice->CreateDomainShader( reinterpret_cast< DWORD * >( m_compiled->GetBufferPointer() )
				, m_compiled->GetBufferSize()
				, nullptr
				, &m_shader.domain );
			dxDebugName( m_shader.domain, DomainShader );
			break;

		case VK_SHADER_STAGE_FRAGMENT_BIT:
			hr = dxDevice->CreatePixelShader( reinterpret_cast< DWORD * >( m_compiled->GetBufferPointer() )
				, m_compiled->GetBufferSize()
				, nullptr
				, &m_shader.pixel );
			dxDebugName( m_shader.pixel, PixelShader );
			break;

		case VK_SHADER_STAGE_COMPUTE_BIT:
			hr = dxDevice->CreateComputeShader( reinterpret_cast< DWORD * >( m_compiled->GetBufferPointer() )
				, m_compiled->GetBufferSize()
				, nullptr
				, &m_shader.compute );
			dxDebugName( m_shader.compute, ComputeShader );
			break;

		default:
			assert( false && "Unsupported shader stage" );
			break;
		}

		checkError( device, hr, "RetrieveShader" );
	}

	ShaderDesc CompiledShaderModule::doRetrieveShaderDesc( VkDevice device )
	{
		// Reflect shader info
		CComPtr< ID3D11ShaderReflection > shaderReflection;
		auto hr = D3DReflect( m_compiled->GetBufferPointer()
			, m_compiled->GetBufferSize()
			, __uuidof( ID3D11ShaderReflection )
			, reinterpret_cast< void ** >( &shaderReflection ) );
		checkError( device, hr, "D3DReflect" );

		// Get shader info
		D3D11_SHADER_DESC shaderDesc{};
		shaderReflection->GetDesc( &shaderDesc );
		ShaderDesc result{ this };
		result.inputLayout = doRetrieveInputLayout( shaderReflection, shaderDesc.InputParameters );
		result.interfaceBlockLayout = doRetrieveInterfaceBlockLayout( shaderReflection, shaderDesc.ConstantBuffers );
		return result;
	}

	InputLayout CompiledShaderModule::doRetrieveInputLayout( ID3D11ShaderReflection * reflection
		, UINT inputParameters )const
	{
		InputLayout result;

		for ( uint32_t i = 0; i < inputParameters; i++ )
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc{};
			reflection->GetInputParameterDesc( i, &paramDesc );

			// fill out input element desc
			InputElementDesc elementDesc{};
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.BaseSemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = paramDesc.Stream;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = elementDesc.SemanticName.back() == '_'
				? D3D11_INPUT_PER_INSTANCE_DATA
				: D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			// determine DXGI format
			if ( paramDesc.Mask == 1 )
			{
				if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if ( paramDesc.Mask <= 3 )
			{
				if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if ( paramDesc.Mask <= 7 )
			{
				if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if ( paramDesc.Mask <= 15 )
			{
				if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			//save element desc
			normalise( elementDesc );
			result.push_back( elementDesc );
		}

		return result;
	}

	InterfaceBlockLayout CompiledShaderModule::doRetrieveInterfaceBlockLayout( ID3D11ShaderReflection * reflection
		, UINT constantBuffers )const
	{
		InterfaceBlockLayout result;

		for ( UINT i = 0; i < constantBuffers; i++ )
		{
			auto constantBuffer = reflection->GetConstantBufferByIndex( i );
			D3D11_SHADER_BUFFER_DESC sbDesc{ "" };
			constantBuffer->GetDesc( &sbDesc );

			if ( sbDesc.Type == D3D_CT_CBUFFER )
			{
				// Look for cbuffers declared without registers (as they are then generated from push_constant blocks).
				auto it = m_source.find( std::string{ "cbuffer " } + sbDesc.Name + "\n" );

				if ( it != std::string::npos )
				{
					ConstantBufferDesc constantBufferDesc{};
					constantBufferDesc.name = sbDesc.Name;
					constantBufferDesc.size = sbDesc.Size;

					for ( UINT j = 0; j < sbDesc.Variables; ++j )
					{
						auto variable = constantBuffer->GetVariableByIndex( j );
						D3D11_SHADER_VARIABLE_DESC svDesc{};
						variable->GetDesc( &svDesc );
						ConstantDesc constantDesc{};
						constantDesc.name = svDesc.Name;
						constantDesc.offset = svDesc.StartOffset;
						constantDesc.size = svDesc.Size;
						constantBufferDesc.constants.push_back( constantDesc );
					}

					D3D11_SHADER_INPUT_BIND_DESC sibDesc{};
					reflection->GetResourceBindingDescByName( sbDesc.Name, &sibDesc );
					constantBufferDesc.binding = sibDesc.BindPoint;
					result.push_back( constantBufferDesc );
				}
			}
		}

		return result;
	}

	//*************************************************************************

	ShaderModule::ShaderModule( VkDevice device
		, VkShaderModuleCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
		, m_code{ UInt32Array( m_createInfo.pCode, m_createInfo.pCode + ( m_createInfo.codeSize / sizeof( uint32_t ) ) ) }
	{
		m_createInfo.pCode = m_code.data();
	}

	CompiledShaderModule ShaderModule::compile( VkPipelineShaderStageCreateInfo const & state
		, VkPipelineLayout pipelineLayout
		, VkPipelineCreateFlags createFlags )const
	{
		return CompiledShaderModule{ m_device, pipelineLayout, createFlags, get( this ), m_code, state };
	}

	//*************************************************************************
}
