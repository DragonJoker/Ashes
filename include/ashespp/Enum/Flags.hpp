/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Flags_HPP___
#define ___AshesPP_Flags_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	List the flags names of given value.
	*\param[in] value
	*	The flags.
	*\return
	*	The flags names.
	*/
	template< typename Enum >
	inline std::string getFlagsName( VkFlags value )
	{
		if ( value == 0u )
		{
			return "none";
		}

		Enum test = 0x00000001;
		std::string result;
		std::string sep;

		while ( test )
		{
			if ( checkFlag( value, test ) )
			{
				result += sep + getName( test );
				sep = " | ";
			}

			test <<= 1;
		}

		return result;
	}
}

#endif
