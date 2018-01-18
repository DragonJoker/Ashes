#include "RendererPrerequisites.hpp"

namespace renderer
{
	bool isDepthStencilFormat( PixelFormat format )
	{
		return format == PixelFormat::eD24S8;
	}

	bool isStencilFormat( PixelFormat format )
	{
		return format == PixelFormat::eS8;
	}

	bool isDepthFormat( PixelFormat format )
	{
		return format == PixelFormat::eD16;
	}
}
