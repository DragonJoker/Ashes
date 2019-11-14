/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"
#include "renderer/D3D11Renderer/Shader/D3D11ShaderDesc.hpp"

#include <d3d11shader.h>

namespace ashes::d3d11
{
	class CompiledShaderModule
	{
	public:
		CompiledShaderModule( CompiledShaderModule const & ) = delete;
		CompiledShaderModule & operator=( CompiledShaderModule const & ) = delete;
		CompiledShaderModule( CompiledShaderModule && rhs );
		CompiledShaderModule & operator=( CompiledShaderModule && rhs );

		CompiledShaderModule( VkDevice device
			, VkPipelineLayout pipelineLayout
			, VkShaderModule module
			, UInt32Array const & spv
			, VkPipelineShaderStageCreateInfo const & state );
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

		inline VkShaderStageFlagBits getStage()const
		{
			return m_stage;
		}

		inline ShaderDesc const & getLayout()const
		{
			return m_layout;
		}

	private:
		void doRetrieveShader( VkDevice device );
		ShaderDesc doRetrieveShaderDesc( VkDevice device );
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
		VkShaderStageFlagBits m_stage;
		std::string m_source;
		ID3DBlob * m_compiled{ nullptr };
		ShaderDesc m_layout;
	};

	class ShaderModule
	{
	public:
		ShaderModule( VkDevice device
			, VkShaderModuleCreateInfo createInfo );
		~ShaderModule();

		CompiledShaderModule compile( VkPipelineShaderStageCreateInfo const & state
			, VkPipelineLayout pipelineLayout )const;

	private:
		VkDevice m_device;
		VkShaderModuleCreateInfo m_createInfo;
		UInt32Array m_code;
		mutable std::string m_source;
	};
}
