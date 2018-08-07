#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkSurfaceTransformFlagBitsKHR convert( renderer::SurfaceTransformFlag const & value )
	{
		switch ( value )
		{
		case renderer::SurfaceTransformFlag::eIdentity:
			return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;

		case renderer::SurfaceTransformFlag::eRotate90:
			return VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR;

		case renderer::SurfaceTransformFlag::eRotate180:
			return VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR;

		case renderer::SurfaceTransformFlag::eRotate270:
			return VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR;

		case renderer::SurfaceTransformFlag::eHorizontalMirror:
			return VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR;

		case renderer::SurfaceTransformFlag::eHorizontalMirrorRotate90:
			return VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR;

		case renderer::SurfaceTransformFlag::eHorizontalMirrorRotate180:
			return VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR;

		case renderer::SurfaceTransformFlag::eHorizontalMirrorRotate270:
			return VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR;

		case renderer::SurfaceTransformFlag::eInherit:
			return VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR;

		default:
			assert( false );
			return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
	}

	renderer::SurfaceTransformFlag convert( VkSurfaceTransformFlagBitsKHR const & value )
	{
		switch ( value )
		{
		case VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR:
			return renderer::SurfaceTransformFlag::eIdentity;

		case VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR:
			return renderer::SurfaceTransformFlag::eRotate90;

		case VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR:
			return renderer::SurfaceTransformFlag::eRotate180;

		case VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR:
			return renderer::SurfaceTransformFlag::eRotate270;

		case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR:
			return renderer::SurfaceTransformFlag::eHorizontalMirror;

		case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR:
			return renderer::SurfaceTransformFlag::eHorizontalMirrorRotate90;

		case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR:
			return renderer::SurfaceTransformFlag::eHorizontalMirrorRotate180;

		case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR:
			return renderer::SurfaceTransformFlag::eHorizontalMirrorRotate270;

		case VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR:
			return renderer::SurfaceTransformFlag::eInherit;

		default:
			assert( false );
			return renderer::SurfaceTransformFlag::eIdentity;
		}
	}

	VkSurfaceTransformFlagsKHR convert( renderer::SurfaceTransformFlags const & flags )
	{
		VkAccessFlags result{ 0 };

		if ( checkFlag( flags, renderer::SurfaceTransformFlag::eIdentity ) )
		{
			result |= VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}

		if ( checkFlag( flags, renderer::SurfaceTransformFlag::eRotate90 ) )
		{
			result |= VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR;
		}

		if ( checkFlag( flags, renderer::SurfaceTransformFlag::eRotate180 ) )
		{
			result |= VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR;
		}

		if ( checkFlag( flags, renderer::SurfaceTransformFlag::eRotate270 ) )
		{
			result |= VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR;
		}

		if ( checkFlag( flags, renderer::SurfaceTransformFlag::eHorizontalMirror ) )
		{
			result |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR;
		}

		if ( checkFlag( flags, renderer::SurfaceTransformFlag::eHorizontalMirrorRotate90 ) )
		{
			result |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR;
		}

		if ( checkFlag( flags, renderer::SurfaceTransformFlag::eHorizontalMirrorRotate180 ) )
		{
			result |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR;
		}

		if ( checkFlag( flags, renderer::SurfaceTransformFlag::eHorizontalMirrorRotate270 ) )
		{
			result |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR;
		}

		if ( checkFlag( flags, renderer::SurfaceTransformFlag::eInherit ) )
		{
			result |= VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR;
		}

		return result;
	}

	renderer::SurfaceTransformFlags convertSurfaceTransform( VkSurfaceTransformFlagsKHR const & flags )
	{
		renderer::SurfaceTransformFlags result;

		if ( renderer::checkFlag( flags, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR ) )
		{
			result |= renderer::SurfaceTransformFlag::eIdentity;
		}

		if ( renderer::checkFlag( flags, VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR ) )
		{
			result |= renderer::SurfaceTransformFlag::eRotate90;
		}

		if ( renderer::checkFlag( flags, VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR ) )
		{
			result |= renderer::SurfaceTransformFlag::eRotate180;
		}

		if ( renderer::checkFlag( flags, VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR ) )
		{
			result |= renderer::SurfaceTransformFlag::eRotate270;
		}

		if ( renderer::checkFlag( flags, VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR ) )
		{
			result |= renderer::SurfaceTransformFlag::eHorizontalMirror;
		}

		if ( renderer::checkFlag( flags, VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR ) )
		{
			result |= renderer::SurfaceTransformFlag::eHorizontalMirrorRotate90;
		}

		if ( renderer::checkFlag( flags, VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR ) )
		{
			result |= renderer::SurfaceTransformFlag::eHorizontalMirrorRotate180;
		}

		if ( renderer::checkFlag( flags, VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR ) )
		{
			result |= renderer::SurfaceTransformFlag::eHorizontalMirrorRotate270;
		}

		if ( renderer::checkFlag( flags, VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR ) )
		{
			result |= renderer::SurfaceTransformFlag::eInherit;
		}

		return result;
	}
}
