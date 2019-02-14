/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ComponentMapping_HPP___
#define ___Ashes_ComponentMapping_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Structure specifying a colour component mapping.
	*\~french
	*\brief
	*	Spécifie le mapping des composantes couleur.
	*/
	struct ComponentMapping
	{
		ComponentSwizzle r{ ComponentSwizzle::eIdentity };
		ComponentSwizzle g{ ComponentSwizzle::eIdentity };
		ComponentSwizzle b{ ComponentSwizzle::eIdentity };
		ComponentSwizzle a{ ComponentSwizzle::eIdentity };
	};
}

#endif
