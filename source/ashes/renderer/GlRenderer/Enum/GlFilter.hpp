/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::gl
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
	inline std::string toString( GlFilter value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkFilter en GlFilter.
	*\param[in] filter
	*	Le VkFilter.
	*\return
	*	Le GlFilter.
	*/
	GlFilter convert( VkFilter const & filter );
	/**
	*\brief
	*	Convertit un VkFilter en GlFilter, en tenant compte d'un mode de mipmapping.
	*\param[in] filter
	*	Le VkFilter.
	*\param[in] mode
	*	Le mode de mipmapping.
	*\return
	*	Le GlFilter.
	*/
	GlFilter convert( VkFilter const & filter
		, VkSamplerMipmapMode mode
		, float minLod
		, float maxLod );
}
