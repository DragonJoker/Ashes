/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>
#include <Renderer/AttributeFormat.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::AttributeFormat en VkFormat.
	*\param[in] flags
	*	Le renderer::AttributeFormat.
	*\return
	*	Le VkFormat.
	*/
	VkFormat convert( renderer::AttributeFormat format );
	/**
	*\brief
	*	Récupère la taille en octets du format donné.
	*\param[in] flags
	*	Le renderer::AttributeFormat.
	*\return
	*	La taille.
	*/
	uint32_t getSize( renderer::AttributeFormat format );
}
