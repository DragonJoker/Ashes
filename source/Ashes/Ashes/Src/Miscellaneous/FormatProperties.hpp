/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_FormatProperties_HPP___
#define ___Ashes_FormatProperties_HPP___
#pragma once

#include "AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies image format properties.
	*\~french
	*\brief
	*	Définit les propriétés de format d'une image.
	*/
	struct FormatProperties
	{
		FormatFeatureFlags linearTilingFeatures;
		FormatFeatureFlags optimalTilingFeatures;
		FormatFeatureFlags bufferFeatures;
	};
}

#endif
