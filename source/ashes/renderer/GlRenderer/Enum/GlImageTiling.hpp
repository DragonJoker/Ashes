/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlImageTiling
	{
		GL_OPTIMAL_TILING_EXT = 0x9584,
		GL_LINEAR_TILING_EXT = 0x9585,
	};
	std::string getName( GlImageTiling value );
	inline std::string toString( GlImageTiling value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkImageTiling en GlImageTiling.
	*\param[in] value
	*	Le VkImageTiling.
	*\return
	*	Le GlImageTiling.
	*/
	GlImageTiling convert( VkImageTiling const & value );
}
