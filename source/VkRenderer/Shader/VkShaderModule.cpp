/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/VkShaderModule.hpp"

#include "Core/VkDevice.hpp"

#include <locale>
#include <regex>

namespace vk_renderer
{
	ShaderModule::ShaderModule( Device const & device
		, ashes::ShaderStageFlag stage )
		: ashes::ShaderModule{ device, stage }
		, m_device{ device }
		, m_stage{ convert( stage ) }
	{
	}

	ShaderModule::~ShaderModule()
	{
		if ( m_shader != VK_NULL_HANDLE )
		{
			m_device.vkDestroyShaderModule( m_device, m_shader, nullptr );
		}
	}

	void ShaderModule::loadShader( ashes::UInt32Array const & shader )
	{
		VkShaderModuleCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			nullptr,
			0,
			uint32_t( shader.size() * sizeof( uint32_t ) ),
			shader.data()
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateShaderModule( m_device
			, &createInfo
			, nullptr
			, &m_shader );
		checkError( res, "ShaderModule creation" );
	}
}
