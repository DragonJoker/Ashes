/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Pipeline/SpecialisationInfo.hpp>

namespace d3d11_renderer
{
	ashes::PushConstantArray convert( ashes::ShaderStageFlag stage
		, std::vector< ashes::SpecialisationMapEntry > const & entries );
	ashes::PushConstantsBufferPtr convert( ashes::Device const & device
		, uint32_t location
		, ashes::ShaderStageFlag stage
		, ashes::SpecialisationInfoBase const & specialisationInfo );
}
