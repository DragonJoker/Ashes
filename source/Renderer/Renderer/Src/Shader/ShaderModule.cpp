/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Shader/ShaderModule.hpp"

#include "Core/Device.hpp"

namespace renderer
{
	ShaderModule::ShaderModule( Device const & device
		, ShaderStageFlag stage )
		: m_device{ device }
		, m_stage{ stage }
	{
		registerObject( m_device, "ShaderModule", this );
	}

	ShaderModule::~ShaderModule()
	{
		unregisterObject( m_device, this );
	}
}
