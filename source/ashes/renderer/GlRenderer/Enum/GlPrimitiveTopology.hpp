/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::gl
{
	enum GlPrimitiveTopology
		: GLenum
	{
		GL_PRIMITIVE_TOPOLOGY_POINT_LIST = 0x0000,
		GL_PRIMITIVE_TOPOLOGY_LINE_LIST = 0x0001,
		GL_PRIMITIVE_TOPOLOGY_LINE_LOOP = 0x0002,
		GL_PRIMITIVE_TOPOLOGY_LINE_STRIP = 0x0003,
		GL_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST = 0x0004,
		GL_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP = 0x0005,
		GL_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN = 0x0006,
		GL_PRIMITIVE_TOPOLOGY_LINE_LIST_ADJACENCY = 0x000A,
		GL_PRIMITIVE_TOPOLOGY_LINE_STRIP_ADJACENCY = 0x000B,
		GL_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_ADJACENCY = 0x000C,
		GL_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_ADJACENCY = 0x000D,
		GL_PRIMITIVE_TOPOLOGY_PATCH_LIST = 0x000E,
	};
	std::string getName( GlPrimitiveTopology value );
	inline std::string toString( GlPrimitiveTopology value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkPrimitiveTopology en GlPrimitiveTopolgy.
	*\param[in] format
	*	Le VkPrimitiveTopology.
	*\return
	*	Le GlPrimitiveTopolgy.
	*/
	GlPrimitiveTopology convert( VkPrimitiveTopology const & topology );
}
