/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_VertexInputState_HPP___
#define ___Ashes_VertexInputState_HPP___
#pragma once

#include "VertexInputAttributeDescription.hpp"
#include "VertexInputBindingDescription.hpp"

#include <vector>

namespace ashes
{
	/**
	*\~french
	*\brief
	*	Structure specifying parameters of a vertex input state.
	*\~french
	*\brief
	*	Structure décrivant les paramètres d'un état d'entrée de sommets.
	*/
	struct VertexInputState
	{
		VertexInputBindingDescriptionArray vertexBindingDescriptions;
		VertexInputAttributeDescriptionArray vertexAttributeDescriptions;

		static VertexInputState create( VertexLayout const & vertexLayout );
		static VertexInputState create( VertexLayoutCRefArray const & vertexLayouts );
	};
}

#endif
