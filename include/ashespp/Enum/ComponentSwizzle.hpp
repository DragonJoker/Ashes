/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ComponentSwizzle_HPP___
#define ___AshesPP_ComponentSwizzle_HPP___
#pragma once

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*\~french
	*\brief
	*	Récupère le nom du type d'élément donné.
	*\param[in] value
	*	Le type d'élément.
	*\return
	*	Le nom.
	*/
	inline std::string getName( VkComponentSwizzle value )
	{
		switch ( value )
		{
		case VK_COMPONENT_SWIZZLE_IDENTITY:
			return "identity";
		case VK_COMPONENT_SWIZZLE_ZERO:
			return "zero";
		case VK_COMPONENT_SWIZZLE_ONE:
			return "one";
		case VK_COMPONENT_SWIZZLE_R:
			return "r";
		case VK_COMPONENT_SWIZZLE_G:
			return "g";
		case VK_COMPONENT_SWIZZLE_B:
			return "b";
		case VK_COMPONENT_SWIZZLE_A:
			return "a";
		default:
			assert( false && "Unsupported VkComponentSwizzle." );
			throw std::runtime_error{ "Unsupported VkComponentSwizzle" };
		}

		return 0;
	}
}

#endif
