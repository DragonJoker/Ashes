/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Sync/Fence.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Classe permettant la synchronisation des opérations sur une file.
	*/
	class Fence
		: public ashes::Fence
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
		Fence( Device const & device
			, ashes::FenceCreateFlags flags = 0 );
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
		ashes::WaitResult wait( uint64_t timeout )const override;
		/**
		*\brief
		*	Remet la barrière en non signalée.
		*/ 
		void reset()const override;

		inline GLsync getSync()const
		{
			return m_fence;
		}

	private:
		Device const & m_device;
		mutable GLsync m_fence{ nullptr };
	};
}
