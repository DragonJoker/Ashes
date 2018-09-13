/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/D3D11ShaderModule.hpp"

#include "Core/D3D11Device.hpp"

#include <Pipeline/ShaderStageState.hpp>

#include <locale>
#include <regex>

#include <D3DCompiler.h>

namespace d3d11_renderer
{
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

	void ShaderModule::loadShader( std::string const & shader )
	{
		m_source = shader;

		if ( m_source.find( "ashesScalePosition" ) != std::string::npos )
		{
			m_source = R"(float4 ashesScalePosition(float4 pos)
{
	float4x4 scale;
	scale[0] = float4( 1.0, 0.0, 0.0, 0.0 );
	scale[1] = float4( 0.0, -1.0, 0.0, 0.0 );
	scale[2] = float4( 0.0, 0.0, 1.0, 0.0 );
	scale[3] = float4( 0.0, 0.0, 0.0, 1.0 );
	return mul( scale, pos );
}

)" + m_source;
		}
	}

	void ShaderModule::loadShader( ashes::UInt32Array const & shader )
	{
	}

	void ShaderModule::loadShader( ashes::ByteArray const & shader )
	{
	}

	void ShaderModule::compile( ashes::ShaderStageState const & state )
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

		std::string profile = Profiles[getStage()];

		ID3DBlob * errors = NULL;
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

		if ( hr == S_OK )
		{
			doRetrieveShader();
		}
		else if ( errors )
		{
			ashes::Logger::logError( reinterpret_cast< char * >( errors->GetBufferPointer() ) );
			ashes::Logger::logError( m_source );
		}

		safeRelease( errors );
	}

	void ShaderModule::doCreateInputLayout()
	{
		// Reflect shader info
		ID3D11ShaderReflection * vertexShaderReflection{ nullptr };
		auto hr = D3DReflect( m_compiled->GetBufferPointer()
			, m_compiled->GetBufferSize()
			, IID_ID3D11ShaderReflection
			, reinterpret_cast< void ** >( &vertexShaderReflection ) );

		if ( dxCheckError( hr, "D3DReflect" ) )
		{
			// Get shader info
			D3D11_SHADER_DESC shaderDesc;
			vertexShaderReflection->GetDesc( &shaderDesc );

			for ( uint32_t i = 0; i < shaderDesc.InputParameters; i++ )
			{
				D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
				vertexShaderReflection->GetInputParameterDesc( i, &paramDesc );

				// fill out input element desc
				D3D11InputElementDesc elementDesc;
				elementDesc.SemanticName = paramDesc.SemanticName;
				elementDesc.SemanticIndex = paramDesc.SemanticIndex;
				elementDesc.InputSlot = 0;
				elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
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
				m_inputLayout.push_back( elementDesc );
			}
		}

		safeRelease( vertexShaderReflection );
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
			doCreateInputLayout();
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
		}

		dxCheckError( hr, "RetrieveShader" );
	}
}
