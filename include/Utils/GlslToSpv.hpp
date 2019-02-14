/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "Utils/UtilsPrerequisites.hpp"

namespace utils
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
	UInt32Array compileGlslToSpv( ashes::Device const & device
		, ashes::ShaderStageFlag stage
		, std::string const & shader );
}
