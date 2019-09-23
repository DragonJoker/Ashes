/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ImageCreateFlags_HPP___
#define ___AshesPP_ImageCreateFlags_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkImageCreateFlagBits value )
	{
		switch ( value )
		{
		case VK_IMAGE_CREATE_SPARSE_BINDING_BIT:
			return "sparse_binding";
		case VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT:
			return "sparse_residency";
		case VK_IMAGE_CREATE_SPARSE_ALIASED_BIT:
			return "sparse_aliased";
		case VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT:
			return "mutable_format";
		case VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT:
			return "cube_compatible";
		case VK_IMAGE_CREATE_ALIAS_BIT:
			return "alias";
		case VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT:
			return "split_instance_bind_regions";
		case VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT:
			return "2d_array_compatible";
		case VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT:
			return "block_texel_view_compatible";
		case VK_IMAGE_CREATE_EXTENDED_USAGE_BIT:
			return "extended_usage";
		case VK_IMAGE_CREATE_PROTECTED_BIT:
			return "protected";
		case VK_IMAGE_CREATE_DISJOINT_BIT:
			return "disjoint";
#ifdef VK_NV_corner_sampled_image
		case VK_IMAGE_CREATE_CORNER_SAMPLED_BIT_NV:
			return "corner_sampled";
#endif
#ifdef VK_EXT_sample_locations
		case VK_IMAGE_CREATE_SAMPLE_LOCATIONS_COMPATIBLE_DEPTH_BIT_EXT:
			return "sample_locations_compatible_depth";
#endif
#ifdef VK_EXT_fragment_density_map
		case VK_IMAGE_CREATE_SUBSAMPLED_BIT_EXT:
			return "subsampled";
#endif
		default:
			assert( false && "Unsupported VkImageCreateFlagBits." );
			throw std::runtime_error{ "Unsupported VkImageCreateFlagBits" };
		}

		return 0;
	}
}

#endif
