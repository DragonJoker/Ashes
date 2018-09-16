/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "AshesPrerequisites.hpp"

namespace ashes
{
	void initialiseGlslang();
	void cleanupGlslang();
	/**
	*\~french
	*\brief
	*	Transpile un shader GLSL en SPIR-V.
	*\~english
	*\brief
	*	Transpiles a GLSL shader to SPIR-V.
	*/
	UInt32Array GlslToSpv( Device const & device
		, ShaderStageFlag stage
		, std::string const & shader );
}
