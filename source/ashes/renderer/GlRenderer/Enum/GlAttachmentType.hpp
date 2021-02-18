/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlAttachmentType
		: GLenum
	{
		GL_ATTACHMENT_TYPE_COLOR = 0x1800,
		GL_ATTACHMENT_TYPE_DEPTH = 0x1801,
		GL_ATTACHMENT_TYPE_STENCIL = 0x1802,
		GL_ATTACHMENT_TYPE_DEPTH_STENCIL = 0x84F9,
	};
	std::string getName( GlAttachmentType value );
	inline std::string toString( GlAttachmentType value ) { return getName( value ); }
}
