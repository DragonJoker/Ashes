/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "ColourBlendStateAttachment.hpp"

#include <VkLib/ColourBlendState.hpp>

namespace renderer
{
	/**
	*\brief
	*	Wrapper de vk::ColourBlendState.
	*/
	class ColourBlendState
	{
	public:
		ColourBlendState( bool logicOpEnable = false
			, LogicOp logicOp = LogicOp::eCopy
			, utils::Vec4 const & blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f } );
		/**
		*\return
		*	Un ColourBlendState avec une attache couleur par défaut.
		*/
		static ColourBlendState const & createDefault();
		/**
		*\brief
		*	Ajoute une attache à l'état.
		*\param[in] attachment
		*	L'attache.
		*/
		void addAttachment( ColourBlendStateAttachment const & attachment );
		/**
		*\return
		*	Le vk::ColourBlendState.
		*/
		inline vk::ColourBlendState getState()const
		{
			return m_state;
		}

	private:
		vk::ColourBlendState m_state;
	};
}
