/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	/**
	*\brief
	*	Classe permettant la synchronisation des opérations sur une file.
	*/
	class Fence
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
		Fence( VkDevice device
			, VkFenceCreateFlags flags = 0 );
		/**
		*\brief
		*	Destructeur
		*/
		~Fence();
		/**
		*\brief
		*	Attend que la barrière soit signalée.
		*\param[in] timeout
		*	Le temps à attendre pour le signalement.
		*\return
		*	\p WaitResult::eSuccess ou \p WaitResult::eTimeOut en cas de succès.
		*/ 
		VkResult wait( uint64_t timeout )const;
		/**
		*\brief
		*	Remet la barrière en non signalée.
		*/ 
		void reset()const;

		inline GLsync getInternal()const
		{
			return m_fence;
		}

	private:
		VkDevice m_device;
		mutable GLsync m_fence{ nullptr };
	};
}
