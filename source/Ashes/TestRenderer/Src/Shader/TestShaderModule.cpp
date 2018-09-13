/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/TestShaderModule.hpp"

#include "Core/TestDevice.hpp"

#include <locale>
#include <regex>

namespace test_renderer
{
	ShaderModule::ShaderModule( Device const & device
		, ashes::ShaderStageFlag stage )
		: ashes::ShaderModule{ device, stage }
		, m_device{ device }
	{
	}

	ShaderModule::~ShaderModule()
	{
	}

	void ShaderModule::loadShader( std::string const & shader )
	{
	}

	void ShaderModule::loadShader( ashes::UInt32Array const & shader )
	{
	}

	void ShaderModule::loadShader( ashes::ByteArray const & shader )
	{
	}
}
