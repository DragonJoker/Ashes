/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Pipeline/ShaderStageState.hpp"

namespace renderer
{
	ShaderStageState::ShaderStageState( ShaderModulePtr && module
		, SpecialisationInfoBasePtr && specialisationInfo
		, std::string entryPoint )
		: m_module{ std::move( module ) }
		, m_specialisationInfo{ std::move( specialisationInfo ) }
		, m_entryPoint{ entryPoint }
	{
	}
}
