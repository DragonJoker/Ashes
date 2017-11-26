/*
This file belongs to Renderer.
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
	class RenderSubpassState
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] pipelineStage
		*	Les indicateurs d'état du pipeline.
		*\param[in] access
		*	Les indicateurs d'accès.
		*/
		RenderSubpassState( PipelineStageFlags pipelineStage
			, AccessFlags access );
		/**
		*\return
		*	Le vk::RenderPassState.
		*/
		inline vk::RenderSubpassState const & getState()const
		{
			return m_state;
		}

	private:
		vk::RenderSubpassState m_state;
	};
}
