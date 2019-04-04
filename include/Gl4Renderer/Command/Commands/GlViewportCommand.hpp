/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	/**
	*\brief
	*	Commande d'application d'un viewport.
	*/
	class ViewportCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] viewport
		*	Le viewport.
		*/
		ViewportCommand( VkDevice device
			, uint32_t firstViewport
			, VkViewportArray viewports );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkViewportArray m_viewports;
	};
}
