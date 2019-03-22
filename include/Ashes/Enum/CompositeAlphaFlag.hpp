/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_CompositeAlphaFlag_HPP___
#define ___Ashes_CompositeAlphaFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Alpha composite flags.
	*\~french
	*\brief
	*	Indicateurs de composition alpha.
	*/
	enum class CompositeAlphaFlag
		: uint32_t
	{
		eNone = 0x00000000,
		eOpaque = 0x00000001,
		ePreMultiplied = 0x00000002,
		ePostMultiplied = 0x00000004,
		eInherit = 0x00000008,
	};
	Ashes_ImplementFlag( CompositeAlphaFlag )
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
	*	R�cup�re le nom du type d'�l�ment donn�.
	*\param[in] value
	*	Le type d'�l�ment.
	*\return
	*	Le nom.
	*/
	inline std::string getName( CompositeAlphaFlags value )
	{
		std::string result;
		std::string sep;

		if ( checkFlag( value, CompositeAlphaFlag::eOpaque ) )
		{
			result += sep + "opaque";
			sep = " | ";
		}

		if ( checkFlag( value, CompositeAlphaFlag::ePreMultiplied ) )
		{
			result += sep + "pre_multiplied";
			sep = " | ";
		}

		if ( checkFlag( value, CompositeAlphaFlag::ePostMultiplied ) )
		{
			result += sep + "post_multiplied";
			sep = " | ";
		}

		if ( checkFlag( value, CompositeAlphaFlag::eInherit ) )
		{
			result += sep + "inherit";
			sep = " | ";
		}

		return result;
	}
}

#endif
