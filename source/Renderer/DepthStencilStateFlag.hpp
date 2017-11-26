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
	enum class DepthStencilStateFlag
		: uint32_t
	{
	};
	VkLib_ImplementFlag( DepthStencilStateFlag )
	/**
	*\brief
	*	Convertit un renderer::DepthStencilStateFlags en VkPipelineDepthStencilStateCreateFlags.
	*\param[in] flags
	*	Le renderer::DepthStencilStateFlags.
	*\return
	*	Le VkPipelineDepthStencilStateCreateFlags.
	*/
	VkPipelineDepthStencilStateCreateFlags convert( DepthStencilStateFlags const & flags );
}
