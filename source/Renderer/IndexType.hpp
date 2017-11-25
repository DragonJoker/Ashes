/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>

namespace renderer
{
	/**
	*\brief
	*	Enumération des types d'indices.
	*/
	enum class IndexType
	{
		eUInt16,
		eUInt32,
	};
	/**
	*\brief
	*	Convertit un renderer::IndexType en VkIndexType.
	*\param[in] type
	*	Le renderer::IndexType.
	*\return
	*	Le VkIndexType.
	*/
	VkIndexType convert( IndexType const & type );
}
