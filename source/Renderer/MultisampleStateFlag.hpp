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
	enum class MultisampleStateFlag
		: uint32_t
	{
	};
	VkLib_ImplementFlag( MultisampleStateFlag )
	/**
	*\brief
	*	Convertit un renderer::MultisampleStateFlags en VkPipelineMultisampleStateCreateFlags.
	*\param[in] flags
	*	Le renderer::MultisampleStateFlags.
	*\return
	*	Le VkPipelineMultisampleStateCreateFlags.
	*/
	VkPipelineMultisampleStateCreateFlags convert( MultisampleStateFlags const & flags );
}
