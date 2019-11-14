/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl3
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
	*	Convertit un VkQueryType en GlQueryType.
	*\param[in] value
	*	Le VkQueryType.
	*\return
	*	Le GlQueryType.
	*/
	GlQueryType convert( VkQueryType const & value );
}
