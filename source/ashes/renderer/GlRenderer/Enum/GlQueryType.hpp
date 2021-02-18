/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <vector>

namespace ashes::gl
{
	enum GlQueryType
		: GLenum
	{
		GL_QUERY_TYPE_SAMPLES_PASSED = 0x8914,
		GL_QUERY_TYPE_PRIMITIVES_GENERATED = 0x8C87,
		GL_QUERY_TYPE_TIMESTAMP = 0x8E28,
		GL_QUERY_TYPE_VERTICES_SUBMITTED = 0x82EE,
		GL_QUERY_TYPE_PRIMITIVES_SUBMITTED = 0x82EF,
		GL_QUERY_TYPE_VERTEX_SHADER_INVOCATIONS = 0x82F0,
		GL_QUERY_TYPE_TESS_CONTROL_SHADER_PATCHES = 0x82F1,
		GL_QUERY_TYPE_TESS_EVALUATION_SHADER_INVOCATIONS = 0x82F2,
		GL_QUERY_TYPE_GEOMETRY_SHADER_INVOCATIONS = 0x887F,
		GL_QUERY_TYPE_GEOMETRY_SHADER_PRIMITIVES_EMITTED = 0x82F3,
		GL_QUERY_TYPE_FRAGMENT_SHADER_INVOCATIONS = 0x82F4,
		GL_QUERY_TYPE_COMPUTE_SHADER_INVOCATIONS = 0x82F5,
		GL_QUERY_TYPE_CLIPPING_INPUT_PRIMITIVES = 0x82F6,
		GL_QUERY_TYPE_CLIPPING_OUTPUT_PRIMITIVES = 0x82F7,
	};
	std::string getName( GlQueryType value );
	inline std::string toString( GlQueryType value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkQueryType en GlQueryType.
	*\param[in] value
	*	Le VkQueryType.
	*\return
	*	Le GlQueryType.
	*/
	GlQueryType convert( VkQueryType const & value );
	std::vector< GlQueryType > getQueryTypes( VkQueryPipelineStatisticFlags value );
}
