/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/GlRendererPrerequisites.hpp"
#include <string>

namespace gl_renderer
{
	std::string getErrorName( uint32_t code, uint32_t category );
	bool glCheckError( std::string const & text );
}
