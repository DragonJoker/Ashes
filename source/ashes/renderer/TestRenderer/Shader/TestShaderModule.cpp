/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/TestShaderModule.hpp"

namespace ashes::test
{
	ShaderModule::ShaderModule( VkDevice device
		, VkShaderModuleCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
		, m_code{ UInt32Array( m_createInfo.pCode, m_createInfo.pCode + ( m_createInfo.codeSize / sizeof( uint32_t ) ) ) }
	{
	}
}
