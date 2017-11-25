/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#pragma once

#include "RendererPrerequisites.hpp"

namespace renderer
{
	/**
	*\brief
	*	Description d'un état d'une passe de rendu.
	*/
	class RenderPassState
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] pipelineStage
		*	Les indicateurs d'état du pipeline.
		*\param[in] access
		*	Les indicateurs d'accès.
		*\param[in] imageLayouts
		*	Le layout des attaches.
		*/
		RenderPassState( PipelineStageFlags pipelineStage
			, AccessFlags access
			, std::vector< ImageLayout > const & imageLayouts );
		/**
		*\return
		*	Le vk::RenderPassState.
		*/
		inline vk::RenderPassState const & getState()const
		{
			return m_state;
		}

	private:
		vk::RenderPassState m_state;
	};
}
