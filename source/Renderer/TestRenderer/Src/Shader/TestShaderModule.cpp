/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Shader/TestShaderModule.hpp"

#include "Core/TestDevice.hpp"

#include <locale>
#include <regex>

namespace test_renderer
{
	ShaderModule::ShaderModule( Device const & device
		, renderer::ShaderStageFlag stage )
		: renderer::ShaderModule{ device, stage }
		, m_device{ device }
	{
	}

	ShaderModule::~ShaderModule()
	{
	}

	void ShaderModule::loadShader( std::string const & shader )
	{
	}

	void ShaderModule::loadShader( renderer::UInt32Array const & shader )
	{
	}

	void ShaderModule::loadShader( renderer::ByteArray const & shader )
	{
	}
}
