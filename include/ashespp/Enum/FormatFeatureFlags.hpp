/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_FormatFeatureFlags_HPP___
#define ___AshesPP_FormatFeatureFlags_HPP___
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
	inline std::string getName( VkFormatFeatureFlagBits value )
	{
		switch ( value )
		{
		case VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT:
			return "sampled_image";
		case VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT:
			return "storage_image";
		case VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT:
			return "storage_image_atomic";
		case VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT:
			return "uniform_texel_buffer";
		case VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT:
			return "storage_texel_buffer";
		case VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT:
			return "storage_texel_buffer_atomic";
		case VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT:
			return "vertex_buffer";
		case VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT:
			return "color_attachment";
		case VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT:
			return "color_attachment_blend";
		case VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT:
			return "depth_stencil_attachment";
		case VK_FORMAT_FEATURE_BLIT_SRC_BIT:
			return "blit_src";
		case VK_FORMAT_FEATURE_BLIT_DST_BIT:
			return "blit_dst";
		case VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT:
			return "sampled_image_filter_linear";
#if defined( VK_API_VERSION_1_2 )
		case VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT:
			return "sampled_image_filter_minmax";
#elif defined( VK_EXT_sampler_filter_minmax )
		case VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT_EXT:
			return "sampled_image_filter_minmax";
#endif
#ifdef VK_API_VERSION_1_1
		case VK_FORMAT_FEATURE_TRANSFER_SRC_BIT:
			return "transfer_src";
		case VK_FORMAT_FEATURE_TRANSFER_DST_BIT:
			return "transfer_dst";
		case VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT:
			return "midpoint_chroma_samples";
		case VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT:
			return "sampled_image_ycbcr_conversion_linear_filter";
		case VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT:
			return "sampled_image_ycbcr_conversion_separate_reconstruction_filter";
		case VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT:
			return "sampled_image_ycbcr_conversion_chroma_reconstruction_explicit";
		case VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT:
			return "sampled_image_ycbcr_conversion_chroma_reconstruction_explicit_forceable";
		case VK_FORMAT_FEATURE_DISJOINT_BIT:
			return "disjoint";
		case VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT:
			return "cosited_chroma_samples";
#else
#	ifdef VK_KHR_maintenance1
		case VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR:
			return "transfer_src";
		case VK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR:
			return "transfer_dst";
#	endif
#	ifdef VK_KHR_sampler_ycbcr_conversion
		case VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT_KHR:
			return "midpoint_chroma_samples";
		case VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT_KHR:
			return "sampled_image_ycbcr_conversion_linear_filter";
		case VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT_KHR:
			return "sampled_image_ycbcr_conversion_separate_reconstruction_filter";
		case VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT_KHR:
			return "sampled_image_ycbcr_conversion_chroma_reconstruction_explicit";
		case VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT_KHR:
			return "sampled_image_ycbcr_conversion_chroma_reconstruction_explicit_forceable";
		case VK_FORMAT_FEATURE_DISJOINT_BIT_KHR:
			return "disjoint";
		case VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT_KHR:
			return "cosited_chroma_samples";
#	endif
#endif
#ifdef VK_EXT_filter_cubic
		case VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG:
			return "sampled_image_filter_cubic";
#endif
#ifdef VK_EXT_fragment_density_map
		case VK_FORMAT_FEATURE_FRAGMENT_DENSITY_MAP_BIT_EXT:
			return "fragment_density_map";
#endif
		default:
			assert( false && "Unsupported VkDeviceQueueCreateFlagBits." );
			throw std::runtime_error{ "Unsupported VkDeviceQueueCreateFlagBits" };
		}

		return 0;
	}
}

#endif
