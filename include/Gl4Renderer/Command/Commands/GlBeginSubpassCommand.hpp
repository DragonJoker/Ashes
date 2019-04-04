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
	*	Démarre une passe de rendu en bindant son framebuffer, et en le vidant au besoin.
	*/
	class BeginSubpassCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] renderPass
		*	La passe de rendu.
		*\param[in] frameBuffer
		*	Le tampon d'image affecté par le rendu.
		*\param[in] clearValues
		*	Les valeurs de vidage, une par attache de la passe de rendu.
		*\param[in] contents
		*	Indique la manière dont les commandes de la première sous-passe sont fournies.
		*/
		BeginSubpassCommand( VkDevice device
			, VkRenderPass renderPass
			, VkFramebuffer frameBuffer
			, VkSubpassDescription subpass );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkRenderPass m_renderPass;
		VkSubpassDescription m_subpass;
		VkFramebuffer m_frameBuffer;
	};
}
