/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ViewportState_HPP___
#define ___Ashes_ViewportState_HPP___
#pragma once

#include "AshesRenderer/AshesRendererPrerequisites.hpp"
#include "AshesRenderer/Pipeline/Scissor.hpp"
#include "AshesRenderer/Pipeline/Viewport.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	The viewport state.
	*\~french
	*\brief
	*	L'état de viewport.
	*/
	struct ViewportState
	{
		ViewportStateFlags flags = ViewportStateFlags{};
		ViewportArray viewports;
		ScissorArray scissors;
	};

	inline bool operator==( ViewportArray const & lhs, ViewportArray const & rhs )
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

	inline bool operator==( ScissorArray const & lhs, ScissorArray const & rhs )
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

	inline bool operator==( ViewportState const & lhs, ViewportState const & rhs )
	{
		return lhs.flags == rhs.flags
			&& lhs.viewports == rhs.viewports
			&& lhs.scissors == rhs.scissors;
	}

	inline bool operator!=( ViewportState const & lhs, ViewportState const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
