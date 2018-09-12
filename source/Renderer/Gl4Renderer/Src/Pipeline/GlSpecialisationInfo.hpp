/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___GlRenderer_SpecialisationInfo_HPP___
#define ___GlRenderer_SpecialisationInfo_HPP___
#pragma once

#include <Pipeline/SpecialisationInfo.hpp>

namespace renderer
{
	renderer::PushConstantArray convert( renderer::ShaderStageFlag stage
		, std::vector< renderer::SpecialisationMapEntry > const & entries );
	renderer::PushConstantsBufferPtr convert( renderer::Device const & device
		, uint32_t location
		, renderer::ShaderStageFlag stage
		, renderer::SpecialisationInfoBase const & specialisationInfo );
}

#endif
