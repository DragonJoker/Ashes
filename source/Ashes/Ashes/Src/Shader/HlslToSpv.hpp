/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~french
	*\brief
	*	Transpile un shader HLSL en SPIR-V.
	*\~english
	*\brief
	*	Transpiles an HLSL shader to SPIR-V.
	*/
	UInt32Array HlslToSpv( Device const & device
		, ShaderStageFlag stage
		, std::string const & shader );
}
