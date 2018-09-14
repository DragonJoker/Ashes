/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_FormatFeatureFlag_HPP___
#define ___Ashes_FormatFeatureFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Enumération des capacités mémoire.
	*/
	enum class FormatFeatureFlag
		: int32_t
	{
		eSampledImage = 0x00000001,
		eStorageImage = 0x00000002,
		eStorageImageAtomic = 0x00000004,
		eUniformTexelBuffer = 0x00000008,
		eStorageTexelBuffer = 0x00000010,
		eStorageTexelBufferAtomic = 0x00000020,
		eVertexBuffer = 0x00000040,
		eColourAttachment = 0x00000080,
		eColourAttachmentBlend = 0x00000100,
		eDepthStencilAttachment = 0x00000200,
		eBlitSrc = 0x00000400,
		eBlitDst = 0x00000800,
		eSampledImageFilterLinear = 0x00001000,
		eSampledImageFilterCubic = 0x00002000,
		eTransferSrc = 0x00004000,
		eTransferDst = 0x00008000,
		eMidPointChromaSamples = 0x00020000,
		eSampledImageYCbCrConversionLinearFilter = 0x00040000,
		eSampledImageYCbCrConversionSeparateReconstructionFilter = 0x00080000,
		eSampledImageYCbCrConversionChromaReconstructionExplicit = 0x00100000,
		eSampledImageYCbCrConversionChromaReconstructionExplicitForceable = 0x00200000,
		eDisjoint = 0x00400000,
		eCositedChromaSamples = 0x00800000,
		eSampledImageFilterMinmax = 0x00010000,
	};
	Ashes_ImplementFlag( FormatFeatureFlag )
}

#endif
