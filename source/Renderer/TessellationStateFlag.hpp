/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/FlagCombination.hpp>

namespace renderer
{
	/**
	*\brief
	*	Enumération des types d'accès.
	*/
	enum class TessellationStateFlag
		: uint32_t
	{
	};
	VkLib_ImplementFlag( TessellationStateFlag )
	/**
	*\brief
	*	Convertit un renderer::TessellationStateFlags en VkPipelineTessellationStateCreateFlags.
	*\param[in] flags
	*	Le renderer::TessellationStateFlags.
	*\return
	*	Le VkPipelineTessellationStateCreateFlags.
	*/
	VkPipelineTessellationStateCreateFlags convert( TessellationStateFlags const & flags );
}
