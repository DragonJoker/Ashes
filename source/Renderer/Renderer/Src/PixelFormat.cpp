#include "RendererPrerequisites.hpp"

namespace renderer
{
	bool isDepthStencilFormat( PixelFormat format )
	{
		return format == PixelFormat::eD24S8
			|| format == PixelFormat::eD32FS8;
	}

	bool isStencilFormat( PixelFormat format )
	{
		return format == PixelFormat::eS8;
	}

	bool isDepthFormat( PixelFormat format )
	{
		return format == PixelFormat::eD16
			|| format == PixelFormat::eD32F;
	}

	ImageAspectFlags getAspectMask( renderer::PixelFormat format )
	{
		return isDepthStencilFormat( format )
			? renderer::ImageAspectFlags( renderer::ImageAspectFlag::eDepth | renderer::ImageAspectFlag::eStencil )
			: isDepthFormat( format )
				? renderer::ImageAspectFlags( renderer::ImageAspectFlag::eDepth )
				: isStencilFormat( format )
					? renderer::ImageAspectFlags( renderer::ImageAspectFlag::eStencil )
					: renderer::ImageAspectFlags( renderer::ImageAspectFlag::eColour );
	}

	std::string getName( renderer::PixelFormat value )
	{
		switch ( value )
		{
		case renderer::PixelFormat::eR8:
			return "R8";
		case renderer::PixelFormat::eR32F:
			return "R32F";
		case renderer::PixelFormat::eR8G8:
			return "R8G8";
		case renderer::PixelFormat::eRG32F:
			return "RG32F";
		case renderer::PixelFormat::eR8G8B8:
			return "R8G8B8";
		case renderer::PixelFormat::eRGB32F:
			return "RGB32F";
		case renderer::PixelFormat::eR8G8B8A8:
			return "R8G8B8A8";
		case renderer::PixelFormat::eB8G8R8A8:
			return "B8G8R8A8";
		case renderer::PixelFormat::eRGBA32F:
			return "RGBA32F";
		case renderer::PixelFormat::eD16:
			return "D16";
		case renderer::PixelFormat::eD24S8:
			return "D24S8";
		case renderer::PixelFormat::eD32F:
			return "D32F";
		case renderer::PixelFormat::eD32FS8:
			return "D32FS8";
		case renderer::PixelFormat::eS8:
			return "S8";

		default:
			assert( false && "Unsupported PixelFormat." );
			throw std::runtime_error{ "Unsupported PixelFormat" };
		}

		return 0;
	}
}
