/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_QueryControlFlag_HPP___
#define ___Ashes_QueryControlFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Enumération des contrôles de requête d'occlusion.
	*/
	enum class QueryControlFlag
		: uint32_t
	{
		ePrecise = 0x00000001,
	};
	Ashes_ImplementFlag( QueryControlFlag )
}

#endif
