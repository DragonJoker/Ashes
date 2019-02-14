#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GLsizei convert( ashes::SampleCountFlag const & flags )
	{
		switch ( flags )
		{
		case ashes::SampleCountFlag::e1:
			return 1;

		case ashes::SampleCountFlag::e2:
			return 2;

		case ashes::SampleCountFlag::e4:
			return 4;

		case ashes::SampleCountFlag::e8:
			return 8;

		case ashes::SampleCountFlag::e16:
			return 16;

		case ashes::SampleCountFlag::e32:
			return 32;

		case ashes::SampleCountFlag::e64:
			return 64;

		default:
			assert( false && "Unsupported sample count flag." );
			return 1;
		}
	}
}
