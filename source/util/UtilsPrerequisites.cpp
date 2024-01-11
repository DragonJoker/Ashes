/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "util/UtilsPrerequisites.hpp"

#include "util/Vec2.hpp"
#include "util/Vec3.hpp"
#include "util/Vec4.hpp"

#include <cassert>

namespace utils
{
	VkExtent2D makeExtent2D( UIVec2 const & value )
	{
		return { value[0], value[1] };
	}

	VkExtent3D makeExtent3D( UIVec2 const & value )
	{
		return { value[0], value[1], 1u };
	}

	VkExtent3D makeExtent3D( UIVec3 const & value )
	{
		return { value[0], value[1], value[2] };
	}

	VkOffset2D makeOffset2D( IVec2 const & value )
	{
		return { value[0], value[1] };
	}

	VkOffset3D makeOffset3D( IVec3 const & value )
	{
		return { value[0], value[1], value[2] };
	}

	VkClearColorValue makeClearColorValue( RgbColour const & value )
	{
		VkClearColorValue result{};
		result.float32[0] = value.r;
		result.float32[1] = value.g;
		result.float32[2] = value.b;
		result.float32[3] = 1.0f;
		return result;
	}

	VkClearColorValue makeClearColorValue( RgbaColour const & value )
	{
		VkClearColorValue result{};
		result.float32[0] = value.r;
		result.float32[1] = value.g;
		result.float32[2] = value.b;
		result.float32[3] = value.a;
		return result;
	}
}
