/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlQueryResultFlag
	{
		GL_QUERY_NONE = 0,
		GL_QUERY_RESULT = 0x8866,
		GL_QUERY_RESULT_NO_WAIT = 0x9194,
	};
	Ashes_ImplementFlag( GlQueryResultFlag )
	std::string getName( GlQueryResultFlags value );
	inline std::string toString( GlQueryResultFlags value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkQueryResultFlags en GLuint.
	*\param[in] flags
	*	Le VkQueryResultFlags.
	*\return
	*	Le GLuint.
	*/
	GlQueryResultFlags convertQueryResultFlags( VkQueryResultFlags const & flags );
}
