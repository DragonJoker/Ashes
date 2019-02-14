#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkFormatFeatureFlags convert( ashes::FormatFeatureFlags const & flags )
	{
		VkFormatFeatureFlags result{ 0 };

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eSampledImage ) )
		{
			result |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eStorageImage ) )
		{
			result |= VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eStorageImageAtomic ) )
		{
			result |= VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eUniformTexelBuffer ) )
		{
			result |= VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eStorageTexelBuffer ) )
		{
			result |= VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eStorageTexelBufferAtomic ) )
		{
			result |= VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eVertexBuffer ) )
		{
			result |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eColourAttachment ) )
		{
			result |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eColourAttachmentBlend ) )
		{
			result |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eDepthStencilAttachment ) )
		{
			result |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eBlitSrc ) )
		{
			result |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eBlitDst ) )
		{
			result |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eSampledImageFilterLinear ) )
		{
			result |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eSampledImageFilterCubic ) )
		{
			result |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eTransferSrc ) )
		{
			result |= VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eTransferDst ) )
		{
			result |= VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eMidPointChromaSamples ) )
		{
			result |= VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eSampledImageYCbCrConversionLinearFilter ) )
		{
			result |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eSampledImageYCbCrConversionSeparateReconstructionFilter ) )
		{
			result |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eSampledImageYCbCrConversionChromaReconstructionExplicit ) )
		{
			result |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eSampledImageYCbCrConversionChromaReconstructionExplicitForceable ) )
		{
			result |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eDisjoint ) )
		{
			result |= VK_FORMAT_FEATURE_DISJOINT_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eCositedChromaSamples ) )
		{
			result |= VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT;
		}

		if ( checkFlag( flags, ashes::FormatFeatureFlag::eSampledImageFilterMinmax ) )
		{
			result |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT_EXT;
		}

		return result;
	}

	ashes::FormatFeatureFlags convertFormatFeatureFlags( VkFormatFeatureFlags const & flags )
	{
		ashes::FormatFeatureFlags result{ 0 };

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eSampledImage;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eStorageImage;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eStorageImageAtomic;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eUniformTexelBuffer;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eStorageTexelBuffer;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eStorageTexelBufferAtomic;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eVertexBuffer;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eColourAttachment;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eColourAttachmentBlend;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eDepthStencilAttachment;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_BLIT_SRC_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eBlitSrc;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_BLIT_DST_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eBlitDst;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eSampledImageFilterLinear;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG ) )
		{
			result |= ashes::FormatFeatureFlag::eSampledImageFilterCubic;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_TRANSFER_SRC_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eTransferSrc;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_TRANSFER_DST_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eTransferDst;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eMidPointChromaSamples;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eSampledImageYCbCrConversionLinearFilter;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eSampledImageYCbCrConversionSeparateReconstructionFilter;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eSampledImageYCbCrConversionChromaReconstructionExplicit;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eSampledImageYCbCrConversionChromaReconstructionExplicitForceable;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_DISJOINT_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eDisjoint;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT ) )
		{
			result |= ashes::FormatFeatureFlag::eCositedChromaSamples;
		}

		if ( ashes::checkFlag( flags, VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT_EXT ) )
		{
			result |= ashes::FormatFeatureFlag::eSampledImageFilterMinmax;
		}

		return result;
	}
}
