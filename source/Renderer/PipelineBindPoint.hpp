/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>

namespace renderer
{
	/**
	*\brief
	*	Enumération des points d'attache de tampon de commandes.
	*/
	enum class PipelineBindPoint
		: uint32_t
	{
		eGraphics,
		eCompute,
	};
	/**
	*\brief
	*	Convertit un renderer::PipelineBindPoint en VkPipelineBindPoint.
	*\param[in] point
	*	Le renderer::PipelineBindPoint.
	*\return
	*	Le VkPipelineBindPoint.
	*/
	VkPipelineBindPoint convert( PipelineBindPoint const & point );
}
