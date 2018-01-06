/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/Fence.hpp>
#include <Renderer/Fence.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Classe permettant la synchronisation des opérations sur une file.
	*/
	class Fence
		: public renderer::Fence
	{
	public:
		/**
		*\brief
		*	Constructeur
		*\param[in] device
		*	Le device parent.
		*\param[in] flags
		*	Les indicateurs de création de la barrière.
		*/ 
		Fence( renderer::Device const & device
			, renderer::FenceCreateFlags flags = 0 );
		/**
		*\brief
		*	Attend que la barrière soit signalée.
		*\param[in] timeout
		*	Le temps à attendre pour le signalement.
		*\return
		*	\p WaitResult::eSuccess ou \p WaitResult::eTimeOut en cas de succès.
		*/ 
		renderer::WaitResult wait( uint32_t timeout )const override;
		/**
		*\brief
		*	Remet la barrière en non signalée.
		*/ 
		void reset()const override;
		/**
		*\return
		*	La vk::Fence.
		*/
		inline vk::Fence const & getFence()const
		{
			return m_fence;
		}

	private:
		vk::Fence m_fence;
	};
}
