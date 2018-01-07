/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Renderer/RenderSubpassState.hpp>

namespace vk_renderer
{
	/**
	*\~french
	*\brief
	*	Description d'un état d'une sous passe de rendu.
	*\~english
	*\brief
	*	Describes the state of a render subpass.
	*/
	struct RenderSubpassState
	{
		//!\~french		Les indicateurs d'état du pipeline.
		//!\~english	Pipeline stage flags.
		VkPipelineStageFlags m_pipelineStage;
		//!\~french		Les indicateurs d'accès.
		//!\~english	Access flags.
		VkAccessFlags m_access;
	};
	/**
	*\brief
	*	Convertit un renderer::RenderSubpassState en vk::RenderSubpassState.
	*\param[in] state
	*	Le renderer::RenderSubpassState.
	*\return
	*	Le vk::RenderSubpassState.
	*/
	RenderSubpassState convert( renderer::RenderSubpassState const & state );
}
