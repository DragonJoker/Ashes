/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Enum/QueryType.hpp>

namespace gl_renderer
{
	enum GlQueryType
		: GLenum
	{
		GL_QUERY_TYPE_SAMPLES_PASSED = 0x8914,
		GL_QUERY_TYPE_PRIMITIVES_GENERATED = 0x8C87,
		GL_QUERY_TYPE_TIMESTAMP = 0x8E28,
	};
	std::string getName( GlQueryType value );
	/**
	*\brief
	*	Convertit un ashes::QueryType en GlQueryType.
	*\param[in] value
	*	Le ashes::QueryType.
	*\return
	*	Le GlQueryType.
	*/
	GlQueryType convert( ashes::QueryType const & value );
}
