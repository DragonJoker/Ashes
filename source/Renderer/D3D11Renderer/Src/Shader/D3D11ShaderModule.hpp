/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Shader/ShaderModule.hpp>

#include <d3d11shader.h>

namespace d3d11_renderer
{
	struct D3D11InputElementDesc
	{
		std::string SemanticName;
		UINT SemanticIndex;
		DXGI_FORMAT Format;
		UINT InputSlot;
		UINT AlignedByteOffset;
		D3D11_INPUT_CLASSIFICATION InputSlotClass;
		UINT InstanceDataStepRate;
	};
	/**
	*\~french
	*\brief
	*	Classe encapsulant un TestShaderModule.
	*\~english
	*\brief
	*	TestShaderModule wrapper.
	*/
	class ShaderModule
		: public renderer::ShaderModule
	{
	public:
		ShaderModule( Device const & device
			, renderer::ShaderStageFlag stage );
		~ShaderModule();
		/**
		*\~copydoc	renderer::ShaderModule::loadShader
		*/
		void loadShader( std::string const & shader )override;
		/**
		*\~copydoc	renderer::ShaderModule::loadShader
		*/
		void loadShader( renderer::ByteArray const & shader )override;
		/**
		*\~copydoc	renderer::ShaderModule::loadShader
		*/
		void loadShader( renderer::UInt32Array const & shader );

		void compile( renderer::ShaderStageState const & state );

		inline ID3D11ComputeShader * getCSShader()const
		{
			return m_shader.compute;
		}

		inline ID3D11VertexShader * getVSShader()const
		{
			return m_shader.vertex;
		}

		inline ID3D11GeometryShader * getGSShader()const
		{
			return m_shader.geometry;
		}

		inline ID3D11HullShader * getHSShader()const
		{
			return m_shader.hull;
		}

		inline ID3D11DomainShader * getDSShader()const
		{
			return m_shader.domain;
		}

		inline ID3D11PixelShader * getPSShader()const
		{
			return m_shader.pixel;
		}

		inline ID3DBlob * getCompiled()const
		{
			return m_compiled;
		}

		inline std::vector< D3D11InputElementDesc > const & getShaderInputLayout()const
		{
			return m_inputLayout;
		}

	private:
		void doRetrieveShader();
		void doCreateInputLayout();

	private:
		Device const & m_device;
		union
		{
			ID3D11ComputeShader * compute;
			ID3D11VertexShader * vertex;
			ID3D11GeometryShader * geometry;
			ID3D11HullShader * hull;
			ID3D11DomainShader * domain;
			ID3D11PixelShader * pixel;
		} m_shader;
		std::string m_source;
		ID3DBlob * m_compiled{ nullptr };
		std::vector< D3D11InputElementDesc > m_inputLayout;
	};
}
