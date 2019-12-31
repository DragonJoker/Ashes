/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::gl
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
	*	Convertit un VkDebugReportObjectTypeEXT en GlDebugReportObjectType.
	*\param[in] value
	*	Le VkDebugReportObjectTypeEXT.
	*\return
	*	Le GlDebugReportObjectType.
	*/
	GlDebugReportObjectType convert( VkDebugReportObjectTypeEXT const & value );
	/**
	*\brief
	*	Convertit un VkObjectType en GlDebugReportObjectType.
	*\param[in] value
	*	Le VkObjectType.
	*\return
	*	Le GlDebugReportObjectType.
	*/
	GlDebugReportObjectType convert( VkObjectType const & value );
}
