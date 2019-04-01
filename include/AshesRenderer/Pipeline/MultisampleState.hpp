/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_MultisampleState_HPP___
#define ___Ashes_MultisampleState_HPP___
#pragma once

#include "AshesRenderer/AshesRendererPrerequisites.hpp"

namespace ashes
{
	/**
	*\~french
	*\brief
	*	Etat de multi-échantillonnage.
	*/
	struct MultisampleState
	{
		MultisampleStateFlags flags = 0;
		SampleCountFlag rasterisationSamples = SampleCountFlag::e1;
		bool sampleShadingEnable = false;
		float minSampleShading = 1.0f;
		uint32_t sampleMask = 0u;
		bool alphaToCoverageEnable = false;
		bool alphaToOneEnable = false;
	};

	inline bool operator==( MultisampleState const & lhs, MultisampleState const & rhs )
	{
		return lhs.flags == rhs.flags
			&& lhs.rasterisationSamples == rhs.rasterisationSamples
			&& lhs.sampleShadingEnable == rhs.sampleShadingEnable
			&& lhs.minSampleShading == rhs.minSampleShading
			&& lhs.alphaToCoverageEnable == rhs.alphaToCoverageEnable
			&& lhs.alphaToOneEnable == rhs.alphaToOneEnable;
	}

	inline bool operator!=( MultisampleState const & lhs, MultisampleState const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif