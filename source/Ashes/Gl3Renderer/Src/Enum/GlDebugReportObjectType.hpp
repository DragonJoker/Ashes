/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Enum/DebugReportObjectType.hpp>

namespace gl_renderer
{
	enum class GlDebugReportObjectType
		: GLenum
	{
		eUnknown = 0,
		eBuffer = 0x82E0,
		eShaderModule = 0x82E1,
		eQuery = 0x82E3,
		eSampler = 0x82E6,
		eTexture = 0x1702,
		eFrameBuffer = 0x8D40,
	};
	/**
	*\brief
	*	Convertit un ashes::DebugReportObjectType en GlDebugReportObjectType.
	*\param[in] value
	*	Le ashes::DebugReportObjectType.
	*\return
	*	Le GlDebugReportObjectType.
	*/
	GlDebugReportObjectType convert( ashes::DebugReportObjectType const & value );
}
