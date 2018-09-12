/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#pragma once

#include <Pipeline/SpecialisationInfo.hpp>

namespace d3d11_renderer
{
	renderer::PushConstantArray convert( renderer::ShaderStageFlag stage
		, std::vector< renderer::SpecialisationMapEntry > const & entries );
	renderer::PushConstantsBufferPtr convert( renderer::Device const & device
		, uint32_t location
		, renderer::ShaderStageFlag stage
		, renderer::SpecialisationInfoBase const & specialisationInfo );
}
