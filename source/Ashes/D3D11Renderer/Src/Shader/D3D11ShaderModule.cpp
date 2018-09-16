/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/D3D11ShaderModule.hpp"

#include "Core/D3D11Device.hpp"

#include "spirv_cpp.hpp"
#include "spirv_cross_util.hpp"
#include "spirv_hlsl.hpp"

#include <Pipeline/ShaderStageState.hpp>

#include <locale>
#include <regex>
#include <atlbase.h>

#include <D3DCompiler.h>

namespace d3d11_renderer
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
			switch ( type.columns )
			{
			case 1u:
				std::memcpy( &constant.m.c[0].r[0]
					, specialisationInfo.getData() + entry.offset
					, entry.size );
				break;
			case 2u:
				switch ( type.vecsize )
				{
				case 1u:
					break;
				case 2u:
					break;
				case 3u:
					break;
				case 4u:
					break;
				}
				break;
			case 3u:
				switch ( type.vecsize )
				{
				case 1u:
					break;
				case 2u:
					break;
				case 3u:
					break;
				case 4u:
					break;
				}
				break;
			case 4u:
				switch ( type.vecsize )
				{
				case 1u:
					break;
				case 2u:
					break;
				case 3u:
					break;
				case 4u:
					break;
				}
				break;
			}
		}

		void doProcessSpecializationConstants( ashes::ShaderStageState const & state
			, spirv_cross::CompilerHLSL & compiler )
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
				throw std::runtime_error{ "Could not find an entry point with stage: " + getName( stage ) };
			}

			compiler.set_entry_point( entryPoint, model );
		}

		void doSetupOptions( Device const & device
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

		void doSetupHlslOptions( Device const & device
			, spirv_cross::CompilerHLSL & compiler )
		{
			auto hlslOptions = compiler.get_hlsl_options();
			hlslOptions.shader_model = 50;
			compiler.set_hlsl_options( hlslOptions );
		}

		std::string compileSpvToHlsl( Device const & device
			, ashes::UInt32Array const & shader
			, ashes::ShaderStageFlag stage
			, ashes::ShaderStageState const & state )
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

			auto compiler = std::make_unique< spirv_cross::CompilerHLSL >( shader );
			doProcessSpecializationConstants( state, *compiler );
			doSetEntryPoint( stage, *compiler );
			doSetupOptions( device, *compiler );
			doSetupHlslOptions( device, *compiler );
			return compiler->compile();
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

	ShaderModule::ShaderModule( Device const & device
		, ashes::ShaderStageFlag stage )
		: ashes::ShaderModule{ device, stage }
		, m_device{ device }
		, m_shader{ nullptr }
	{
	}

	ShaderModule::~ShaderModule()
	{
		safeRelease( m_compiled );

		switch ( getStage() )
		{
		case ashes::ShaderStageFlag::eVertex:
			safeRelease( m_shader.vertex );
			break;

		case ashes::ShaderStageFlag::eGeometry:
			safeRelease( m_shader.geometry );
			break;

		case ashes::ShaderStageFlag::eTessellationControl:
			safeRelease( m_shader.hull );
			break;

		case ashes::ShaderStageFlag::eTessellationEvaluation:
			safeRelease( m_shader.domain );
			break;

		case ashes::ShaderStageFlag::eFragment:
			safeRelease( m_shader.pixel );
			break;

		case ashes::ShaderStageFlag::eCompute:
			safeRelease( m_shader.compute );
			break;
		}
	}

	void ShaderModule::loadShader( ashes::UInt32Array const & shader )
	{
		m_spv = shader;
	}

	ShaderDesc ShaderModule::compile( ashes::ShaderStageState const & state )
	{
		static std::map< ashes::ShaderStageFlag, std::string > Profiles
		{
			{ ashes::ShaderStageFlag::eVertex, "vs_5_0" },
			{ ashes::ShaderStageFlag::eGeometry, "gs_5_0" },
			{ ashes::ShaderStageFlag::eTessellationControl, "hs_5_0" },
			{ ashes::ShaderStageFlag::eTessellationEvaluation, "ds_5_0" },
			{ ashes::ShaderStageFlag::eFragment, "ps_5_0" },
			{ ashes::ShaderStageFlag::eCompute, "cs_5_0" },
		};

		m_source = compileSpvToHlsl( m_device
			, m_spv
			, m_stage
			, state );
		std::string profile = Profiles[getStage()];
		CComPtr< ID3DBlob > errors;
		UINT flags = 0;

#if !defined( NDEBUG )
		flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_WARNINGS_ARE_ERRORS;
#endif

		auto hr = D3DCompile( m_source.c_str()
			, UINT( m_source.size() )
			, nullptr
			, nullptr
			, nullptr
			, state.entryPoint.c_str()
			, profile.c_str()
			, flags
			, 0
			, &m_compiled
			, &errors );
		ShaderDesc result;

		if ( hr == S_OK )
		{
			doRetrieveShader();
			result = doRetrieveShaderDesc();
		}

		if ( errors )
		{
			ashes::Logger::logError( reinterpret_cast< char * >( errors->GetBufferPointer() ) );
			ashes::Logger::logError( m_source );
		}

		return result;
	}

	void ShaderModule::doRetrieveShader()
	{
		auto device = m_device.getDevice();
		HRESULT hr;

		switch ( getStage() )
		{
		case ashes::ShaderStageFlag::eVertex:
			hr = device->CreateVertexShader( reinterpret_cast< DWORD * >( m_compiled->GetBufferPointer() )
				, m_compiled->GetBufferSize()
				, nullptr
				, &m_shader.vertex );
			dxDebugName( m_shader.vertex, VertexShader );
			break;

		case ashes::ShaderStageFlag::eGeometry:
			hr = device->CreateGeometryShader( reinterpret_cast< DWORD * >( m_compiled->GetBufferPointer() )
				, m_compiled->GetBufferSize()
				, nullptr
				, &m_shader.geometry );
			dxDebugName( m_shader.geometry, GeometryShader );
			break;

		case ashes::ShaderStageFlag::eTessellationControl:
			hr = device->CreateHullShader( reinterpret_cast< DWORD * >( m_compiled->GetBufferPointer() )
				, m_compiled->GetBufferSize()
				, nullptr
				, &m_shader.hull );
			dxDebugName( m_shader.hull, HullShader );
			break;

		case ashes::ShaderStageFlag::eTessellationEvaluation:
			hr = device->CreateDomainShader( reinterpret_cast< DWORD * >( m_compiled->GetBufferPointer() )
				, m_compiled->GetBufferSize()
				, nullptr
				, &m_shader.domain );
			dxDebugName( m_shader.domain, DomainShader );
			break;

		case ashes::ShaderStageFlag::eFragment:
			hr = device->CreatePixelShader( reinterpret_cast< DWORD * >( m_compiled->GetBufferPointer() )
				, m_compiled->GetBufferSize()
				, nullptr
				, &m_shader.pixel );
			dxDebugName( m_shader.pixel, PixelShader );
			break;

		case ashes::ShaderStageFlag::eCompute:
			hr = device->CreateComputeShader( reinterpret_cast< DWORD * >( m_compiled->GetBufferPointer() )
				, m_compiled->GetBufferSize()
				, nullptr
				, &m_shader.compute );
			dxDebugName( m_shader.compute, ComputeShader );
			break;

		default:
			assert( false && "Unsupported shader stage" );
			break;
		}

		dxCheckError( hr, "RetrieveShader" );
	}

	ShaderDesc ShaderModule::doRetrieveShaderDesc()
	{
		// Reflect shader info
		CComPtr< ID3D11ShaderReflection > shaderReflection;
		auto hr = D3DReflect( m_compiled->GetBufferPointer()
			, m_compiled->GetBufferSize()
			, __uuidof( ID3D11ShaderReflection )
			, reinterpret_cast< void ** >( &shaderReflection ) );
		dxCheckError( hr, "D3DReflect" );

		// Get shader info
		D3D11_SHADER_DESC shaderDesc{};
		shaderReflection->GetDesc( &shaderDesc );
		ShaderDesc result{ this };
		result.inputLayout = doRetrieveInputLayout( shaderReflection, shaderDesc.InputParameters );
		result.interfaceBlockLayout = doRetrieveInterfaceBlockLayout( shaderReflection, shaderDesc.ConstantBuffers );
		return result;
	}

	InputLayout ShaderModule::doRetrieveInputLayout( ID3D11ShaderReflection * reflection
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

	InterfaceBlockLayout ShaderModule::doRetrieveInterfaceBlockLayout( ID3D11ShaderReflection * reflection
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
}
