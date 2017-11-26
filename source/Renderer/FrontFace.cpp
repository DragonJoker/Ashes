#include "FrontFace.hpp"

namespace renderer
{
	VkFrontFace convert( FrontFace const & value )
	{
		switch ( value )
		{
		case FrontFace::eCounterClockwise:
			return VK_FRONT_FACE_COUNTER_CLOCKWISE;

		case FrontFace::eClockwise:
			return VK_FRONT_FACE_CLOCKWISE;

		default:
			assert( false && "Unsupported front face mode");
			return VK_FRONT_FACE_COUNTER_CLOCKWISE;
		}
	}
}
