/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "ShaderProgram.hpp"

#include "RenderingResources.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace renderer
{
	ShaderProgram::ShaderProgram( RenderingResources const & resources )
		: m_program{ resources.getDevice().createShaderProgram() }
	{
	}

	void ShaderProgram::createModule( ByteArray const & fileData
		, ShaderStageFlag stage )
	{
		m_program->createModule( fileData, convert( stage ) );
	}
}
