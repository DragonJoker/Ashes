/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"
#include "D3D11Renderer/Shader/D3D11ShaderDesc.hpp"

#include <Ashes/Shader/ShaderModule.hpp>

#include <d3d11shader.h>

namespace d3d11_renderer
{
	class CompiledShaderModule
	{
	public:
		CompiledShaderModule( CompiledShaderModule const & ) = delete;
		CompiledShaderModule & operator=( CompiledShaderModule const & ) = delete;
		CompiledShaderModule( CompiledShaderModule && rhs );
		CompiledShaderModule & operator=( CompiledShaderModule && rhs );

		CompiledShaderModule( Device const & device
			, ashes::UInt32Array const & spv
			, ashes::ShaderStageState const & state );
		~CompiledShaderModule();

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

		inline ashes::ShaderStageFlag getStage()const
		{
			return m_stage;
		}

		inline ShaderDesc const & getLayout()const
		{
			return m_layout;
		}

	private:
		void doRetrieveShader( Device const & device );
		ShaderDesc doRetrieveShaderDesc();
		InputLayout doRetrieveInputLayout( ID3D11ShaderReflection * reflection
			, UINT inputParameters );
		InterfaceBlockLayout doRetrieveInterfaceBlockLayout( ID3D11ShaderReflection * reflection
			, UINT constantBuffers );

	private:
		union
		{
			ID3D11ComputeShader * compute;
			ID3D11VertexShader * vertex;
			ID3D11GeometryShader * geometry;
			ID3D11HullShader * hull;
			ID3D11DomainShader * domain;
			ID3D11PixelShader * pixel;
		} m_shader;
		ashes::ShaderStageFlag m_stage;
		std::string m_source;
		ID3DBlob * m_compiled{ nullptr };
		ShaderDesc m_layout;
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
		: public ashes::ShaderModule
	{
	public:
		ShaderModule( Device const & device
			, ashes::ShaderStageFlag stage );
		~ShaderModule();
		/**
		*\~copydoc	ashes::ShaderModule::loadShader
		*/
		void loadShader( ashes::UInt32Array const & shader )override;

		CompiledShaderModule compile( ashes::ShaderStageState const & state );

	private:
		Device const & m_device;
		ashes::UInt32Array m_spv;
	};
}
