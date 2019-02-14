/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Shader/ShaderModule.hpp"

#include "Ashes/Core/Device.hpp"

namespace ashes
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
