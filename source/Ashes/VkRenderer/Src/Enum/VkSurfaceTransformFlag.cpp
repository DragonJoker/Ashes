#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkSurfaceTransformFlagBitsKHR convert( ashes::SurfaceTransformFlag const & value )
	{
		switch ( value )
		{
		case ashes::SurfaceTransformFlag::eIdentity:
			return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;

		case ashes::SurfaceTransformFlag::eRotate90:
			return VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR;

		case ashes::SurfaceTransformFlag::eRotate180:
			return VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR;

		case ashes::SurfaceTransformFlag::eRotate270:
			return VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR;

		case ashes::SurfaceTransformFlag::eHorizontalMirror:
			return VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR;

		case ashes::SurfaceTransformFlag::eHorizontalMirrorRotate90:
			return VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR;

		case ashes::SurfaceTransformFlag::eHorizontalMirrorRotate180:
			return VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR;

		case ashes::SurfaceTransformFlag::eHorizontalMirrorRotate270:
			return VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR;

		case ashes::SurfaceTransformFlag::eInherit:
			return VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR;

		default:
			assert( false );
			return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
	}

	ashes::SurfaceTransformFlag convert( VkSurfaceTransformFlagBitsKHR const & value )
	{
		switch ( value )
		{
		case VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR:
			return ashes::SurfaceTransformFlag::eIdentity;

		case VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR:
			return ashes::SurfaceTransformFlag::eRotate90;

		case VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR:
			return ashes::SurfaceTransformFlag::eRotate180;

		case VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR:
			return ashes::SurfaceTransformFlag::eRotate270;

		case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR:
			return ashes::SurfaceTransformFlag::eHorizontalMirror;

		case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR:
			return ashes::SurfaceTransformFlag::eHorizontalMirrorRotate90;

		case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR:
			return ashes::SurfaceTransformFlag::eHorizontalMirrorRotate180;

		case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR:
			return ashes::SurfaceTransformFlag::eHorizontalMirrorRotate270;

		case VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR:
			return ashes::SurfaceTransformFlag::eInherit;

		default:
			assert( false );
			return ashes::SurfaceTransformFlag::eIdentity;
		}
	}

	VkSurfaceTransformFlagsKHR convert( ashes::SurfaceTransformFlags const & flags )
	{
		VkAccessFlags result{ 0 };

		if ( checkFlag( flags, ashes::SurfaceTransformFlag::eIdentity ) )
		{
			result |= VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}

		if ( checkFlag( flags, ashes::SurfaceTransformFlag::eRotate90 ) )
		{
			result |= VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR;
		}

		if ( checkFlag( flags, ashes::SurfaceTransformFlag::eRotate180 ) )
		{
			result |= VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR;
		}

		if ( checkFlag( flags, ashes::SurfaceTransformFlag::eRotate270 ) )
		{
			result |= VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR;
		}

		if ( checkFlag( flags, ashes::SurfaceTransformFlag::eHorizontalMirror ) )
		{
			result |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR;
		}

		if ( checkFlag( flags, ashes::SurfaceTransformFlag::eHorizontalMirrorRotate90 ) )
		{
			result |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR;
		}

		if ( checkFlag( flags, ashes::SurfaceTransformFlag::eHorizontalMirrorRotate180 ) )
		{
			result |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR;
		}

		if ( checkFlag( flags, ashes::SurfaceTransformFlag::eHorizontalMirrorRotate270 ) )
		{
			result |= VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR;
		}

		if ( checkFlag( flags, ashes::SurfaceTransformFlag::eInherit ) )
		{
			result |= VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR;
		}

		return result;
	}

	ashes::SurfaceTransformFlags convertSurfaceTransform( VkSurfaceTransformFlagsKHR const & flags )
	{
		ashes::SurfaceTransformFlags result;

		if ( ashes::checkFlag( flags, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR ) )
		{
			result |= ashes::SurfaceTransformFlag::eIdentity;
		}

		if ( ashes::checkFlag( flags, VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR ) )
		{
			result |= ashes::SurfaceTransformFlag::eRotate90;
		}

		if ( ashes::checkFlag( flags, VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR ) )
		{
			result |= ashes::SurfaceTransformFlag::eRotate180;
		}

		if ( ashes::checkFlag( flags, VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR ) )
		{
			result |= ashes::SurfaceTransformFlag::eRotate270;
		}

		if ( ashes::checkFlag( flags, VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR ) )
		{
			result |= ashes::SurfaceTransformFlag::eHorizontalMirror;
		}

		if ( ashes::checkFlag( flags, VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR ) )
		{
			result |= ashes::SurfaceTransformFlag::eHorizontalMirrorRotate90;
		}

		if ( ashes::checkFlag( flags, VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR ) )
		{
			result |= ashes::SurfaceTransformFlag::eHorizontalMirrorRotate180;
		}

		if ( ashes::checkFlag( flags, VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR ) )
		{
			result |= ashes::SurfaceTransformFlag::eHorizontalMirrorRotate270;
		}

		if ( ashes::checkFlag( flags, VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR ) )
		{
			result |= ashes::SurfaceTransformFlag::eInherit;
		}

		return result;
	}
}
