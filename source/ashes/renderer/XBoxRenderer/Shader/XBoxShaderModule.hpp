/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/XBoxRenderer/XBoxRendererPrerequisites.hpp"
#include "renderer/XBoxRenderer/Shader/XBoxShaderDesc.hpp"

#include <d3d11shader.h>

namespace ashes::xbox
{
	class CompiledShaderModule
	{
	public:
		CompiledShaderModule( CompiledShaderModule const & ) = delete;
		CompiledShaderModule & operator=( CompiledShaderModule const & ) = delete;
		CompiledShaderModule( CompiledShaderModule && rhs )noexcept;
		CompiledShaderModule & operator=( CompiledShaderModule && rhs )noexcept;

		CompiledShaderModule( VkDevice device
			, VkPipelineLayout pipelineLayout
			, VkPipelineCreateFlags createFlags
			, VkShaderModule shaderModule
			, UInt32Array const & spv
			, VkPipelineShaderStageCreateInfo const & state );
		~CompiledShaderModule()noexcept;

		ID3D11ComputeShader * getCSShader()const
		{
			return m_shader.compute;
		}

		ID3D11VertexShader * getVSShader()const
		{
			return m_shader.vertex;
		}

		ID3D11GeometryShader * getGSShader()const
		{
			return m_shader.geometry;
		}

		ID3D11HullShader * getHSShader()const
		{
			return m_shader.hull;
		}

		ID3D11DomainShader * getDSShader()const
		{
			return m_shader.domain;
		}

		ID3D11PixelShader * getPSShader()const
		{
			return m_shader.pixel;
		}

		ID3DBlob * getCompiled()const
		{
			return m_compiled;
		}

		VkShaderStageFlagBits getStage()const
		{
			return m_stage;
		}

		ShaderDesc const & getLayout()const
		{
			return m_layout;
		}

	private:
		void doRetrieveShader( VkDevice device );
		ShaderDesc doRetrieveShaderDesc( VkDevice device );
		InputLayout doRetrieveInputLayout( ID3D11ShaderReflection * reflection
			, UINT inputParameters )const;
		InterfaceBlockLayout doRetrieveInterfaceBlockLayout( ID3D11ShaderReflection * reflection
			, UINT constantBuffers )const;

	private:
		union
		{
			ID3D11ComputeShader * compute;
			ID3D11VertexShader * vertex;
			ID3D11GeometryShader * geometry;
			ID3D11HullShader * hull;
			ID3D11DomainShader * domain;
			ID3D11PixelShader * pixel;
		} m_shader{};
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

		CompiledShaderModule compile( VkPipelineShaderStageCreateInfo const & state
			, VkPipelineLayout pipelineLayout
			, VkPipelineCreateFlags createFlags )const;

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkShaderModuleCreateInfo m_createInfo;
		UInt32Array m_code;
		mutable std::string m_source;
	};
}
