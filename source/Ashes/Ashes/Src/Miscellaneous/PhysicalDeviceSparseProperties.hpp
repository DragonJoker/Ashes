/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_PhysicalDeviceSparseProperties_HPP___
#define ___Ashes_PhysicalDeviceSparseProperties_HPP___
#pragma once

#include "AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies physical device sparse memory properties.
	*\~french
	*\brief
	*	Spécifie les propriétés de mémoire dispersée d'un périphérique physique.
	*/
	struct PhysicalDeviceSparseProperties
	{
		bool residencyStandard2DBlockShape;
		bool residencyStandard2DMultisampleBlockShape;
		bool residencyStandard3DBlockShape;
		bool residencyAlignedMipSize;
		bool residencyNonResidentStrict;
	};
}

#endif
