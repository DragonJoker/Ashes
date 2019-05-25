/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/D3D11ShaderModule.hpp"

#include "Core/D3D11Device.hpp"

#if D3D11Renderer_USE_SPIRV_CROSS
#	include "spirv_cpp.hpp"
#	include "spirv_cross_util.hpp"
#	include "spirv_hlsl.hpp"
#endif

#include <iostream>
#include <locale>
#include <regex>

#define CreateEvent CreateEventA
#include <atlbase.h>
#undef CreateEvent

#include <D3DCompiler.h>

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	//*************************************************************************

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

			for ( auto & e : compiler.get_entry_points_and_stages() )
			{
				if ( entryPoint.empty() && e.execution_model == model )
				{
					entryPoint = e.name;
				}
			}

			if ( entryPoint.empty() )
			{
				throw std::runtime_error{ "Could not find an entry point." };
			}

			compiler.set_entry_point( entryPoint, model );
		}

		void doSetupOptions( VkDevice device
			, spirv_cross::CompilerHLSL & compiler )
		{
			auto options = compiler.get_common_options();
			options.es = false;
			options.separate_shader_objects = true;
			options.enable_420pack_extension = true;
			options.vertex.fixup_clipspace = false;
			options.vertex.flip_vert_y = true;
			options.vertex.support_nonzero_base_instance = true;
			compiler.set_common_options( options );
		}

		void doSetupHlslOptions( VkDevice device
			, spirv_cross::CompilerHLSL & compiler )
		{
			auto hlslOptions = compiler.get_hlsl_options();
			hlslOptions.shader_model = 50;
			compiler.set_hlsl_options( hlslOptions );
		}

#endif

		std::string compileSpvToHlsl( VkDevice device
			, UInt32Array const & shader
			, VkShaderStageFlagBits stage
			, VkPipelineShaderStageCreateInfo const & state )
		{
			if ( shader[0] == OpCodeSPIRV )
			{
#if D3D11Renderer_USE_SPIRV_CROSS

				BlockLocale guard;
				auto compiler = std::make_unique< spirv_cross::CompilerHLSL >( shader );
				doProcessSpecializationConstants( state, *compiler );
				doSetEntryPoint( stage, *compiler );
				doSetupOptions( device, *compiler );
				doSetupHlslOptions( device, *compiler );
				return compiler->compile();

#else

				throw std::runtime_error{ "Can't parse SPIR-V shaders, pull submodule SpirvCross" };

#endif
			}

			std::vector< char > hlslCode( shader.size() * sizeof( uint32_t ) );
			std::memcpy( hlslCode.data(), shader.data(), hlslCode.size() );
			return std::string( hlslCode.data(), hlslCode.data() + strlen( hlslCode.data() ) );
		}

		uint32_t extractLocation( InputElementDesc & desc )
		{
			uint32_t result{ 0u };

			if ( desc.InputSlotClass == D3D11_INPUT_PER_INSTANCE_DATA )
			{
				auto name = desc.SemanticName;
				name = name.substr( 0u, name.size() - 1u );
				auto index = name.find_last_not_of( "0123456789" );
				name = name.substr( index + 1 );
				result = std::stoi( name );
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

	CompiledShaderModule::CompiledShaderModule( CompiledShaderModule && rhs )
		: m_shader{ std::move( rhs.m_shader ) }
		, m_stage{ std::move( rhs.m_stage ) }
		, m_source{ std::move( rhs.m_source ) }
		, m_compiled{ std::move( rhs.m_compiled ) }
		, m_layout{ std::move( rhs.m_layout ) }
	{
		m_layout.module = this;
		rhs.m_shader.compute = nullptr;
		rhs.m_compiled = nullptr;
		rhs.m_layout.module = nullptr;
	}

	CompiledShaderModule & CompiledShaderModule::operator=( CompiledShaderModule && rhs )
	{
		if ( this != &rhs )
		{
			m_shader = std::move( rhs.m_shader );
			m_stage = std::move( rhs.m_stage );
			m_source = std::move( rhs.m_source );
			m_compiled = std::move( rhs.m_compiled );
			m_layout = std::move( rhs.m_layout );
			m_layout.module = this;
			rhs.m_shader.compute = nullptr;
			rhs.m_compiled = nullptr;
			rhs.m_layout.module = nullptr;
		}

		return *this;
	}

	CompiledShaderModule::CompiledShaderModule( VkDevice device
		, VkShaderModule module
		, UInt32Array const & spv
		, VkPipelineShaderStageCreateInfo const & state )
		: m_shader{ nullptr }
		, m_stage{ state.stage }
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

		m_source = compileSpvToHlsl( device
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
				get( get( device )->getInstance() )->onReportMessage( VK_DEBUG_REPORT_WARNING_BIT_EXT
					, VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT
					, uint64_t( module )
					, 0u
					, 0u
					, "CORE"
					, stream.str().c_str() );
			}
		}
		else if ( errors )
		{
			std::stringstream stream;
			stream << reinterpret_cast< char * >( errors->GetBufferPointer() ) << std::endl;
			stream << m_source << std::endl;
			get( get( device )->getInstance() )->onReportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT
				, uint64_t( module )
				, 0u
				, 0u
				, "CORE"
				, stream.str().c_str() );
		}
	}

	CompiledShaderModule::~CompiledShaderModule()
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
		}
	}

	void CompiledShaderModule::doRetrieveShader( VkDevice device )
	{
		auto dxDevice = get( device )->getDevice();
		HRESULT hr;

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
		, UINT inputParameters )
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
		, UINT constantBuffers )
	{
		InterfaceBlockLayout result;

		for ( UINT i = 0; i < constantBuffers; i++ )
		{
			auto constantBuffer = reflection->GetConstantBufferByIndex( i );
			D3D11_SHADER_BUFFER_DESC sbDesc{};
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

	ShaderModule::~ShaderModule()
	{
	}

	CompiledShaderModule ShaderModule::compile( VkPipelineShaderStageCreateInfo const & state )const
	{
		return CompiledShaderModule{ m_device, get( this ), m_code, state };
	}

	//*************************************************************************
}
