/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::D3D11_NAMESPACE
{
	/**
	*\brief
	*	Commande de fin d'une passe de rendu, unbind son framebuffer.
	*/
	class EndRenderPassCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		EndRenderPassCommand( VkDevice device
			, VkRenderPass renderPass
			, VkFramebuffer frameBuffer );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkRenderPass m_renderPass;
		VkFramebuffer m_frameBuffer;
	};
}
