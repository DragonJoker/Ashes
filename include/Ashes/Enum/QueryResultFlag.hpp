/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_QueryResultFlag_HPP___
#define ___Ashes_QueryResultFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Enumération des résultats de requête d'occlusion.
	*/
	enum class QueryResultFlag
		: uint32_t
	{
		e64 = 0x00000001,
		eWait = 0x00000002,
		eWithAvailability = 0x00000004,
		ePartial = 0x00000008,
	};
	Ashes_ImplementFlag( QueryResultFlag )
}

#endif
