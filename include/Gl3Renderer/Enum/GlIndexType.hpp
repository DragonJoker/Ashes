/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/Enum/IndexType.hpp>

namespace gl_renderer
{
	enum GlIndexType
		: GLenum
	{
		GL_INDEX_TYPE_UINT16 = 0x1403,
		GL_INDEX_TYPE_UINT32 = 0x1405,
	};
	std::string getName( GlIndexType value );
	/**
	*\brief
	*	Convertit un ashes::IndexType en GlIndexType.
	*\param[in] type
	*	Le ashes::IndexType.
	*\return
	*	Le GlIndexType.
	*/
	GlIndexType convert( ashes::IndexType const & type );
}
