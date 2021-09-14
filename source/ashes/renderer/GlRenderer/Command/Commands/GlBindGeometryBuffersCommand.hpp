/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void buildBindGeometryBuffersCommand( GeometryBuffers const & vao
		, CmdList & list );
}
