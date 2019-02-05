/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DynamicState_HPP___
#define ___Ashes_DynamicState_HPP___
#pragma once

#include "AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	The dynamic state.
	*\~french
	*\brief
	*	L'état dynamique.
	*/
	struct DynamicState
	{
		DynamicStateFlags flags = DynamicStateFlags{};
		DynamicStateEnableArray dynamicStates;
	};

	inline bool operator==( DynamicStateEnableArray const & lhs, DynamicStateEnableArray const & rhs )
	{
		auto result = lhs.size() == rhs.size();
		auto itLhs = lhs.begin();
		auto itRhs = rhs.begin();

		while ( result && itLhs != lhs.end() )
		{
			result = *itLhs == *itRhs;
		}

		return result;
	}

	inline bool operator==( DynamicState const & lhs, DynamicState const & rhs )
	{
		return lhs.flags == rhs.flags
			&& lhs.dynamicStates == rhs.dynamicStates;
	}

	inline bool operator!=( DynamicState const & lhs, DynamicState const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
