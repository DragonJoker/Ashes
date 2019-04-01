/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Shader/ShaderModule.hpp"

#include "AshesPP/Core/Device.hpp"

namespace ashespp
{
	ShaderModule::ShaderModule( Device const & device
		, VkShaderStageFlagBits stage )
		: m_device{ device }
		, m_stage{ stage }
	{
	}

	ShaderModule::~ShaderModule()
	{
		unregisterObject( m_device, this );

		if ( m_internal != VK_NULL_HANDLE )
		{
			m_device.vkDestroyShaderModule( m_device, m_internal, nullptr );
		}
	}

	void ShaderModule::loadShader( UInt32Array const & shader )
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
			, &m_internal );
		checkError( res, "ShaderModule creation" );
		registerObject( m_device, "ShaderModule", this );
	}
}
