/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___GlRenderer_SpecialisationInfo_HPP___
#define ___GlRenderer_SpecialisationInfo_HPP___
#pragma once

#include <Ashes/Pipeline/SpecialisationInfo.hpp>

namespace ashes
{
	ashes::PushConstantArray convert( ashes::ShaderStageFlag stage
		, std::vector< ashes::SpecialisationMapEntry > const & entries );
	ashes::PushConstantsBufferPtr convert( ashes::Device const & device
		, uint32_t location
		, ashes::ShaderStageFlag stage
		, ashes::SpecialisationInfoBase const & specialisationInfo );
}

#endif
