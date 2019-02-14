/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Enum/QueryResultFlag.hpp>

namespace gl_renderer
{
	enum GlQueryResultFlag
	{
		GL_QUERY_RESULT = 0x8866,
		GL_QUERY_RESULT_NO_WAIT = 0x9194,
	};
	Ashes_ImplementFlag( GlQueryResultFlag )
	std::string getName( GlQueryResultFlags value );
	/**
	*\brief
	*	Convertit un ashes::QueryResultFlags en GLuint.
	*\param[in] flags
	*	Le ashes::QueryResultFlags.
	*\return
	*	Le GLuint.
	*/
	GLuint convert( ashes::QueryResultFlags const & flags );
}
