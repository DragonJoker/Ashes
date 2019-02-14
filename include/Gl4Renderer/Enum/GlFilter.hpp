/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/Enum/Filter.hpp>

namespace gl_renderer
{
	enum GlFilter
		: GLenum
	{
		GL_FILTER_NEAREST = 0x2600,
		GL_FILTER_LINEAR = 0x2601,
		GL_FILTER_NEAREST_MIPMAP_NEAREST = 0x2700,
		GL_FILTER_LINEAR_MIPMAP_NEAREST = 0x2701,
		GL_FILTER_NEAREST_MIPMAP_LINEAR = 0x2702,
		GL_FILTER_LINEAR_MIPMAP_LINEAR = 0x2703,
	};
	std::string getName( GlFilter value );
	/**
	*\brief
	*	Convertit un ashes::Filter en GlFilter.
	*\param[in] filter
	*	Le ashes::Filter.
	*\return
	*	Le GlFilter.
	*/
	GlFilter convert( ashes::Filter const & filter );
	/**
	*\brief
	*	Convertit un ashes::Filter en GlFilter, en tenant compte d'un mode de mipmapping.
	*\param[in] filter
	*	Le ashes::Filter.
	*\param[in] mode
	*	Le mode de mipmapping.
	*\return
	*	Le GlFilter.
	*/
	GlFilter convert( ashes::Filter const & filter
		, ashes::MipmapMode mode
		, float minLod
		, float maxLod );
}
